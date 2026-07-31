/**
 * @file   core.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  Application task supervisor and control-plane dispatcher.
 * @date   2025-03-21
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "tasks/core.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

#include <cmsis_os2.h>

#include "base/bootstrap.h"
#include "tasks/bluetooth.hpp"
#include "tasks/gui.hpp"
#include "tasks/sensor.hpp"
#include "tasks/storage.hpp"

namespace bd::task {
namespace {
constexpr uint32_t core_inbox_capacity{16U};

enum class TaskState : uint8_t {
  Dormant,
  Created,
  Ready,
  Failed,
};

enum class FailurePolicy : uint8_t {
  Continue,
  HaltSystem,
};

struct TaskSpec {
  TaskId id;
  const char *name;
  osThreadFunc_t entry;
  uint32_t stack_size;
  osPriority_t priority;
  FailurePolicy failure_policy;
};

class ManagedTask {
public:
  bool create(const TaskSpec &spec, void *argument = nullptr) {
    if (state_ != TaskState::Dormant) {
      return false;
    }

    osThreadAttr_t attributes{};
    attributes.name = spec.name;
    attributes.stack_size = spec.stack_size;
    attributes.priority = spec.priority;

    spec_ = &spec;
    handle_ = osThreadNew(spec.entry, argument, &attributes);
    state_ = handle_ == nullptr ? TaskState::Failed : TaskState::Created;
    return handle_ != nullptr;
  }

  bool markReady() {
    if (state_ != TaskState::Created) {
      return false;
    }

    state_ = TaskState::Ready;
    return true;
  }

  void markFailed() { state_ = TaskState::Failed; }

  [[nodiscard]] constexpr const TaskSpec *spec() const { return spec_; }

private:
  const TaskSpec *spec_{nullptr};
  osThreadId_t handle_{nullptr};
  TaskState state_{TaskState::Dormant};
};

class TaskRegistry {
public:
  bool create(const TaskSpec &spec, void *argument = nullptr) {
    auto *task{find(spec.id)};
    return task != nullptr && task->create(spec, argument);
  }

  bool markReady(TaskId id) {
    auto *task{find(id)};
    return task != nullptr && task->markReady();
  }

  void markFailed(TaskId id) {
    if (auto *task{find(id)}; task != nullptr) {
      task->markFailed();
    }
  }

  [[nodiscard]] const TaskSpec *spec(TaskId id) const {
    const auto *task{find(id)};
    return task == nullptr ? nullptr : task->spec();
  }

private:
  static constexpr std::size_t task_count{
      static_cast<std::size_t>(TaskId::Count)};

  [[nodiscard]] ManagedTask *find(TaskId id) {
    const auto index{static_cast<std::size_t>(id)};
    return index < tasks_.size() ? &tasks_[index] : nullptr;
  }

  [[nodiscard]] const ManagedTask *find(TaskId id) const {
    const auto index{static_cast<std::size_t>(id)};
    return index < tasks_.size() ? &tasks_[index] : nullptr;
  }

  std::array<ManagedTask, task_count> tasks_{};
};

enum class CoreCommandType : uint8_t {
  TaskReady,
  TaskFault,
};

struct CoreCommand {
  CoreCommandType type;
  TaskId source;
};

constexpr TaskSpec core_spec{
    TaskId::Core, "Core", CoreTask, 128U * 8U, osPriorityNormal,
    FailurePolicy::HaltSystem};

constexpr std::array worker_specs{
    TaskSpec{TaskId::Gui, "Gui", GuiTask, 128U * 24U, osPriorityLow,
             FailurePolicy::HaltSystem},
    TaskSpec{TaskId::Sensor, "Sensor", SensorTask, 128U * 4U,
             osPriorityNormal, FailurePolicy::Continue},
    TaskSpec{TaskId::Storage, "Storage", StorageTask, 128U * 16U,
             osPriorityNormal, FailurePolicy::Continue},
    TaskSpec{TaskId::Bluetooth, "Bluetooth", BluetoothTask, 128U * 4U,
             osPriorityAboveNormal, FailurePolicy::Continue},
};

osMessageQueueId_t core_inbox{nullptr};

class CoreRuntime {
public:
  bool prepare() {
    if (core_inbox != nullptr) {
      return false;
    }

    core_inbox =
        osMessageQueueNew(core_inbox_capacity, sizeof(CoreCommand), nullptr);
    if (core_inbox == nullptr || !prepareStorageChannel()) {
      return false;
    }

    return registry_.create(core_spec, this);
  }

  [[noreturn]] void run() {
    if (!registry_.markReady(TaskId::Core) || !startWorkers()) {
      app_fatal();
    }

    while (true) {
      CoreCommand command{};
      if (osMessageQueueGet(core_inbox, &command, nullptr, osWaitForever) !=
          osOK) {
        app_fatal();
      }

      dispatch(command);
    }
  }

private:
  bool startWorkers() {
    bool critical_tasks_started{true};

    for (const auto &spec : worker_specs) {
      if (!registry_.create(spec) &&
          spec.failure_policy == FailurePolicy::HaltSystem) {
        critical_tasks_started = false;
      }
    }

    return critical_tasks_started;
  }

  void dispatch(const CoreCommand &command) {
    switch (command.type) {
    case CoreCommandType::TaskReady:
      if (!registry_.markReady(command.source)) {
        handleFault(command.source);
      }
      break;

    case CoreCommandType::TaskFault:
      handleFault(command.source);
      break;
    }
  }

  void handleFault(TaskId source) {
    registry_.markFailed(source);
    const auto *spec{registry_.spec(source)};
    if (spec == nullptr ||
        spec->failure_policy == FailurePolicy::HaltSystem) {
      app_fatal();
    }
  }

  TaskRegistry registry_{};
};

CoreRuntime runtime{};

osStatus_t postCoreCommand(const CoreCommand &command, uint32_t timeout = 0U) {
  if (core_inbox == nullptr) {
    return osErrorResource;
  }

  return osMessageQueuePut(core_inbox, &command, 0U, timeout);
}
} // namespace

bool prepareRuntime() { return runtime.prepare(); }

osStatus_t reportTaskReady(TaskId source) {
  return postCoreCommand(CoreCommand{CoreCommandType::TaskReady, source});
}

osStatus_t reportTaskFault(TaskId source) {
  return postCoreCommand(CoreCommand{CoreCommandType::TaskFault, source});
}

[[noreturn]] void CoreTask(void *argument) {
  auto *core_runtime{static_cast<CoreRuntime *>(argument)};
  if (core_runtime == nullptr) {
    app_fatal();
  }

  core_runtime->run();
}
} // namespace bd::task

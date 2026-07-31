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
#include <cstdint>

#include <cmsis_os2.h>

#include "base/bootstrap.h"
#include "func/task.hpp"
#include "tasks/bluetooth.hpp"
#include "tasks/core_messages.hpp"
#include "tasks/gui.hpp"
#include "tasks/sensor.hpp"
#include "tasks/storage.hpp"

namespace bd::task {
namespace {
constexpr uint32_t core_inbox_capacity{16U};

constexpr TaskSpec core_spec{
    TaskId::Core, "Core", CoreTask, 128U * 8U, osPriorityNormal, true};

constexpr std::array worker_specs{
    TaskSpec{TaskId::Gui, "Gui", GuiTask, 128U * 24U, osPriorityLow, true},
    TaskSpec{TaskId::Sensor, "Sensor", SensorTask, 128U * 4U, osPriorityNormal,
             false},
    TaskSpec{TaskId::Storage, "Storage", StorageTask, 128U * 8U,
             osPriorityNormal, false},
    TaskSpec{TaskId::Bluetooth, "Bluetooth", BluetoothTask, 128U * 4U,
             osPriorityAboveNormal, false},
};

enum class RuntimeState : uint8_t {
  Booting,
  Running,
  Degraded,
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
    if (!startWorkers()) {
      app_fatal();
    }

    updateRuntimeState();

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
    bool required_tasks_started{true};

    for (const auto &spec : worker_specs) {
      if (!registry_.create(spec) && spec.required) {
        required_tasks_started = false;
      }
    }

    return required_tasks_started;
  }

  void dispatch(const CoreCommand &command) {
    switch (command.type) {
    case CoreCommandType::TaskReady:
      if (!registry_.markReady(command.source)) {
        registry_.markFailed(command.source);
      }
      updateRuntimeState();
      break;

    case CoreCommandType::TaskFault:
      registry_.markFailed(command.source);
      if (const auto *spec{registry_.spec(command.source)};
          spec != nullptr && spec->required) {
        app_fatal();
      }
      updateRuntimeState();
      break;

    case CoreCommandType::DisplayIdle:
      setDisplayIdle(true);
      break;

    case CoreCommandType::DisplayActive:
      setDisplayIdle(false);
      break;
    }
  }

  void setDisplayIdle(bool idle) {
    if (display_idle_ == idle) {
      return;
    }

    const auto signal{
        idle ? static_cast<uint32_t>(GuiSignal::DisplayOff)
             : static_cast<uint32_t>(GuiSignal::DisplayOn)};
    const auto flags{registry_.signal(TaskId::Gui, signal)};
    if ((flags & osFlagsError) != 0U) {
      app_fatal();
    }

    display_idle_ = idle;
  }

  void updateRuntimeState() {
    bool pending{false};
    bool degraded{false};

    for (const auto &spec : worker_specs) {
      switch (registry_.state(spec.id)) {
      case TaskState::Dormant:
      case TaskState::Created:
        pending = true;
        break;
      case TaskState::Ready:
        break;
      case TaskState::Failed:
        if (spec.required) {
          app_fatal();
        }
        degraded = true;
        break;
      }
    }

    if (!pending) {
      state_ = degraded ? RuntimeState::Degraded : RuntimeState::Running;
    }
  }

  TaskRegistry registry_{};
  RuntimeState state_{RuntimeState::Booting};
  bool display_idle_{false};
};

CoreRuntime runtime{};
} // namespace

bool prepareRuntime() { return runtime.prepare(); }

osStatus_t postCoreCommand(const CoreCommand &command, uint32_t timeout) {
  if (core_inbox == nullptr) {
    return osErrorResource;
  }

  return osMessageQueuePut(core_inbox, &command, 0U, timeout);
}

osStatus_t reportTaskReady(TaskId source) {
  return postCoreCommand(
      CoreCommand{CoreCommandType::TaskReady, source, 0});
}

osStatus_t reportTaskFault(TaskId source, int32_t detail) {
  return postCoreCommand(
      CoreCommand{CoreCommandType::TaskFault, source, detail});
}

[[noreturn]] void CoreTask(void *argument) {
  auto *core_runtime{static_cast<CoreRuntime *>(argument)};
  if (core_runtime == nullptr) {
    app_fatal();
  }

  core_runtime->run();
}
} // namespace bd::task

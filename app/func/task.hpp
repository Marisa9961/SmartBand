/**
 * @file   task.hpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  task define
 * @date   2025-03-21
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef __FUNC_TASK_HPP__
#define __FUNC_TASK_HPP__

#include <array>
#include <cstddef>
#include <cstdint>

#include <cmsis_os2.h>

namespace bd {
enum class TaskId : uint8_t {
  Core,
  Gui,
  Sensor,
  Storage,
  Bluetooth,
  Count,
};

enum class TaskState : uint8_t {
  Dormant,
  Created,
  Ready,
  Failed,
};

struct TaskSpec {
  TaskId id;
  const char *name;
  osThreadFunc_t entry;
  uint32_t stack_size;
  osPriority_t priority;
  bool required;
};

class ManagedTask {
public:
  bool create(const TaskSpec &spec, void *argument = nullptr);
  bool markReady();
  void markFailed();

  [[nodiscard]] constexpr const TaskSpec *spec() const { return spec_; }

private:
  const TaskSpec *spec_{nullptr};
  osThreadId_t handle_{nullptr};
  TaskState state_{TaskState::Dormant};
};

class TaskRegistry {
public:
  bool create(const TaskSpec &spec, void *argument = nullptr);
  bool markReady(TaskId id);
  void markFailed(TaskId id);

  [[nodiscard]] const TaskSpec *spec(TaskId id) const;

private:
  static constexpr std::size_t task_count{
      static_cast<std::size_t>(TaskId::Count)};

  [[nodiscard]] ManagedTask *find(TaskId id);
  [[nodiscard]] const ManagedTask *find(TaskId id) const;

  std::array<ManagedTask, task_count> tasks_{};
};

} // namespace bd

#endif

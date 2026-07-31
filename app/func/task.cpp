/**
 * @file   task.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  task impl
 * @date   2025-03-21
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "func/task.hpp"

namespace bd {
bool ManagedTask::create(const TaskSpec &spec, void *argument) {
  if (state_ != TaskState::Dormant) {
    return false;
  }

  spec_ = &spec;

  osThreadAttr_t attr{};

  attr.name = spec.name;
  attr.stack_size = spec.stack_size;
  attr.priority = spec.priority;

  handle_ = osThreadNew(spec.entry, argument, &attr);
  state_ = handle_ == nullptr ? TaskState::Failed : TaskState::Created;

  return handle_ != nullptr;
}

bool ManagedTask::markReady() {
  if (state_ != TaskState::Created) {
    return false;
  }

  state_ = TaskState::Ready;
  return true;
}

void ManagedTask::markFailed() { state_ = TaskState::Failed; }

ManagedTask *TaskRegistry::find(TaskId id) {
  const auto index{static_cast<std::size_t>(id)};
  return index < tasks_.size() ? &tasks_[index] : nullptr;
}

const ManagedTask *TaskRegistry::find(TaskId id) const {
  const auto index{static_cast<std::size_t>(id)};
  return index < tasks_.size() ? &tasks_[index] : nullptr;
}

bool TaskRegistry::create(const TaskSpec &spec, void *argument) {
  auto *task{find(spec.id)};
  return task != nullptr && task->create(spec, argument);
}

bool TaskRegistry::markReady(TaskId id) {
  auto *task{find(id)};
  return task != nullptr && task->markReady();
}

void TaskRegistry::markFailed(TaskId id) {
  if (auto *task{find(id)}; task != nullptr) {
    task->markFailed();
  }
}

const TaskSpec *TaskRegistry::spec(TaskId id) const {
  const auto *task{find(id)};
  return task == nullptr ? nullptr : task->spec();
}

} // namespace bd

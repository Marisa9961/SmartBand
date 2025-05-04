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
// Notice: UID helps with threads message communication
// But only support up to 31 existed tasks
// using like this -> osThreadFlagsSet(gui.getHandle(), core.getUid())
// From CORE task send message to GUI task
Task::Task(osThreadFunc_t func, const char *name, uint32_t stack_size,
           osPriority_t priority)
    : name_(name), stack_size_(stack_size), priority_(priority),
      handle_(nullptr), func_(func) {
  static Flag uid = 1;
  task_flag_ = uid;
  uid <<= 1;
}

bool Task::run(void *argument) {
  osThreadAttr_t attr{};

  attr.name = name_;
  attr.stack_size = stack_size_;
  attr.priority = priority_;

  handle_ = osThreadNew(func_, argument, &attr);

  return handle_ != nullptr;
}

} // namespace bd

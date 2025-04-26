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

#include <cmsis_os2.h>

namespace bd {
class Task {
  using UID = uint32_t;

public:
  Task(osThreadFunc_t func, const char *name, uint32_t stack_size,
       osPriority_t priority);

  Task(const Task &) = delete;
  Task &operator=(const Task &) = delete;
  Task(Task &&) = delete;
  Task &operator=(Task &&) = delete;

  inline constexpr osThreadId_t getHandle() { return handle_; }
  inline constexpr UID getUid() { return task_uid_; }

  bool run(void *argument = nullptr);

private:
  UID task_uid_;

  const char *name_;
  uint32_t stack_size_;
  osPriority_t priority_;

  osThreadId_t handle_;
  osThreadFunc_t func_;
};

} // namespace bd

#endif

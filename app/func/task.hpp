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
public:
  Task(osThreadFunc_t func, const char *name, uint32_t stack_size,
       osPriority_t priority);

  Task(const Task &) = delete;
  Task &operator=(const Task &) = delete;
  Task(Task &&) = delete;
  Task &operator=(Task &&) = delete;

  bool run(void *argument = nullptr);
  uint32_t notify(const Task &other);
  uint32_t wait(const Task &other);

  [[nodiscard]] inline constexpr osThreadId_t handle() const { return handle_; }
  [[nodiscard]] inline constexpr uint32_t uid() const { return uid_; }

private:
  const char *name_;
  uint32_t stack_size_;
  osPriority_t priority_;

  osThreadId_t handle_;
  osThreadFunc_t func_;

  uint32_t uid_;
};

} // namespace bd

#endif

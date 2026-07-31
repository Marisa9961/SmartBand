/**
 * @file core_messages.hpp
 * @brief Typed control-plane messages consumed by the core supervisor.
 */

#ifndef __TASKS_CORE_MESSAGES_HPP__
#define __TASKS_CORE_MESSAGES_HPP__

#include <cstdint>

#include <cmsis_os2.h>

#include "func/task.hpp"

namespace bd::task {
enum class CoreCommandType : uint8_t {
  TaskReady,
  TaskFault,
};

struct CoreCommand {
  CoreCommandType type;
  TaskId source;
  int32_t detail;
};

osStatus_t postCoreCommand(const CoreCommand &command, uint32_t timeout = 0);
osStatus_t reportTaskReady(TaskId source);
osStatus_t reportTaskFault(TaskId source, int32_t detail);
} // namespace bd::task

#endif

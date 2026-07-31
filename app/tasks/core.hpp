/**
 * @file   core.hpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  Public API for application task supervision.
 * @date   2025-03-21
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef BD_TASKS_CORE_HPP
#define BD_TASKS_CORE_HPP

#include <cstdint>

#include <cmsis_os2.h>

namespace bd::task {
enum class TaskId : uint8_t {
  Core,
  Gui,
  Sensor,
  Storage,
  Bluetooth,
  Count,
};

bool prepareRuntime();
osStatus_t reportTaskReady(TaskId source);
osStatus_t reportTaskFault(TaskId source);

[[noreturn]] void CoreTask(void *argument);
} // namespace bd::task

#endif

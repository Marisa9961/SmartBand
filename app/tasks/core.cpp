/**
 * @file   core.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  core impl
 * @date   2025-03-21
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "tasks/core.hpp"

#include "tasks/gui.hpp"
#include "tasks/sensor.hpp"
#include "tasks/storage.hpp"

namespace bd::task {
void CoreTask(void *argument) {
  gui.run();
  sensor.run();
  storage.run();

  osThreadFlagsWait(gui.getUid() | sensor.getUid() | storage.getUid(),
                    osFlagsWaitAll, osWaitForever);

  osThreadFlagsSet(gui.getHandle(), core.getUid());

  while (true) {
    osThreadSuspend(osThreadGetId());
  }
}

Task core{CoreTask, "Core", 128 * 4, osPriorityHigh};
} // namespace bd::task

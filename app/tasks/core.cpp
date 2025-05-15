/**
 * @file   core.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  core impl
 * @date   2025-03-21
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "tasks/core.hpp"

#include <cmsis_os2.h>

#include "tasks/bluetooth.hpp"
#include "tasks/gui.hpp"
#include "tasks/sensor.hpp"
#include "tasks/storage.hpp"

#include "components/bluetooth/kt6368a.h"

namespace bd::task {
void CoreTask(void *argument) {
  gui.run();
  sensor.run();
  storage.run();
  bluetooth.run();

  while (true) {
    osDelay(100);

    if (BLE_flag()) {
      core.notify(bluetooth);
    }
  }
}

Task core{CoreTask, "Core", 128 * 4, osPriorityNormal};
} // namespace bd::task

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

#include "components/bluetooth/ble.h"

namespace bd::task {
void CoreTask(void *argument) {
  gui.run();
  bluetooth.run();

  while (true) {
    if (BLE_flag()) {
      osThreadFlagsSet(bluetooth.getHandle(), core.flag());
    }

    osDelay(100);
  }
}

Task core{CoreTask, "Core", 128 * 4, osPriorityNormal};
} // namespace bd::task

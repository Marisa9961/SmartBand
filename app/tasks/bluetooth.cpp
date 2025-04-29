/**
 * @file   bluetooth.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  BLE impl
 * @date   2025-04-22
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "tasks/bluetooth.hpp"

#include "tasks/core.hpp"

#include "components/bluetooth/ble.h"

namespace bd::task {
void checkBluetoothSwitch() {
  if (osThreadFlagsGet() == core.getUid()) {
    if (HW_BLE) {
      BLE_enable();
    } else {
      BLE_disable();
    }
  }
}

enum BluetoothState {
  UNKNOWN,
  SET_TIME,
  SET_DATE,
  FIND_BAT,
  FIND_HR,
  FIND_STEP,
} bluetooth_state;

BluetoothState checkResult() {
  while (BLE_flag()) {
  }
  return bluetooth_state;
}

void BluetoothTask(void *parameter) {
  BLE_init();
  BLE_enable();

  while (true) {
    if (BLE_flag()) {
      // TODO parse command
    }

    checkBluetoothSwitch();
    osDelayUntil(5);
  }
}

Task bluetooth{BluetoothTask, "BlueTooth", 128 * 4, osPriorityHigh};
} // namespace bd::task

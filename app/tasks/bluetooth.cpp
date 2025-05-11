/**
 * @file   bluetooth.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  BLE impl
 * @date   2025-04-22
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "tasks/bluetooth.hpp"

#include "func/bluetooth_helper.hpp"

#include "tasks/core.hpp"

#include "components/battery/battery.h"

namespace bd::task {
void BluetoothTask(void *parameter) {
  BluetoothHelper helper{};

  while (true) {
    osThreadFlagsWait(core.flag(), osFlagsWaitAll, osWaitForever);

    switch (helper.parse()) {
    case BluetoothHelper::State::SET_TIME: {
      helper.handleTime();
      helper.print("TIME", helper.handleTime());
    } break;
    case BluetoothHelper::State::SET_DATE: {
      helper.handleDate();
      helper.print("DATE", helper.handleDate());
    } break;
    case BluetoothHelper::State::FIND_BAT: {
      helper.print("BATTERY", static_cast<int>(HW_BATTERY));
    } break;
    case BluetoothHelper::State::FIND_HR: {
      helper.print("HEARTRATE", 90);
    } break;
    case BluetoothHelper::State::FIND_STEP: {
      helper.print("STEPS", 8000);
    } break;
    case BluetoothHelper::State::ERROR: {
      helper.print("ERROR");
    } break;
    }

    helper.flush();
  }
}

Task bluetooth{BluetoothTask, "BlueTooth", 128 * 4, osPriorityAboveNormal};
} // namespace bd::task

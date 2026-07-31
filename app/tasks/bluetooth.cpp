/**
 * @file   bluetooth.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  BLE impl
 * @date   2025-04-22
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "tasks/bluetooth.hpp"

#include <cstdint>

#include <cmsis_os2.h>

#include "base/bootstrap.h"
#include "func/bluetooth_helper.hpp"
#include "func/task.hpp"
#include "tasks/core_messages.hpp"

#include "components/battery/battery.h"
#include "components/bluetooth/kt6368a.h"

namespace {
constexpr uint32_t bluetooth_rx_ready{1U << 0U};
osThreadId_t bluetooth_thread{nullptr};
}

extern "C" void BluetoothRxReadyCallback(void) {
  if (bluetooth_thread != nullptr) {
    osThreadFlagsSet(bluetooth_thread, bluetooth_rx_ready);
  }
}

namespace bd::task {
[[noreturn]] void BluetoothTask(void *parameter) {
  static_cast<void>(parameter);

  bluetooth_thread = osThreadGetId();
  BLE_init(BluetoothRxReadyCallback);

  BluetoothHelper helper{};

  if (reportTaskReady(TaskId::Bluetooth) != osOK) {
    app_fatal();
  }

  while (true) {
    const auto flags{osThreadFlagsWait(bluetooth_rx_ready, osFlagsWaitAny,
                                       osWaitForever)};
    if ((flags & osFlagsError) != 0U) {
      if (reportTaskFault(TaskId::Bluetooth,
                          static_cast<int32_t>(flags)) != osOK) {
        app_fatal();
      }
      continue;
    }

    switch (helper.parse()) {
    case BluetoothHelper::State::SET_TIME: {
      const bool success{helper.handleTime()};
      helper.print("TIME", success);
    } break;
    case BluetoothHelper::State::SET_DATE: {
      const bool success{helper.handleDate()};
      helper.print("DATE", success);
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
} // namespace bd::task

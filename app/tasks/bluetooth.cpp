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
#include "func/bluetooth_protocol.hpp"
#include "tasks/core.hpp"

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

  BluetoothProtocol protocol{};

  if (reportTaskReady(TaskId::Bluetooth) != osOK) {
    app_fatal();
  }

  while (true) {
    const auto flags{osThreadFlagsWait(bluetooth_rx_ready, osFlagsWaitAny,
                                       osWaitForever)};
    if ((flags & osFlagsError) != 0U) {
      if (reportTaskFault(TaskId::Bluetooth) != osOK) {
        app_fatal();
      }
      continue;
    }

    switch (protocol.receiveCommand()) {
    case BluetoothProtocol::Command::SetTime: {
      const bool success{protocol.applyTime()};
      protocol.prepareStatusResponse("TIME", success);
    } break;
    case BluetoothProtocol::Command::SetDate: {
      const bool success{protocol.applyDate()};
      protocol.prepareStatusResponse("DATE", success);
    } break;
    case BluetoothProtocol::Command::Invalid: {
      protocol.prepareResponse("ERROR");
    } break;
    }

    protocol.sendResponse();
  }
}
} // namespace bd::task

/**
 * @file   bluetooth.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  BLE impl
 * @date   2025-04-22
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "tasks/bluetooth.hpp"

#include <array>
#include <cstdio>
#include <cstring>

#include "tasks/core.hpp"

#include "components/bluetooth/ble.h"

namespace bd::task {
enum BluetoothState {
  ERROR,
  SET_TIME,
  SET_DATE,
  FIND_BAT,
  FIND_HR,
  FIND_STEP,
};

std::array<char, 16> rx_buffer{};
std::array<char, 16> tx_buffer{};

BluetoothState parseBluetoothData() {
  if (BLE_readNextChar() != '@') {
    return ERROR;
  }

  for (uint32_t i{}; i < rx_buffer.size(); i++) {
    auto c{BLE_readNextChar()};
    if (c == '@') {
      break;
    }

    rx_buffer[i] = c;
  }

  auto isSame{[](const char *str) {
    return std::strncmp(rx_buffer.data(), str, std::strlen(str)) == 0;
  }};

  BluetoothState bluetooth_state{};
  if (isSame("TIME:")) {
    bluetooth_state = SET_TIME;
  } else if (isSame("DATE:")) {
    bluetooth_state = SET_DATE;
  } else if (isSame("BATTERY?")) {
    bluetooth_state = FIND_BAT;
  } else if (isSame("HEARTRATE?")) {
    bluetooth_state = FIND_HR;
  } else if (isSame("STEPS?")) {
    bluetooth_state = FIND_STEP;
  }

  return bluetooth_state;
}

void printBluetoothData(const char *str) {
  std::sprintf(tx_buffer.data(), "#%s#", str);
}

void printBluetoothData(const char *str, bool state) {
  if (state) {
    std::sprintf(tx_buffer.data(), "#%s_OK#", str);
  } else {
    std::sprintf(tx_buffer.data(), "#RETRY");
  }
}

void printBluetoothData(const char *str, int dat) {
  std::sprintf(tx_buffer.data(), "#%s:%d#", str, dat);
}

void BluetoothTask(void *parameter) {
  BLE_init();

  while (true) {
    osThreadFlagsWait(core.flag(), osFlagsWaitAll, osWaitForever);

    rx_buffer.fill(0);
    tx_buffer.fill(0);

    switch (parseBluetoothData()) {
    case SET_TIME: {
      printBluetoothData("TIME", true);
    } break;
    case SET_DATE: {
      printBluetoothData("DATE", true);
    } break;
    case FIND_BAT: {
      printBluetoothData("BATTERY", 50);
    } break;
    case FIND_HR: {
      printBluetoothData("HEARTRATE", 90);
    } break;
    case FIND_STEP: {
      printBluetoothData("STEPS", 8000);
    } break;
    case ERROR: {
      printBluetoothData("ERROR");
    } break;
    }

    BLE_transmit(tx_buffer.data());
  }
}

Task bluetooth{BluetoothTask, "BlueTooth", 128 * 4, osPriorityAboveNormal};
} // namespace bd::task

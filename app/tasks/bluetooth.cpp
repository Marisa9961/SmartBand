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
  UNKNOWN,
  SET_TIME,
  SET_DATE,
  FIND_BAT,
  FIND_HR,
  FIND_STEP,
};

struct BluetoothCommand {
  std::array<char, 16> buffer{};
  uint32_t length{};
} command;

BluetoothState parseBluetoothData() {
  if (BLE_readNextChar() != '@') {
    return UNKNOWN;
  }

  command = {};

  for (uint32_t i{}; command.buffer[i] != '@'; i++) {
    command.buffer[i] = BLE_readNextChar();
    command.length++;
  }

  auto isSame{[](const char *str) {
    return std::strncmp(command.buffer.data(), str, std::strlen(str));
  }};

  BluetoothState bluetooth_state{};
  if (isSame("TIME:")) {
    bluetooth_state = SET_TIME;
  } else if (isSame("Date:")) {
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

void BluetoothTask(void *parameter) {
  BLE_init();

  auto send{[](const char *str, int dat) {
    std::array<char, 16> buf{};
    std::sprintf(buf.data(), "#%s:%d#", str, dat);
    BLE_transmit(buf.data());
  }};

  while (true) {
    osThreadFlagsWait(core.flag(), osFlagsWaitAll, osWaitForever);

    // TODO Finish parser function

    switch (parseBluetoothData()) {
    case UNKNOWN:
      break;
    case SET_TIME:
      break;
    case SET_DATE:
      break;
    case FIND_BAT:
      send("BATTERY", 50);
      break;
    case FIND_HR:
      send("HEARTRATE", 90);
      break;
    case FIND_STEP:
      send("STEPS", 8000);
      break;
    }

    if (!BLE_flag()) {
      BLE_ClearCallback();
    }
  }
}

Task bluetooth{BluetoothTask, "BlueTooth", 128 * 4, osPriorityAboveNormal};
} // namespace bd::task

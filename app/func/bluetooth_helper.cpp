/**
 * @file   bluetooth_helper.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  bluetooth function helper
 * @date   2025-05-11
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "bluetooth_helper.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

#include "components/battery/battery.h"
#include "components/bluetooth/kt6368a.h"
#include "components/rtc/rtc.h"

namespace bd::task {
void BluetoothHelper::clear() {
  rx.fill(0);
  tx.fill(0);
}

BluetoothHelper::State BluetoothHelper::parse() {
  clear();

  if (BLE_readNextChar() != '@') {
    return ERROR;
  }

  for (uint32_t i{}; i < rx.size(); i++) {
    auto c{BLE_readNextChar()};
    if (c == '@') {
      break;
    }

    rx[i] = c;
  }

  auto isSame{[&](const char *str) {
    return std::strncmp(rx.data(), str, std::strlen(str)) == 0;
  }};

  if (isSame("TIME:")) {
    return SET_TIME;
  } else if (isSame("DATE:")) {
    return SET_DATE;
  } else if (isSame("BATTERY?")) {
    return FIND_BAT;
  } else if (isSame("HEARTRATE?")) {
    return FIND_HR;
  } else if (isSame("STEPS?")) {
    return FIND_STEP;
  }

  return ERROR;
}

bool BluetoothHelper::handleTime() {
  uint8_t hour = 0, minute = 0, second = 0;
  if (std::sscanf(rx.data() + 5, "%2hhu:%2hhu:%2hhu", &hour, &minute,
                  &second) == 3) {
    RTC_setTime(hour, minute, second);
    return true;
  }

  return false;
}

bool BluetoothHelper::handleDate() {
  uint16_t year = 0;
  uint8_t month = 0, day = 0;
  if (std::sscanf(rx.data() + 5, "%4hu-%2hhu-%2hhu", &year, &month, &day) ==
      3) {
    RTC_setDate(year, month, day);
    return true;
  }

  return false;
}

void BluetoothHelper::print(const char *str) {
  std::sprintf(tx.data(), "#%s#", str);
}

void BluetoothHelper::print(const char *str, bool state) {
  if (state) {
    std::sprintf(tx.data(), "#%s_OK#", str);
  } else {
    std::sprintf(tx.data(), "#RETRY");
  }
}

void BluetoothHelper::print(const char *str, int dat) {
  std::sprintf(tx.data(), "#%s:%d#", str, dat);
}

void BluetoothHelper::transmit() { BLE_transmit(tx.data()); }

} // namespace bd::task

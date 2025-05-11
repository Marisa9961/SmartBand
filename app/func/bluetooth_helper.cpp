/**
 * @file   bluetooth_helper.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  bluetooth function helper
 * @date   2025-05-11
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "bluetooth_helper.hpp"

#include <cctype>
#include <cstdio>
#include <cstring>

#include "components/battery/battery.h"
#include "components/bluetooth/kt6368a.h"
#include "components/rtc/rtc.h"

namespace bd::task {
BluetoothHelper::BluetoothHelper() { BLE_init(); }

BluetoothHelper::State BluetoothHelper::parse() {
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
  const char *str = rx.data() + 5;

  uint8_t hour = 0;
  if (!std::isdigit(str[0]) || !std::isdigit(str[1]))
    return false;
  hour = (str[0] - '0') * 10 + (str[1] - '0');

  uint8_t minute = 0;
  if (!std::isdigit(str[3]) || !std::isdigit(str[4]))
    return false;
  minute = (str[3] - '0') * 10 + (str[4] - '0');

  uint8_t second = 0;
  if (!std::isdigit(str[6]) || !std::isdigit(str[7]))
    return false;
  second = (str[6] - '0') * 10 + (str[7] - '0');

  RTC_setTime(hour, minute, second);
  return true;
}

bool BluetoothHelper::handleDate() {
  const char *str = rx.data() + 5;

  uint16_t year = 0;
  for (int i = 0; i < 4; ++i) {
    if (!std::isdigit(str[i]))
      return false;
    year = year * 10 + (str[i] - '0');
  }

  uint8_t month = 0;
  if (!std::isdigit(str[5]) || !std::isdigit(str[6]))
    return false;
  month = (str[5] - '0') * 10 + (str[6] - '0');

  uint8_t day = 0;
  if (!std::isdigit(str[8]) || !std::isdigit(str[9]))
    return false;
  day = (str[8] - '0') * 10 + (str[9] - '0');

  RTC_setDate(year, month, day);
  return true;
}

void BluetoothHelper::print(const char *str) {
  std::sprintf(tx.data(), "#%s#", str);
}

void BluetoothHelper::print(const char *str, bool state) {
  if (state) {
    std::sprintf(tx.data(), "#%s_OK#", str);
  } else {
    std::sprintf(tx.data(), "#RETRY#");
  }
}

void BluetoothHelper::print(const char *str, int dat) {
  std::sprintf(tx.data(), "#%s:%d#", str, dat);
}

void BluetoothHelper::flush() {
  BLE_transmit(tx.data());
  rx.fill(0);
  tx.fill(0);
}

} // namespace bd::task

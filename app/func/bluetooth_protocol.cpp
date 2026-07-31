/**
 * @file   bluetooth_protocol.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  Parser and response builder for the Bluetooth text protocol.
 * @date   2025-05-11
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "func/bluetooth_protocol.hpp"

#include <cctype>
#include <cstddef>
#include <cstdio>
#include <string_view>

#include "components/bluetooth/kt6368a.h"
#include "components/rtc/rtc.h"

namespace bd {
namespace {
constexpr std::string_view time_prefix{"TIME:"};
constexpr std::string_view date_prefix{"DATE:"};
constexpr std::string_view time_frame_example{"TIME:00:00:00"};
constexpr std::string_view date_frame_example{"DATE:2000-01-01"};
constexpr uint16_t first_supported_year{2000U};
constexpr uint16_t last_supported_year{2099U};

bool parseTwoDigits(const char *text, uint8_t &value) {
  const auto first{static_cast<unsigned char>(text[0])};
  const auto second{static_cast<unsigned char>(text[1])};
  if (!std::isdigit(first) || !std::isdigit(second)) {
    return false;
  }

  value = static_cast<uint8_t>((text[0] - '0') * 10 + (text[1] - '0'));
  return true;
}
} // namespace

BluetoothProtocol::Command BluetoothProtocol::receiveCommand() {
  receive_buffer_.fill('\0');
  if (BLE_readNextChar() != '@') {
    return Command::Invalid;
  }

  std::size_t length{0U};
  bool complete{false};
  while (length + 1U < receive_buffer_.size()) {
    const char character{BLE_readNextChar()};
    if (character == '@') {
      complete = true;
      break;
    }
    if (character == '\0') {
      break;
    }
    receive_buffer_[length++] = character;
  }

  if (!complete) {
    return Command::Invalid;
  }

  const std::string_view frame{receive_buffer_.data(), length};
  if (frame.size() == time_frame_example.size() &&
      frame.starts_with(time_prefix)) {
    return Command::SetTime;
  }
  if (frame.size() == date_frame_example.size() &&
      frame.starts_with(date_prefix)) {
    return Command::SetDate;
  }

  return Command::Invalid;
}

bool BluetoothProtocol::applyTime() {
  const char *text{receive_buffer_.data() + time_prefix.size()};
  if (text[2] != ':' || text[5] != ':') {
    return false;
  }

  uint8_t hour{0U};
  uint8_t minute{0U};
  uint8_t second{0U};
  if (!parseTwoDigits(text, hour) ||
      !parseTwoDigits(text + 3U, minute) ||
      !parseTwoDigits(text + 6U, second) || hour > 23U || minute > 59U ||
      second > 59U) {
    return false;
  }

  RTC_setTime(hour, minute, second);
  return true;
}

bool BluetoothProtocol::applyDate() {
  const char *text{receive_buffer_.data() + date_prefix.size()};
  if (text[4] != '-' || text[7] != '-') {
    return false;
  }

  uint16_t year{0U};
  for (std::size_t index{0U}; index < 4U; ++index) {
    const auto character{static_cast<unsigned char>(text[index])};
    if (!std::isdigit(character)) {
      return false;
    }
    year = static_cast<uint16_t>(year * 10U + (text[index] - '0'));
  }

  uint8_t month{0U};
  uint8_t day{0U};
  if (!parseTwoDigits(text + 5U, month) ||
      !parseTwoDigits(text + 8U, day) || year < first_supported_year ||
      year > last_supported_year ||
      month == 0U || month > 12U || day == 0U || day > 31U) {
    return false;
  }

  RTC_setDate(year, month, day);
  return true;
}

void BluetoothProtocol::prepareResponse(const char *message) {
  std::snprintf(transmit_buffer_.data(), transmit_buffer_.size(), "#%s#",
                message);
}

void BluetoothProtocol::prepareStatusResponse(const char *operation,
                                              bool success) {
  if (success) {
    std::snprintf(transmit_buffer_.data(), transmit_buffer_.size(),
                  "#%s_OK#", operation);
  } else {
    prepareResponse("RETRY");
  }
}

void BluetoothProtocol::sendResponse() {
  BLE_transmit(transmit_buffer_.data());
}

} // namespace bd

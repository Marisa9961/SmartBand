/**
 * @file   bluetooth_protocol.hpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  Parser and response builder for the Bluetooth text protocol.
 * @date   2025-05-11
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef BD_FUNC_BLUETOOTH_PROTOCOL_HPP
#define BD_FUNC_BLUETOOTH_PROTOCOL_HPP

#include <array>
#include <cstddef>
#include <cstdint>

namespace bd {
class BluetoothProtocol {
public:
  enum class Command : uint8_t {
    Invalid,
    SetTime,
    SetDate,
  };

  Command receiveCommand();

  bool applyTime();
  bool applyDate();

  void prepareResponse(const char *message);
  void prepareStatusResponse(const char *operation, bool success);
  void sendResponse();

private:
  static constexpr std::size_t buffer_capacity{24U};

  std::array<char, buffer_capacity> receive_buffer_{};
  std::array<char, buffer_capacity> transmit_buffer_{};
};

} // namespace bd

#endif

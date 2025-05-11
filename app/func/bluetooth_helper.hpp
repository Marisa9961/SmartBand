/**
 * @file   bluetooth_helper.hpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  bluetooth function helper
 * @date   2025-05-11
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef __BLUETOOTH_HELPER_HPP__
#define __BLUETOOTH_HELPER_HPP__

#include <array>

namespace bd::task {
class BluetoothHelper {
public:
  enum State {
    ERROR,
    SET_TIME,
    SET_DATE,
    FIND_BAT,
    FIND_HR,
    FIND_STEP,
  };

  void clear();
  State parse();

  bool handleTime();
  bool handleDate();

  void print(const char *str);
  void print(const char *str, bool state);
  void print(const char *str, int dat);

  void transmit();

  std::array<char, 16> rx{};
  std::array<char, 16> tx{};
};

} // namespace bd::task

#endif

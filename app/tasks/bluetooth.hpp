/**
 * @file   bluetooth.hpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  Bluetooth task entry point.
 * @date   2025-04-22
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef BD_TASKS_BLUETOOTH_HPP
#define BD_TASKS_BLUETOOTH_HPP

namespace bd::task {
[[noreturn]] void BluetoothTask(void *argument);
} // namespace bd::task

#endif

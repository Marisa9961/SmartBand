/**
 * @file   gui.hpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  gui define
 * @date   2025-03-21
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef __TASKS_GUI_HPP__
#define __TASKS_GUI_HPP__

#include <cstdint>

namespace bd::task {
enum class GuiSignal : uint32_t {
  DisplayOff = 1U << 0U,
  DisplayOn = 1U << 1U,
};

inline constexpr uint32_t gui_signal_mask{
    static_cast<uint32_t>(GuiSignal::DisplayOff) |
    static_cast<uint32_t>(GuiSignal::DisplayOn)};

[[noreturn]] void GuiTask(void *argument);
} // namespace bd::task

#endif

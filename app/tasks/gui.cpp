/**
 * @file   gui.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  gui impl
 * @date   2025-03-21
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "tasks/gui.hpp"

#include <lvgl/lvgl.h>
#include <porting/lv_port_disp.h>
#include <porting/lv_port_indev.h>

#include "base/bootstrap.h"
#include "func/task.hpp"
#include "tasks/core_messages.hpp"

#include "components/lcd/st7789.h"

#include "ui/ui.h"

namespace bd::task {
namespace {
constexpr uint32_t gui_period_ticks{5U};

void applyPendingSignals() {
  const auto flags{
      osThreadFlagsWait(gui_signal_mask, osFlagsWaitAny, 0U)};
  if ((flags & osFlagsError) != 0U) {
    return;
  }

  if ((flags & static_cast<uint32_t>(GuiSignal::DisplayOff)) != 0U) {
    LCD_setLight(0U);
  }
  if ((flags & static_cast<uint32_t>(GuiSignal::DisplayOn)) != 0U) {
    LCD_setLight(static_cast<uint8_t>(HW_LEVEL));
  }
}
} // namespace

[[noreturn]] void GuiTask(void *argument) {
  static_cast<void>(argument);

  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();

  ui_init();

  if (reportTaskReady(TaskId::Gui) != osOK) {
    app_fatal();
  }

  bool idle_reported{false};
  uint32_t next_wake{osKernelGetTickCount()};

  while (true) {
    applyPendingSignals();
    lv_task_handler();

    const bool display_idle{
        lv_disp_get_inactive_time(nullptr) >= HW_SCREEN_OFF_TIME};
    if (display_idle != idle_reported) {
      const CoreCommandType command_type{
          display_idle ? CoreCommandType::DisplayIdle
                       : CoreCommandType::DisplayActive};
      if (postCoreCommand(CoreCommand{command_type, TaskId::Gui, 0}) == osOK) {
        idle_reported = display_idle;
      }
    }

    next_wake += gui_period_ticks;
    if (osDelayUntil(next_wake) != osOK) {
      next_wake = osKernelGetTickCount();
    }
  }
}
} // namespace bd::task

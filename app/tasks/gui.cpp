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

void updateDisplayIdleState(bool should_idle, bool &is_idle) {
  if (should_idle == is_idle) {
    return;
  }

  LCD_setLight(should_idle ? 0U : static_cast<uint8_t>(HW_LEVEL));
  is_idle = should_idle;
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

  bool display_idle{false};
  uint32_t next_wake{osKernelGetTickCount()};

  while (true) {
    lv_task_handler();

    const bool should_display_idle{
        lv_disp_get_inactive_time(nullptr) >= HW_SCREEN_OFF_TIME};
    updateDisplayIdleState(should_display_idle, display_idle);

    next_wake += gui_period_ticks;
    if (osDelayUntil(next_wake) != osOK) {
      next_wake = osKernelGetTickCount();
    }
  }
}
} // namespace bd::task

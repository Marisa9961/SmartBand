/**
 * @file   gui.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  gui impl
 * @date   2025-03-21
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "tasks/gui.hpp"

#include <cstdint>

#include <lvgl/lvgl.h>
#include <porting/lv_port_disp.h>
#include <porting/lv_port_indev.h>

#include "base/bootstrap.h"
#include "func/config/config_manager.hpp"
#include "tasks/core.hpp"

#include "components/lcd/st7789.h"

#include "func/ui_binding/display_settings_binding.hpp"
#include "ui/ui.h"

namespace bd::task {
namespace {
constexpr uint32_t gui_period_ticks{5U};
constexpr uint32_t config_load_timeout_ticks{3000U};

void updateDisplayIdleState(bool should_idle, bool &is_idle,
                            uint8_t brightness_percent) {
  if (should_idle == is_idle) {
    return;
  }

  LCD_setLight(should_idle ? 0U : brightness_percent);
  is_idle = should_idle;
}
} // namespace

[[noreturn]] void GuiTask(void *argument) {
  static_cast<void>(argument);

  config::ConfigManager configuration{};
  if (!configuration.initialize(config_load_timeout_ticks)) {
    app_fatal();
  }

  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();

  ui_init();

  ui::DisplaySettingsBinding display_settings{configuration};
  if (!display_settings.initialize()) {
    app_fatal();
  }
  LCD_setLight(configuration.current().display.brightness_percent);

  if (reportTaskReady(TaskId::Gui) != osOK) {
    app_fatal();
  }

  bool display_idle{false};
  uint32_t next_wake{osKernelGetTickCount()};

  while (true) {
    lv_task_handler();

    configuration.process();
    if (configuration.takeConfigurationUpdate()) {
      display_settings.apply();
      if (!display_idle) {
        LCD_setLight(configuration.current().display.brightness_percent);
      }
    }

    const bool should_display_idle{
        lv_disp_get_inactive_time(nullptr) >=
        configuration.current().display.screen_timeout_ms};
    updateDisplayIdleState(
        should_display_idle, display_idle,
        configuration.current().display.brightness_percent);

    next_wake += gui_period_ticks;
    if (osDelayUntil(next_wake) != osOK) {
      next_wake = osKernelGetTickCount();
    }
  }
}
} // namespace bd::task

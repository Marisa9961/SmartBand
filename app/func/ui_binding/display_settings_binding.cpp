/**
 * @file display_settings_binding.cpp
 * @brief Hand-written LVGL adapter for persistent display settings.
 */

#include "func/ui_binding/display_settings_binding.hpp"

#include <cstdint>

#include "components/lcd/st7789.h"
#include "func/config/device_config.hpp"
#include "ui/ui.h"

namespace bd::ui {
namespace {
uint32_t timeoutIndex(uint32_t timeout_ms) {
  for (uint32_t index{0U}; index < config::screen_timeout_options_ms.size();
       ++index) {
    if (config::screen_timeout_options_ms[index] == timeout_ms) {
      return index;
    }
  }
  return 1U;
}

uint32_t timeoutFromIndex(uint32_t index) {
  return index < config::screen_timeout_options_ms.size()
             ? config::screen_timeout_options_ms[index]
             : config::default_screen_timeout_ms;
}

DisplaySettingsBinding *bindingFrom(lv_event_t *event) {
  return static_cast<DisplaySettingsBinding *>(
      lv_event_get_user_data(event));
}
} // namespace

bool DisplaySettingsBinding::initialize() {
  if (ui_NotificationPanel_SliderLight == nullptr ||
      ui_SettingsLockTime_Dropdown == nullptr) {
    return false;
  }

  lv_slider_set_range(ui_NotificationPanel_SliderLight,
                      config::minimum_brightness_percent,
                      config::maximum_brightness_percent);
  apply();

  return lv_obj_add_event_cb(ui_NotificationPanel_SliderLight,
                             previewBrightness, LV_EVENT_VALUE_CHANGED,
                             this) != nullptr &&
         lv_obj_add_event_cb(ui_NotificationPanel_SliderLight,
                             commitBrightness, LV_EVENT_RELEASED,
                             this) != nullptr &&
         lv_obj_add_event_cb(ui_SettingsLockTime_Dropdown, changeTimeout,
                             LV_EVENT_VALUE_CHANGED, this) != nullptr;
}

void DisplaySettingsBinding::apply() {
  applying_ = true;
  const config::DisplaySettings &display{config_.current().display};
  lv_slider_set_value(ui_NotificationPanel_SliderLight,
                      display.brightness_percent, LV_ANIM_OFF);
  lv_dropdown_set_selected(ui_SettingsLockTime_Dropdown,
                           timeoutIndex(display.screen_timeout_ms));
  applying_ = false;
}

void DisplaySettingsBinding::previewBrightness(lv_event_t *event) {
  auto *binding{bindingFrom(event)};
  if (binding == nullptr || binding->applying_) {
    return;
  }

  const auto brightness{static_cast<uint8_t>(
      lv_slider_get_value(ui_NotificationPanel_SliderLight))};
  LCD_setLight(brightness);
}

void DisplaySettingsBinding::commitBrightness(lv_event_t *event) {
  auto *binding{bindingFrom(event)};
  if (binding == nullptr || binding->applying_) {
    return;
  }

  const auto brightness{static_cast<uint8_t>(
      lv_slider_get_value(ui_NotificationPanel_SliderLight))};
  static_cast<void>(binding->config_.setBrightnessPercent(brightness));
}

void DisplaySettingsBinding::changeTimeout(lv_event_t *event) {
  auto *binding{bindingFrom(event)};
  if (binding == nullptr || binding->applying_) {
    return;
  }

  const uint32_t timeout_ms{
      timeoutFromIndex(lv_dropdown_get_selected(
          ui_SettingsLockTime_Dropdown))};
  static_cast<void>(binding->config_.setScreenTimeoutMs(timeout_ms));
}
} // namespace bd::ui

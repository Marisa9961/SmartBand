/**
 * @file display_settings_binding.hpp
 * @brief Hand-written LVGL adapter for persistent display settings.
 */

#ifndef BD_UI_DISPLAY_SETTINGS_HPP
#define BD_UI_DISPLAY_SETTINGS_HPP

#include <lvgl/lvgl.h>

#include "func/config/config_manager.hpp"

namespace bd::ui {
class DisplaySettingsBinding {
public:
  explicit DisplaySettingsBinding(config::ConfigManager &config)
      : config_{config} {}

  bool initialize();
  void apply();

private:
  static void previewBrightness(lv_event_t *event);
  static void commitBrightness(lv_event_t *event);
  static void changeTimeout(lv_event_t *event);

  config::ConfigManager &config_;
  bool applying_{false};
};
} // namespace bd::ui

#endif

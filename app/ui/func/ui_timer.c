/**
 * @file   ui_timer.c
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  user define ui timer
 * @date   2025-05-11
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "ui_timer.h"

#include "ui_timer_helper.h"

#include "ui/ui.h"

typedef void (*PageUpdateCallback)(void);
static PageUpdateCallback callback;

static lv_timer_t *timer;

static void timer_callback(lv_timer_t *timer) {
  static lv_obj_t *current_screen;
  lv_obj_t *screen = lv_screen_active();

  if (current_screen != screen) {
    if (screen == nullptr) {
    } else if (screen == ui_Calendar) {
      callback = ui_Calendar_update;
    } else if (screen == ui_Director) {
      callback = ui_Director_update;
    } else if (screen == ui_HeartBeat) {
      callback = ui_HeartBeat_update;
    } else if (screen == ui_Home) {
      callback = ui_Home_update;
    } else if (screen == ui_NotificationPanel) {
      callback = ui_NotificationPanel_update;
    } else if (screen == ui_Thermometer) {
      callback = ui_Thermometer_update;
    }

    current_screen = screen;

  } else {
    callback();
  }
}

void ui_timer_init() {
  callback = ui_Home_update;
  timer = lv_timer_create(timer_callback, 1000, nullptr);
}

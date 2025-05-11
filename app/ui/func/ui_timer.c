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
    if (screen == ui_Home) {
      current_screen = screen;
      callback = ui_Home_update;
    }

  } else {
    callback();
  }
}

void ui_timer_init() {
  callback = ui_Home_update;
  timer = lv_timer_create(timer_callback, 100, nullptr);
}

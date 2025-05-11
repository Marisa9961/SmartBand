/**
 * @file   ui_timer_helper.c
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  user define ui timer helper
 * @date   2025-05-11
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "ui_timer_helper.h"

#include "components/battery/battery.h"
#include "components/rtc/rtc.h"

#include "ui/ui.h"

void ui_Calendar_update() {
  lv_calendar_set_today_date(ui_Calendar_CalendarInstance, 2025, 5, 1);
  lv_calendar_set_showed_date(ui_Calendar_CalendarInstance, 2025, 5);
}

void ui_Director_update() {
  lv_label_set_text(ui_Director_LabelHeight, "0m");
  lv_label_set_text(ui_Director_LabelCompass, "0°");
}

void ui_HeartBeat_update() {
  lv_label_set_text(ui_HeartBeat_LabelHeartbeatNum, "82");
}

void ui_Home_update() {
  lv_label_set_text(ui_Home_Date, "05-01");
  lv_label_set_text(ui_Home_Mouth, "Thu");

  lv_label_set_text(ui_Home_TimeMinute, "12");
  lv_label_set_text(ui_Home_TimeSecond, "34");

  lv_label_set_text(ui_Home_LabelApplication, "Today's Step: 103");

  lv_label_set_text(ui_Home_LabelWeather, "Sunny\n28/15 °C");

  lv_label_set_text(ui_Home_LabelHeart, "75 BPM");

  lv_label_set_text(ui_Home_LabelBattery, "78%");
}

void ui_NotificationPanel_update() {
  // TODO too many, maybe later...
}

void ui_Thermometer_update() {
  lv_label_set_text(ui_Thermometer_LabelTemp, " 28 °C");
  lv_label_set_text(ui_Thermometer_LabelHumi, "90 %");
}

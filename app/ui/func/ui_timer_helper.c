/**
 * @file   ui_timer_helper.c
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  user define ui timer helper
 * @date   2025-05-11
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "ui_timer_helper.h"

#include <stdio.h>

#include "components/battery/battery.h"
#include "components/direct/lsm303.h"
#include "components/rtc/rtc.h"
#include "components/thermometer/aht21.h"

#include "ui/ui.h"

static char ui_timer_buffer[16];

const char *week_str[] = {"", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

void ui_Calendar_update() {
  RTC_freshDate();
  lv_calendar_set_today_date(ui_Calendar_CalendarInstance, HW_YEAR, HW_MONTH,
                             HW_DAY);
  lv_calendar_set_showed_date(ui_Calendar_CalendarInstance, HW_YEAR, HW_MONTH);
}

void ui_Director_update() {
  sprintf(ui_timer_buffer, "%d°", (int)HW_DIRECT);
  lv_label_set_text(ui_Director_LabelCompass, ui_timer_buffer);
}

void ui_HeartBeat_update() {
  // lv_label_set_text(ui_HeartBeat_LabelHeartbeatNum, "82");
}

void ui_Home_update() {
  RTC_freshDate();
  sprintf(ui_timer_buffer, "%d-%d", (int)HW_MONTH, (int)HW_DAY);
  lv_label_set_text(ui_Home_Date, ui_timer_buffer);
  lv_label_set_text(ui_Home_Weekday, week_str[(int)HW_WEEKDAY]);

  RTC_freshTime();
  sprintf(ui_timer_buffer, "%d", (int)HW_HOUR);
  lv_label_set_text(ui_Home_TimeHour, ui_timer_buffer);
  sprintf(ui_timer_buffer, "%d", (int)HW_MINUTE);
  lv_label_set_text(ui_Home_TimeMinute, ui_timer_buffer);

  // lv_label_set_text(ui_Home_LabelApplication, "Today's Step: 103");

  // lv_label_set_text(ui_Home_LabelWeather, "Sunny\n28/15 °C");

  // lv_label_set_text(ui_Home_LabelHeart, "75 BPM");

  // lv_label_set_text(ui_Home_LabelBattery, "78%");
}

void ui_NotificationPanel_update() {
  // TODO too many, maybe later...
}

void ui_Thermometer_update() {
  int temp_int = HW_TEMP / 10;
  int temp_frac = HW_TEMP % 10;

  int humi_int = HW_HUMI / 10;
  int humi_frac = HW_HUMI % 10;

  sprintf(ui_timer_buffer, "%d.%d°C", temp_int, temp_frac);
  lv_label_set_text(ui_Thermometer_LabelTemp, ui_timer_buffer);

  sprintf(ui_timer_buffer, "%d.%d%%", humi_int, humi_frac);
  lv_label_set_text(ui_Thermometer_LabelHumi, ui_timer_buffer);
}

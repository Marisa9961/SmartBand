/**
 * @file   rtc.c
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  rtc impl
 * @date   2025-05-10
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "rtc.h"

extern RTC_HandleTypeDef hrtc;
#define RTC_HANDLE &hrtc

uint32_t HW_HOUR;
uint32_t HW_MINUTE;
uint32_t HW_SECOND;

void RTC_freshTime() {
  RTC_TimeTypeDef time = {};
  HAL_RTC_GetTime(RTC_HANDLE, &time, RTC_FORMAT_BCD);

  HW_HOUR = time.Hours;
  HW_MINUTE = time.Minutes;
  HW_SECOND = time.Seconds;
}

void RTC_setTime(uint8_t hour, uint8_t minute, uint8_t second) {
  RTC_TimeTypeDef time = {};
  time.Hours = hour;
  time.Minutes = minute;
  time.Seconds = second;
  HAL_RTC_SetTime(RTC_HANDLE, &time, RTC_FORMAT_BIN);
}

uint32_t HW_YEAR;
uint32_t HW_MONTH;
uint32_t HW_DAY;

void RTC_freshDate() {
  RTC_DateTypeDef date = {};
  HAL_RTC_GetDate(RTC_HANDLE, &date, RTC_FORMAT_BCD);

  HW_YEAR = date.Year + 2000;
  HW_MONTH = date.Month;
  HW_DAY = date.Date;
}

void RTC_setDate(uint16_t year, uint8_t month, uint8_t day) {
  RTC_DateTypeDef date = {};
  date.Year = year - 2000;
  date.Month = month;
  date.Date = day;

  { // Zellers Kongruenz
    if (month < 3) {
      month += 12;
      year -= 1;
    }

    uint16_t K = year % 100;
    uint16_t J = year / 100;

    date.WeekDay =
        (day + (13 * (month + 1)) / 5 + K + K / 4 + J / 4 + 5 * J) % 7 + 6;
    date.WeekDay = (date.WeekDay % 7) + 1; // 1=Monday, ..., 7=Sunday
  }

  HAL_RTC_SetDate(RTC_HANDLE, &date, RTC_FORMAT_BIN);
}

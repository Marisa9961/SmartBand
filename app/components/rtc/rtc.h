/**
 * @file   rtc.h
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  rtc define
 * @date   2025-05-10
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef __RTC_H__
#define __RTC_H__

#include "components/prf.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t HW_HOUR;
extern uint32_t HW_MINUTE;
extern uint32_t HW_SECOND;

void RTC_freshTime();
void RTC_setTime(uint8_t hour, uint8_t minute, uint8_t second);

extern uint32_t HW_YEAR;
extern uint32_t HW_MONTH;
extern uint32_t HW_DAY;

void RTC_freshDate();
void RTC_setDate(uint16_t year, uint8_t month, uint8_t day);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

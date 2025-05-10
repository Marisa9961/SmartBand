/**
 * @file   battery.h
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  bat define
 * @date   2025-05-10
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef __BATTERY_H__
#define __BATTERY_H__

#include "components/prf.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t HW_BATTERY;

void BAT_fresh();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

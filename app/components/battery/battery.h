/**
 * @file   battery.h
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  bat define
 * @date   2025-05-10
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef BD_COMPONENTS_BATTERY_H
#define BD_COMPONENTS_BATTERY_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t HW_BATTERY;

void BAT_enable(void);
void BAT_disable(void);
void BAT_fresh(void);

bool BAT_charging(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

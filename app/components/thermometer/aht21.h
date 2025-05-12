/**
 * @file   aht21.h
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  AHT21 Temperature Sensor
 * @date   2025-04-03
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef __AHT21_H__
#define __AHT21_H__

#include "components/prf.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t HW_TEMP;
extern uint32_t HW_HUMI;

void THER_init();
void THER_fresh();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

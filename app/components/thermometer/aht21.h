/**
 * @file   aht21.h
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  AHT21 Temperature Sensor
 * @date   2025-04-03
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef BD_COMPONENTS_AHT21_H
#define BD_COMPONENTS_AHT21_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t HW_TEMP;
extern uint32_t HW_HUMI;

void THER_init(void);
void THER_fresh(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

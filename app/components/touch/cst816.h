/**
 * @file   cst816.h
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  CST816 Driver
 * @date   2025-04-03
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef __CST816_H__
#define __CST816_H__

#include "components/prf.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint16_t x_pos;
  uint16_t y_pos;
} TOUCH_Info;

extern TOUCH_Info touch_info;

void TOUCH_init();
void TOUCH_freshXY();
uint8_t TOUCH_getFingerNum();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

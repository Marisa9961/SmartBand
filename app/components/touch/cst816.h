/**
 * @file   cst816.h
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  CST816 Driver
 * @date   2025-04-03
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef BD_COMPONENTS_CST816_H
#define BD_COMPONENTS_CST816_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint16_t x_pos;
  uint16_t y_pos;
} TOUCH_Info;

extern TOUCH_Info touch_info;

void TOUCH_init(void);
void TOUCH_freshXY(void);
uint8_t TOUCH_getFingerNum(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

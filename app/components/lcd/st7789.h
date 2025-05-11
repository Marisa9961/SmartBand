/**
 * @file   st7789.h
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  LCD Driver
 * @date   2025-04-03
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef __ST7789_H__
#define __ST7789_H__

#include "components/prf.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t HW_ScreenOffTime;
extern uint32_t HW_Light;

void LCD_init(void);
void LCD_draw(uint16_t xbegin, uint16_t ybegin, uint16_t xend, uint16_t yend,
              uint8_t pic[]);

void LCD_setLight(uint8_t level);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

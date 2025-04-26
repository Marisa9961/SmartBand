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

void LCD_init(void);
void LCD_drawPicture(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                     uint8_t pic[]);

#endif

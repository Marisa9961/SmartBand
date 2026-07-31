/**
 * @file   st7789.h
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  LCD Driver
 * @date   2025-04-03
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef BD_COMPONENTS_ST7789_H
#define BD_COMPONENTS_ST7789_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void LCD_init(void);
void LCD_draw(uint16_t xbegin, uint16_t ybegin, uint16_t xend, uint16_t yend,
              uint8_t pic[]);

/** Sets backlight brightness in the inclusive range 0..100 percent. */
void LCD_setLight(uint8_t level);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

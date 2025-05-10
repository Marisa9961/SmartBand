/**
 * @file   st7789.c
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  LCD Driver
 * @date   2025-04-03
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "st7789.h"

extern SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef *const LCD_SPI = &hspi1;

extern TIM_HandleTypeDef htim1;
TIM_HandleTypeDef *const LCD_TIM = &htim1;

// IMPORTANT!
#define Y_OFFSET 20

#define SPI_transmit(spi_handler, p_data, size)                                \
  HAL_SPI_Transmit(spi_handler, (uint8_t *)p_data, size, HAL_MAX_DELAY)

#define LCD_resetCS()                                                          \
  do {                                                                         \
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);           \
  } while (0)

#define LCD_setCS()                                                            \
  do {                                                                         \
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);             \
  } while (0)

#define LCD_resetDC()                                                          \
  do {                                                                         \
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);           \
  } while (0)

#define LCD_setDC()                                                            \
  do {                                                                         \
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);             \
  } while (0)

// For single data
static void LCD_writeDAT(const uint8_t data) {
  LCD_resetCS();
  SPI_transmit(LCD_SPI, &data, sizeof(data));
  LCD_setCS();
}

// For multi data
static void LCD_writeDATS(const uint8_t data[], uint8_t num) {
  LCD_resetCS();
  SPI_transmit(LCD_SPI, data, num);
  LCD_setDC();
}

// For single register
static void LCD_writeREG(const uint8_t data) {
  LCD_resetDC();
  LCD_writeDAT(data);
  LCD_setDC();
}

static void LCD_setAddress(uint16_t xbegin, uint16_t ybegin, uint16_t xend,
                           uint16_t yend) {
  uint8_t x[4] = {xbegin >> 8U, xbegin, xend >> 8U, xend};
  uint8_t y[4] = {ybegin >> 8U, ybegin, yend >> 8U, yend};

  // COL_ADDRESS
  LCD_writeREG(0x2A);
  LCD_writeDATS(x, 4);
  // ROW_ADDRESS
  LCD_writeREG(0x2B);
  LCD_writeDATS(y, 4);
  // MEM_WRITE
  LCD_writeREG(0x2C);
}

void LCD_init(void) {
  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
  delay(1);

  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
  delay(120);

  LCD_writeREG(0x11); // SLEEP_OFF
  LCD_writeREG(0x11); // SLEEP_OFF
  delay(120);

  // MEM_CTRL & MEM_CTRL_DAT
  LCD_writeREG(0x36);
  LCD_writeDAT(0x00);

  // PIXEL_FORMAT & PIXEL_FORMAT_DAT
  LCD_writeREG(0x3A);
  LCD_writeDAT(0x05);

  // PWR_CTRL
  LCD_writeREG(0xD0);
  LCD_writeDAT(0xA4);
  LCD_writeDAT(0xA1);

  // P_GAM_CTRL
  LCD_writeREG(0xE0);
  LCD_writeDAT(0xF0);
  LCD_writeDAT(0x00);
  LCD_writeDAT(0x04);
  LCD_writeDAT(0x04);
  LCD_writeDAT(0x04);
  LCD_writeDAT(0x05);
  LCD_writeDAT(0x29);
  LCD_writeDAT(0x33);
  LCD_writeDAT(0x3E);
  LCD_writeDAT(0x38);
  LCD_writeDAT(0x12);
  LCD_writeDAT(0x12);
  LCD_writeDAT(0x28);
  LCD_writeDAT(0x30);

  // N_GAM_CTRL
  LCD_writeREG(0xE1);
  LCD_writeDAT(0xF0);
  LCD_writeDAT(0x07);
  LCD_writeDAT(0x0A);
  LCD_writeDAT(0x0D);
  LCD_writeDAT(0x0B);
  LCD_writeDAT(0x07);
  LCD_writeDAT(0x28);
  LCD_writeDAT(0x33);
  LCD_writeDAT(0x3E);
  LCD_writeDAT(0x36);
  LCD_writeDAT(0x14);
  LCD_writeDAT(0x14);
  LCD_writeDAT(0x29);
  LCD_writeDAT(0x32);

  LCD_writeREG(0x21); // INVS_ON

  LCD_writeREG(0x11); // SLEEP_OFF
  delay(120);

  LCD_writeREG(0x29); // PLAY_ON

  HAL_TIM_PWM_Start(LCD_TIM, TIM_CHANNEL_2);
}

void LCD_draw(uint16_t xbegin, uint16_t ybegin, uint16_t xend, uint16_t yend,
              uint8_t pic[]) {
  LCD_setAddress(xbegin, ybegin + Y_OFFSET, xend, yend + Y_OFFSET);
  LCD_resetCS();
  HAL_SPI_Transmit(LCD_SPI, pic, (xend - xbegin + 1) * (yend - ybegin + 1) * 2,
                   HAL_MAX_DELAY);
  LCD_setCS();
}

void LCD_setLight(uint8_t level) {
  HAL_TIM_PWM_Stop(LCD_TIM, TIM_CHANNEL_2);
  __HAL_TIM_SET_COMPARE(LCD_TIM, TIM_CHANNEL_2,
                        level == 0 ? (0) : (level * 3 - 1));
  HAL_TIM_PWM_Start(LCD_TIM, TIM_CHANNEL_2);
};

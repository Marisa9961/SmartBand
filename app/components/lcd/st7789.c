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

/**
 * @brief  写入GPIO
 */
#define writeGPIO(port, pin, value) HAL_GPIO_WritePin(port, pin, value)

/**
 * @brief  传输SPI
 */
#define tranSPI(spi_handler, p_data, size)                                     \
  HAL_SPI_Transmit_DMA(spi_handler, (uint8_t *)p_data, size)

#define LCD_setCS_()                                                           \
  do {                                                                         \
    writeGPIO(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);                     \
  } while (0)

#define LCD_resetCS_()                                                         \
  do {                                                                         \
    writeGPIO(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);                   \
  } while (0)

#define LCD_setDC_()                                                           \
  do {                                                                         \
    writeGPIO(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);                     \
  } while (0)

#define LCD_resetDC_()                                                         \
  do {                                                                         \
    writeGPIO(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);                   \
  } while (0)

// For single data
static void LCD_writeDAT_(const uint8_t data) {
  LCD_resetCS_();
  tranSPI(LCD_SPI, &data, sizeof(data));
  LCD_setCS_();
}

// For multi data
static void LCD_writeDATS_(const uint8_t data[], uint8_t num) {
  LCD_resetCS_();
  tranSPI(LCD_SPI, data, num);
  LCD_setCS_();
}

// For single register
static void LCD_writeREG_(const uint8_t data) {
  LCD_resetDC_();
  LCD_writeDAT_(data);
  LCD_setDC_();
}

static void LCD_setAddress_(uint16_t xbegin, uint16_t ybegin, uint16_t xend,
                            uint16_t yend) {
  uint8_t x[4] = {xbegin >> 8U, xbegin, xend >> 8U, xend};
  uint8_t y[4] = {ybegin >> 8U, ybegin, yend >> 8U, yend};

  // COL_ADDRESS
  LCD_writeREG_(0x2A);
  LCD_writeDATS_(x, 4);
  // ROW_ADDRESS
  LCD_writeREG_(0x2B);
  LCD_writeDATS_(y, 4);
  // MEM_WRITE
  LCD_writeREG_(0x2C);
}

void LCD_init(void) {
  writeGPIO(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
  delay(1);

  writeGPIO(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
  delay(120);

  LCD_writeREG_(0x11); // SLEEP_OFF
  LCD_writeREG_(0x11); // SLEEP_OFF
  delay(120);

  // MEM_CTRL & MEM_CTRL_DAT
  LCD_writeREG_(0x36);
  LCD_writeDAT_(0x00);

  // PIXEL_FORMAT & PIXEL_FORMAT_DAT
  LCD_writeREG_(0x3A);
  LCD_writeDAT_(0x05);

  // PWR_CTRL
  LCD_writeREG_(0xD0);
  LCD_writeDAT_(0xA4);
  LCD_writeDAT_(0xA1);

  // P_GAM_CTRL
  LCD_writeREG_(0xE0);
  LCD_writeDAT_(0xF0);
  LCD_writeDAT_(0x00);
  LCD_writeDAT_(0x04);
  LCD_writeDAT_(0x04);
  LCD_writeDAT_(0x04);
  LCD_writeDAT_(0x05);
  LCD_writeDAT_(0x29);
  LCD_writeDAT_(0x33);
  LCD_writeDAT_(0x3E);
  LCD_writeDAT_(0x38);
  LCD_writeDAT_(0x12);
  LCD_writeDAT_(0x12);
  LCD_writeDAT_(0x28);
  LCD_writeDAT_(0x30);

  // N_GAM_CTRL
  LCD_writeREG_(0xE1);
  LCD_writeDAT_(0xF0);
  LCD_writeDAT_(0x07);
  LCD_writeDAT_(0x0A);
  LCD_writeDAT_(0x0D);
  LCD_writeDAT_(0x0B);
  LCD_writeDAT_(0x07);
  LCD_writeDAT_(0x28);
  LCD_writeDAT_(0x33);
  LCD_writeDAT_(0x3E);
  LCD_writeDAT_(0x36);
  LCD_writeDAT_(0x14);
  LCD_writeDAT_(0x14);
  LCD_writeDAT_(0x29);
  LCD_writeDAT_(0x32);

  LCD_writeREG_(0x21); // INVS_ON

  LCD_writeREG_(0x11); // SLEEP_OFF
  delay(120);

  LCD_writeREG_(0x29); // PLAY_ON

  HAL_TIM_PWM_Start(LCD_TIM, TIM_CHANNEL_2);
}

void LCD_drawPicture(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                     uint8_t pic[]) {
  LCD_setAddress_(x, y, x + width, y + height);
  uint32_t k = 0;
  for (uint32_t i = 0; i < width; i++) {
    for (uint32_t j = 0; j < height; j++) {
      LCD_writeDATS_(pic + 2 * k, 2);
      k++;
    }
  }
}

void LCD_setColor(const uint16_t color) {
  uint8_t color_array[2] = {color >> 8, color};
  LCD_resetCS_();
  tranSPI(LCD_SPI, color_array, sizeof(color_array));
  LCD_setCS_();
}

void LCD_drawScreen(uint16_t xbegin, uint16_t ybegin, uint16_t xend,
                    uint16_t yend, uint16_t color) {
  LCD_setAddress_(xbegin, ybegin, xend, yend);
  for (uint16_t i = ybegin; i <= yend; i++) {
    for (uint16_t j = xbegin; j <= xend; j++) {
      LCD_setColor(color);
    }
  }
}

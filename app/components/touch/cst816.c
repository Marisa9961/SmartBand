/**
 * @file   cst816.c
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  CST816 Driver
 * @date   2025-04-03
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "cst816.h"

extern I2C_HandleTypeDef hi2c3;
#define TOUCH_I2C hi2c3
#define TOUCH_I2C_ADDR (0x15 << 1)

#define TOUCH_FINGER_NUM_REG 0x02
#define TOUCH_X_POSTION_H_REG 0x03
#define TOUCH_Y_POSTION_H_REG 0x05
#define TOUCH_AUTO_SLEEP_REG 0xF9

void TOUCH_init() {
  // TODO RESET GPIO
  uint8_t reg = TOUCH_AUTO_SLEEP_REG;
  HAL_I2C_Master_Transmit(&TOUCH_I2C, TOUCH_I2C_ADDR, &reg, 1, HAL_MAX_DELAY);
  uint8_t data = 5; // senconds
  HAL_I2C_Master_Transmit(&TOUCH_I2C, TOUCH_I2C_ADDR, &data, 1, HAL_MAX_DELAY);

  delay(5);
}

void TOUCH_freshXY(void) {
  uint8_t reg = TOUCH_X_POSTION_H_REG;
  HAL_I2C_Master_Transmit(&TOUCH_I2C, TOUCH_I2C_ADDR, &reg, 1, HAL_MAX_DELAY);
  uint8_t data[4] = {};
  HAL_I2C_Master_Receive(&TOUCH_I2C, TOUCH_I2C_ADDR, data, 4, HAL_MAX_DELAY);
  touch_info.x_pos = ((data[0] & 0x0F) << 8) | data[1];
  touch_info.y_pos = (((data[2] & 0x0F) << 8) | data[3]) + 15;
}

uint8_t TOUCH_getFingerNum(void) {
  uint8_t reg = TOUCH_FINGER_NUM_REG;
  HAL_I2C_Master_Transmit(&TOUCH_I2C, TOUCH_I2C_ADDR, &reg, 1, HAL_MAX_DELAY);
  uint8_t data = 0;
  HAL_I2C_Master_Receive(&TOUCH_I2C, TOUCH_I2C_ADDR, &data, 1, HAL_MAX_DELAY);
  return data;
}

TOUCH_Info touch_info;

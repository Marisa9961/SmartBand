/**
 * @file   aht21.c
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  AHT21 Temperature Sensor
 * @date   2025-04-03
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "aht21.h"

extern I2C_HandleTypeDef hi2c2;
#define TEMP_I2C hi2c2
#define AHT21_I2C_ADDR (0x38 << 1)

uint32_t HW_TEMP;
uint32_t HW_HUMI;

static bool THER_isBusy() {
  uint8_t status = 0x71;
  HAL_I2C_Master_Transmit(&TEMP_I2C, AHT21_I2C_ADDR, &status, 1, HAL_MAX_DELAY);
  HAL_I2C_Master_Receive(&TEMP_I2C, AHT21_I2C_ADDR, &status, 1, HAL_MAX_DELAY);
  return (status & 0x08) == 0;
}

static void THER_wait() {
  uint8_t timeout = 5;
  while (THER_isBusy() && timeout--) {
    delay(5);
  }
}

void THER_init() {
  THER_wait();

  uint8_t init_cmd[] = {0xBE, 0x08, 0x00};
  HAL_I2C_Master_Transmit(&TEMP_I2C, AHT21_I2C_ADDR, init_cmd, sizeof(init_cmd),
                          HAL_MAX_DELAY);
  delay(10);
}

void THER_fresh() {
  uint8_t measure_cmd[] = {0xAC, 0x33, 0x00};

  HAL_I2C_Master_Transmit(&TEMP_I2C, AHT21_I2C_ADDR, measure_cmd,
                          sizeof(measure_cmd), HAL_MAX_DELAY);
  delay(80);
  THER_wait();

  uint8_t raw_data[6];
  HAL_I2C_Master_Receive(&TEMP_I2C, AHT21_I2C_ADDR, raw_data, 6, HAL_MAX_DELAY);

  uint32_t humi_raw = ((uint32_t)raw_data[1] << 16) |
                      ((uint32_t)raw_data[2] << 8) | (uint32_t)raw_data[3];
  humi_raw = humi_raw >> 4;

  uint32_t temp_raw = ((uint32_t)(raw_data[3] & 0x0F) << 16) |
                      ((uint32_t)raw_data[4] << 8) | (uint32_t)raw_data[5];

  HW_HUMI = (uint32_t)(((uint64_t)humi_raw * 1000) >> 20);
  HW_TEMP = (uint32_t)(((uint64_t)temp_raw * 2000) >> 20) - 500;
}

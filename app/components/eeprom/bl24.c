/**
 * @file   bl24.c
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  BL24C02 eeprom driver
 * @date   2025-04-06
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "bl24.h"

extern I2C_HandleTypeDef hi2c1;
#define EEPROM_I2C hi2c1

#define EEPROM_ADDR (0x50 << 1)

void EEPROM_write(uint8_t addr, uint8_t *data, uint8_t length) {
  uint8_t buffer[length + 1];
  uint8_t i;

  buffer[0] = addr;
  for (i = 0; i < length; i++) {
    buffer[i + 1] = data[i];
  }

  HAL_I2C_Master_Transmit(&EEPROM_I2C, EEPROM_ADDR, buffer, length + 1,
                          HAL_MAX_DELAY);
}

void EEPROM_read(uint8_t addr, uint8_t *data, uint8_t length) {
  HAL_I2C_Mem_Read(&EEPROM_I2C, EEPROM_ADDR, addr, I2C_MEMADD_SIZE_8BIT, data,
                   length, HAL_MAX_DELAY);
}

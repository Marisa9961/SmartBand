/**
 * @file   w25q16.c
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  w25q16 impl
 * @date   2025-05-08
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "components/flash/w25q16.h"

extern SPI_HandleTypeDef hspi2;
#define FLASH_HANDLE &hspi2

#define FLASH_WRITE_ENABLE 0x06
#define FLASH_READ_STATUS_REG_1 0x05
#define FLASH_READ_STATUS_REG_2 0x35
#define FLASH_PAGE_PROGRAM 0x02
#define FLASH_SECTOR_ERASE_4KB 0x20
#define FLASH_READ_DATA 0x03

#define FLASH_setCS()                                                          \
  do {                                                                         \
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);         \
  } while (0)

#define FLASH_resetCS()                                                        \
  do {                                                                         \
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);       \
  } while (0)

static void FLASH_WriteEnable() {
  uint8_t cmd = FLASH_WRITE_ENABLE;
  FLASH_resetCS();
  HAL_SPI_Transmit(FLASH_HANDLE, &cmd, 1, HAL_MAX_DELAY);
  FLASH_setCS();
}

static void FLASH_sync(void) {
  uint8_t cmd = FLASH_READ_STATUS_REG_1;
  uint8_t status;

  do {
    FLASH_resetCS();
    HAL_SPI_Transmit(FLASH_HANDLE, &cmd, 1, 10);
    HAL_SPI_Receive(FLASH_HANDLE, &status, 1, 10);
    FLASH_setCS();

    if (!(status & 0x01)) {
      break;
    }

  } while (1);
}

void FLASH_read(uint32_t address, uint8_t *p_data, uint32_t size) {
  uint8_t cmd[4];

  cmd[0] = FLASH_READ_DATA;
  cmd[1] = (address >> 16) & 0xFF;
  cmd[2] = (address >> 8) & 0xFF;
  cmd[3] = address & 0xFF;

  FLASH_resetCS();
  HAL_SPI_Transmit(FLASH_HANDLE, cmd, 4, HAL_MAX_DELAY);
  HAL_SPI_Receive(FLASH_HANDLE, p_data, size, HAL_MAX_DELAY);
  FLASH_setCS();

  FLASH_sync();
}

void FLASH_prog(uint32_t address, uint8_t *p_data, uint16_t size) {
  uint8_t cmd[4];

  FLASH_WriteEnable();

  cmd[0] = FLASH_PAGE_PROGRAM;
  cmd[1] = (address >> 16) & 0xFF;
  cmd[2] = (address >> 8) & 0xFF;
  cmd[3] = address & 0xFF;

  FLASH_resetCS();
  HAL_SPI_Transmit(FLASH_HANDLE, cmd, 4, HAL_MAX_DELAY);
  HAL_SPI_Transmit(FLASH_HANDLE, p_data, size, HAL_MAX_DELAY);
  FLASH_setCS();

  FLASH_sync();
}

void FLASH_erase(uint32_t address) {
  uint8_t cmd[4];

  FLASH_WriteEnable();

  cmd[0] = FLASH_SECTOR_ERASE_4KB;
  cmd[1] = (address >> 16) & 0xFF;
  cmd[2] = (address >> 8) & 0xFF;
  cmd[3] = address & 0xFF;

  FLASH_resetCS();
  HAL_SPI_Transmit(FLASH_HANDLE, cmd, 4, HAL_MAX_DELAY);
  FLASH_setCS();

  FLASH_sync();
}

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
#define FLASH_CHIP_ERASE 0xC7
#define FLASH_READ_DATA 0x03

static void FLASH_WriteEnable(void) {
  uint8_t cmd = FLASH_WRITE_ENABLE;
  HAL_SPI_Transmit(FLASH_HANDLE, &cmd, 1, HAL_MAX_DELAY);
}

static void FLASH_WaitBusy(void) {
  uint8_t cmd = FLASH_READ_STATUS_REG_1;
  uint8_t status;

  do {
    HAL_SPI_Transmit(FLASH_HANDLE, &cmd, 1, 10);
    HAL_SPI_Receive(FLASH_HANDLE, &status, 1, 10);

    if (!(status & 0x01)) {
      break;
    }

    delay(5);

  } while (1);
}

void FLASH_SectorChip(void) {
  FLASH_WriteEnable();

  uint8_t cmd = FLASH_CHIP_ERASE;
  HAL_SPI_Transmit(FLASH_HANDLE, &cmd, 1, HAL_MAX_DELAY);

  FLASH_WaitBusy();
}

void FLASH_SectorErase(uint32_t Address) {
  uint8_t cmd[4];

  FLASH_WriteEnable();

  cmd[0] = FLASH_SECTOR_ERASE_4KB;
  cmd[1] = (Address >> 16) & 0xFF;
  cmd[2] = (Address >> 8) & 0xFF;
  cmd[3] = Address & 0xFF;

  HAL_SPI_Transmit(FLASH_HANDLE, cmd, 4, HAL_MAX_DELAY);
  FLASH_WaitBusy();
}

void FLASH_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count) {
  uint8_t cmd[4];

  cmd[0] = FLASH_READ_DATA;
  cmd[1] = (Address >> 16) & 0xFF;
  cmd[2] = (Address >> 8) & 0xFF;
  cmd[3] = Address & 0xFF;

  HAL_SPI_Transmit(FLASH_HANDLE, cmd, 4, HAL_MAX_DELAY);
  HAL_SPI_Receive(FLASH_HANDLE, DataArray, Count, HAL_MAX_DELAY);
}

void FLASH_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count) {
  uint8_t cmd[4];

  FLASH_WriteEnable();

  cmd[0] = FLASH_PAGE_PROGRAM;
  cmd[1] = (Address >> 16) & 0xFF;
  cmd[2] = (Address >> 8) & 0xFF;
  cmd[3] = Address & 0xFF;

  HAL_SPI_Transmit(FLASH_HANDLE, cmd, 4, HAL_MAX_DELAY);
  HAL_SPI_Transmit(FLASH_HANDLE, DataArray, Count, HAL_MAX_DELAY);

  FLASH_WaitBusy();
}

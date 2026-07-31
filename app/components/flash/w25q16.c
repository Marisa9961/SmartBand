/**
 * @file   w25q16.c
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  w25q16 impl
 * @date   2025-05-08
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "components/flash/w25q16.h"

#include <cmsis_os2.h>

#include "main.h"

extern SPI_HandleTypeDef hspi2;
#define FLASH_HANDLE &hspi2

#define FLASH_CAPACITY_BYTES (2U * 1024U * 1024U)
#define FLASH_PAGE_SIZE 256U
#define FLASH_SECTOR_SIZE 4096U
#define FLASH_SPI_TIMEOUT_MS 100U
#define FLASH_READY_TIMEOUT_MS 5000U

#define FLASH_WRITE_ENABLE 0x06
#define FLASH_READ_STATUS_REG_1 0x05
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

static bool FLASH_rangeValid(uint32_t address, uint32_t size) {
  return address <= FLASH_CAPACITY_BYTES &&
         size <= FLASH_CAPACITY_BYTES - address;
}

static void FLASH_yieldWhileBusy(void) {
  if (osKernelGetState() == osKernelRunning) {
    osDelay(1U);
  }
}

static bool FLASH_WriteEnable(void) {
  uint8_t cmd = FLASH_WRITE_ENABLE;
  FLASH_resetCS();
  const HAL_StatusTypeDef status =
      HAL_SPI_Transmit(FLASH_HANDLE, &cmd, 1U, FLASH_SPI_TIMEOUT_MS);
  FLASH_setCS();
  return status == HAL_OK;
}

bool FLASH_sync(void) {
  uint8_t cmd = FLASH_READ_STATUS_REG_1;
  uint8_t status = 0xFFU;
  const uint32_t started_at = HAL_GetTick();

  do {
    FLASH_resetCS();
    const HAL_StatusTypeDef transmit_status =
        HAL_SPI_Transmit(FLASH_HANDLE, &cmd, 1U, FLASH_SPI_TIMEOUT_MS);
    const HAL_StatusTypeDef receive_status =
        transmit_status == HAL_OK
            ? HAL_SPI_Receive(FLASH_HANDLE, &status, 1U, FLASH_SPI_TIMEOUT_MS)
            : HAL_ERROR;
    FLASH_setCS();

    if (transmit_status != HAL_OK || receive_status != HAL_OK) {
      return false;
    }

    if (!(status & 0x01)) {
      return true;
    }

    FLASH_yieldWhileBusy();
  } while ((HAL_GetTick() - started_at) < FLASH_READY_TIMEOUT_MS);

  return false;
}

bool FLASH_read(uint32_t address, uint8_t *p_data, uint32_t size) {
  if (!FLASH_rangeValid(address, size) ||
      (size > 0U && p_data == NULL) || !FLASH_sync()) {
    return false;
  }
  if (size == 0U) {
    return true;
  }

  uint8_t cmd[4];

  cmd[0] = FLASH_READ_DATA;
  cmd[1] = (address >> 16) & 0xFF;
  cmd[2] = (address >> 8) & 0xFF;
  cmd[3] = address & 0xFF;

  FLASH_resetCS();
  if (HAL_SPI_Transmit(FLASH_HANDLE, cmd, 4U, FLASH_SPI_TIMEOUT_MS) != HAL_OK) {
    FLASH_setCS();
    return false;
  }

  while (size > 0U) {
    const uint16_t chunk =
        size > 0xFFFFU ? 0xFFFFU : (uint16_t)size;
    if (HAL_SPI_Receive(FLASH_HANDLE, p_data, chunk, FLASH_SPI_TIMEOUT_MS) !=
        HAL_OK) {
      FLASH_setCS();
      return false;
    }
    p_data += chunk;
    size -= chunk;
  }

  FLASH_setCS();
  return true;
}

bool FLASH_prog(uint32_t address, const uint8_t *p_data, uint32_t size) {
  if (!FLASH_rangeValid(address, size) ||
      (size > 0U && p_data == NULL)) {
    return false;
  }

  while (size > 0U) {
    const uint32_t page_remaining =
        FLASH_PAGE_SIZE - (address % FLASH_PAGE_SIZE);
    const uint16_t chunk =
        (uint16_t)(size < page_remaining ? size : page_remaining);
    uint8_t cmd[4];

    if (!FLASH_sync() || !FLASH_WriteEnable()) {
      return false;
    }

    cmd[0] = FLASH_PAGE_PROGRAM;
    cmd[1] = (address >> 16) & 0xFF;
    cmd[2] = (address >> 8) & 0xFF;
    cmd[3] = address & 0xFF;

    FLASH_resetCS();
    const HAL_StatusTypeDef command_status =
        HAL_SPI_Transmit(FLASH_HANDLE, cmd, 4U, FLASH_SPI_TIMEOUT_MS);
    const HAL_StatusTypeDef data_status =
        command_status == HAL_OK
            ? HAL_SPI_Transmit(FLASH_HANDLE, (uint8_t *)p_data, chunk,
                               FLASH_SPI_TIMEOUT_MS)
            : HAL_ERROR;
    FLASH_setCS();

    if (command_status != HAL_OK || data_status != HAL_OK || !FLASH_sync()) {
      return false;
    }

    address += chunk;
    p_data += chunk;
    size -= chunk;
  }

  return true;
}

bool FLASH_erase(uint32_t address) {
  if (address >= FLASH_CAPACITY_BYTES ||
      (address % FLASH_SECTOR_SIZE) != 0U || !FLASH_sync() ||
      !FLASH_WriteEnable()) {
    return false;
  }

  uint8_t cmd[4];

  cmd[0] = FLASH_SECTOR_ERASE_4KB;
  cmd[1] = (address >> 16) & 0xFF;
  cmd[2] = (address >> 8) & 0xFF;
  cmd[3] = address & 0xFF;

  FLASH_resetCS();
  const HAL_StatusTypeDef status =
      HAL_SPI_Transmit(FLASH_HANDLE, cmd, 4U, FLASH_SPI_TIMEOUT_MS);
  FLASH_setCS();

  return status == HAL_OK && FLASH_sync();
}

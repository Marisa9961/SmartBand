/**
 * @file   KT6368A.c
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  bluetooth serial port impl
 * @date   2025-04-29
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "kt6368a.h"

#include <string.h>

extern UART_HandleTypeDef huart1;
#define BLE_UART &huart1

#define BLE_BUFFER_SIZE 32U

static struct {
  uint8_t data[BLE_BUFFER_SIZE];
  uint8_t read;
  uint8_t write;
  uint8_t count;
} ble_buffer = {};

void BLE_init() {
  HAL_UARTEx_ReceiveToIdle_DMA(BLE_UART, ble_buffer.data, BLE_BUFFER_SIZE);

  extern DMA_HandleTypeDef hdma_usart1_rx;
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_TC);
}

void BLE_enable() {
  HAL_GPIO_WritePin(BLE_ENABLE_GPIO_Port, BLE_ENABLE_Pin, GPIO_PIN_RESET);
}

void BLE_disable() {
  HAL_GPIO_WritePin(BLE_ENABLE_GPIO_Port, BLE_ENABLE_Pin, GPIO_PIN_SET);
}

void BLE_callback(uint16_t size) {
  if (size > ble_buffer.write) {
    ble_buffer.count += size - ble_buffer.write;
  } else if (size < ble_buffer.write) {
    ble_buffer.count += BLE_BUFFER_SIZE - ble_buffer.read + size;
  }

  ble_buffer.write = size;
}

bool BLE_flag() { return ble_buffer.count > 0; }

char BLE_readNextChar() {
  char c = ble_buffer.data[ble_buffer.read];

  ble_buffer.read =
      ble_buffer.read + 1 >= BLE_BUFFER_SIZE ? 0 : ble_buffer.read + 1;
  ble_buffer.count -= 1;

  return c;
}

void BLE_transmit(const char *str) {
  HAL_UART_Transmit(BLE_UART, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
}

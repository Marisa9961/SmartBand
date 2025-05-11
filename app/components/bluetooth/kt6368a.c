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

#define BLE_BUFFER_SIZE 30U

static struct {
  uint8_t data[BLE_BUFFER_SIZE];
  uint8_t read;
  uint8_t count;
} ble_buffer = {};

/* Some questions are very puzzling. Why does the STM32 DMA circular mode + idle
 * interrupt mode not wake up the interrupt when the buffer is just filled when
 * the half-full and full interrupts are turned off? This causes the interrupt
 * to be unresponsive when a new message arrives, the software cannot work
 * properly, and even introduces sequence errors? Finally, I choose to leave
 * this question alone, and use the single receive mode (DMA Normal) for now. */

void BLE_init() {
  extern DMA_HandleTypeDef hdma_usart1_rx;
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_TC);

  HAL_UARTEx_ReceiveToIdle_DMA(BLE_UART, ble_buffer.data, BLE_BUFFER_SIZE);
}

void BLE_enable() {
  HAL_GPIO_WritePin(BLE_ENABLE_GPIO_Port, BLE_ENABLE_Pin, GPIO_PIN_RESET);
}

void BLE_disable() {
  HAL_GPIO_WritePin(BLE_ENABLE_GPIO_Port, BLE_ENABLE_Pin, GPIO_PIN_SET);
}

void BLE_callback(uint16_t size) {
  ble_buffer.read = 0;
  ble_buffer.count = size;
}

bool BLE_flag() { return ble_buffer.count > 0; }

char BLE_readNextChar() {
  ble_buffer.count -= 1;
  return ble_buffer.data[ble_buffer.read++];
}

void BLE_transmit(const char *str) {
  HAL_UART_Transmit(BLE_UART, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
  HAL_UARTEx_ReceiveToIdle_DMA(BLE_UART, ble_buffer.data, BLE_BUFFER_SIZE);
}

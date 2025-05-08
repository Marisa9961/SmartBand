/**
 * @file   ble.c
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  bluetooth serial port impl
 * @date   2025-04-29
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "ble.h"

#include <string.h>

extern UART_HandleTypeDef huart1;
#define BLE_UART &huart1

#define BLE_BUFFER_SIZE 32U

static struct {
  uint8_t data[BLE_BUFFER_SIZE];
  uint8_t read;
  uint8_t write;
  uint8_t count;
} serial_port_buffer = {};

// blue tooth deinit
void BLE_init() {
  HAL_GPIO_WritePin(BLE_ENABLE_GPIO_Port, BLE_ENABLE_Pin, GPIO_PIN_RESET);
}
void BLE_deinit() {
  HAL_GPIO_WritePin(BLE_ENABLE_GPIO_Port, BLE_ENABLE_Pin, GPIO_PIN_SET);
}

void BLE_freshInterrupt() {
  HAL_UARTEx_ReceiveToIdle_DMA(BLE_UART, serial_port_buffer.data,
                               BLE_BUFFER_SIZE);
}

void BLE_callback(uint16_t size) {
  // each time call the BLE_freshInterrupt()
  // the buffer will be clear
  // so, there doesn't need ring buffer?
  // why? why? why?

  // if (size > serial_port_buffer.write) {
  //   serial_port_buffer.count += size - serial_port_buffer.write;
  // } else if (size < serial_port_buffer.write) {
  //   serial_port_buffer.count +=
  //       BLE_BUFFER_SIZE - serial_port_buffer.read + size;
  // }

  serial_port_buffer.read = 0;
  serial_port_buffer.count = size;
}

bool BLE_flag() { return serial_port_buffer.count > 0; }

char BLE_readNextChar() {
  char c = serial_port_buffer.data[serial_port_buffer.read];

  serial_port_buffer.read += 1;
  serial_port_buffer.count -= 1;

  return c;
}

// stop using DMA, avoid competition
void BLE_transmit(const char *str) {
  HAL_UART_Transmit(BLE_UART, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
}

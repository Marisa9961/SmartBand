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

#define BLE_BUFFER_SIZE 128U

static struct {
  uint8_t data[BLE_BUFFER_SIZE];
  uint8_t read;
  uint8_t write;
  uint8_t count;
} serial_port_buffer = {};

// blue tooth init
void BLE_init() {
  BLE_enable();
  HAL_UARTEx_ReceiveToIdle_DMA(BLE_UART, serial_port_buffer.data,
                               BLE_BUFFER_SIZE);
}

// blue tooth deinit
void BLE_enable() {
  HAL_GPIO_WritePin(BLE_ENABLE_GPIO_Port, BLE_ENABLE_Pin, GPIO_PIN_RESET);
}
void BLE_disable() {
  HAL_GPIO_WritePin(BLE_ENABLE_GPIO_Port, BLE_ENABLE_Pin, GPIO_PIN_SET);
}

// used for HAL callback impl
void BLE_callback(uint16_t size) {
  if (size > serial_port_buffer.write) {
    serial_port_buffer.count += size - serial_port_buffer.write;
  } else if (size < serial_port_buffer.write) {
    serial_port_buffer.count +=
        BLE_BUFFER_SIZE - serial_port_buffer.read + size;
  }

  serial_port_buffer.write = size;

  BLE_setCallback();
}

// when new data comes, flag is true
bool BLE_flag() { return serial_port_buffer.count > 0; }

// pop the next char
char BLE_readNextChar() {
  char c = serial_port_buffer.data[serial_port_buffer.read];

  serial_port_buffer.read = serial_port_buffer.read + 1 >= BLE_BUFFER_SIZE
                                ? 0
                                : serial_port_buffer.read + 1;
  serial_port_buffer.count -= 1;

  return c;
}

// call back flag
static uint32_t HW_BLE_CALLBACK;
void BLE_setCallback() { HW_BLE_CALLBACK = true; }
void BLE_ClearCallback() { HW_BLE_CALLBACK = false; }

// transmit message
void BLE_transmit(const char *str) {
  HAL_UART_Transmit_DMA(BLE_UART, (uint8_t *)str, strlen(str));
}

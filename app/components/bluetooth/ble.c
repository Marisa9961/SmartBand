/**
 * @file   ble.c
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  bluetooth serial port impl
 * @date   2025-04-29
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "ble.h"

extern UART_HandleTypeDef huart1;
#define SERIAL_PORT_UART &huart1

#define SERIAL_BUFFER_SIZE 256U

static struct {
  uint8_t data[SERIAL_BUFFER_SIZE];
  uint8_t read;
  uint8_t write;
  uint8_t count;
} serial_port_buffer = {};

// blue tooth init
void BLE_init() {
  HAL_UARTEx_ReceiveToIdle_DMA(SERIAL_PORT_UART, serial_port_buffer.data,
                               SERIAL_BUFFER_SIZE);
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
        SERIAL_BUFFER_SIZE - serial_port_buffer.read + size;
  }

  serial_port_buffer.write = size;
}

// When new data comes, flag is true
bool BLE_flag() { return serial_port_buffer.count > 0; }

// pop the next char
char BLE_readNextChar() {
  char c = serial_port_buffer.data[serial_port_buffer.read];

  serial_port_buffer.read = serial_port_buffer.read + 1 >= SERIAL_BUFFER_SIZE
                                ? 0
                                : serial_port_buffer.read + 1;
  serial_port_buffer.count -= 1;

  return c;
}

// used for turn on or off
uint32_t HW_BLE;

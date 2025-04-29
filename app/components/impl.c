#include "components/prf.h"

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
  if (huart->Instance == USART1) {
    extern void BLE_callback(uint16_t size);
    BLE_callback(Size);
  }
}

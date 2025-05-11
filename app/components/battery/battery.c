/**
 * @file   battery.c
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  bat impl
 * @date   2025-05-10
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "battery.h"

extern ADC_HandleTypeDef hadc1;
#define ADC_HANDLE &hadc1

uint32_t HW_BATTERY;

void BAT_enable() {
  HAL_GPIO_WritePin(BATTERY_GPIO_Port, BATTERY_Pin, GPIO_PIN_SET);
}

void BAT_disable() {
  HAL_GPIO_WritePin(BATTERY_GPIO_Port, BATTERY_Pin, GPIO_PIN_RESET);
}

void BAT_fresh() {
  HAL_ADC_Start(ADC_HANDLE);
  HAL_ADC_PollForConversion(ADC_HANDLE, HAL_MAX_DELAY);
  HW_BATTERY = HAL_ADC_GetValue(ADC_HANDLE);
  HAL_ADC_Stop(ADC_HANDLE);
}

bool BAT_charging() {
  return HAL_GPIO_ReadPin(CHARGE_GPIO_Port, CHARGE_Pin) == GPIO_PIN_SET;
}

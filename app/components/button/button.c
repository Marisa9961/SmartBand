/**
 * @file   button.c
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  button impl
 * @date   2025-05-15
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "button.h"

bool BTN_read() { return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET; }

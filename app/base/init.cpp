/**
 * @file   init.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  start FreeRTOS kernel
 * @date   2025-03-21
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include <ranges>

#include "func/task.hpp"
#include "tasks/core.hpp"

extern "C" {
#include <cmsis_os2.h>

#include "main.h"

void init() {
  // TODO long press waitting for power on
  // HAL_Delay(3000);

  bd::task::core.run();

  if (osKernelInitialize() == osOK) {
    osKernelStart();
  }

  while (true) {
    // Should never get here
  }
}
}

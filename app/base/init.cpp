/**
 * @file   init.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  start FreeRTOS kernel
 * @date   2025-03-21
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "base/bootstrap.h"
#include "tasks/core.hpp"

extern "C" {
#include <cmsis_os2.h>

#include "main.h"

[[noreturn]] void app_fatal(void) {
  __disable_irq();
  while (true) {
  }
}

[[noreturn]] void app_bootstrap(void) {
  // TODO long press waitting for power on
  // HAL_Delay(3000);

  if (osKernelInitialize() != osOK) {
    app_fatal();
  }

  if (!bd::task::prepareRuntime()) {
    app_fatal();
  }

  if (osKernelStart() != osOK) {
    app_fatal();
  }

  app_fatal();
}
} // extern "C"

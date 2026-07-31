/**
 * @file   sensor.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  sensor impl
 * @date   2025-05-12
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "tasks/sensor.hpp"

#include "base/bootstrap.h"
#include "components/direct/lsm303.h"
#include "components/thermometer/aht21.h"
#include "tasks/core.hpp"

namespace bd::task {
namespace {
constexpr uint32_t sensor_period_ticks{1000U};
}

[[noreturn]] void SensorTask(void *argument) {
  static_cast<void>(argument);

  THER_init();
  DIRECT_init();

  if (reportTaskReady(TaskId::Sensor) != osOK) {
    app_fatal();
  }

  uint32_t next_wake{osKernelGetTickCount()};

  while (true) {
    next_wake += sensor_period_ticks;
    if (osDelayUntil(next_wake) != osOK) {
      next_wake = osKernelGetTickCount();
    }

    THER_fresh();
    DIRECT_fresh();
  }
}
} // namespace bd::task

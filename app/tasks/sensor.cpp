/**
 * @file   sensor.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  sensor impl
 * @date   2025-05-12
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "sensor.hpp"

#include "components/direct/lsm303.h"
#include "components/thermometer/aht21.h"

namespace bd::task {
void SensorTask(void *argument) {
  THER_init();
  DIRECT_init();

  while (true) {
    osDelay(1000);

    THER_fresh();
    DIRECT_fresh();
  }
}

Task sensor{SensorTask, "Core", 128 * 4, osPriorityNormal};
} // namespace bd::task

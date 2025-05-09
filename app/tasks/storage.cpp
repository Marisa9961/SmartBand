/**
 * @file   storage.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  storage impl
 * @date   2025-05-08
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "tasks/storage.hpp"

#include "tasks/core.hpp"

#include "func/lfs.hpp"

// TODO lfs

namespace bd::task {
void StorageTask(void *argument) {
  while (true) {
    osThreadFlagsWait(core.flag(), osFlagsWaitAll, osWaitForever);
  }
}

Task storage{StorageTask, "Storage", 128 * 4, osPriorityNormal};
} // namespace bd::task

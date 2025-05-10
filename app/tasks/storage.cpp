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

#include "porting/lfs_heap.h"
#include "porting/lfs_porting.h"

#include "components/eeprom/bl24.h"

namespace bd::task {
void StorageTask(void *argument) {
  lfs_t lfs{};

  lfs_memory_init();

  int err = lfs_mount(&lfs, &lfs_config);

  if (err) {
    lfs_format(&lfs, &lfs_config);
    lfs_mount(&lfs, &lfs_config);
  }

  uint8_t data{};
  EEPROM_read(0, &data, 1);

  data += 1;
  EEPROM_write(0, &data, 1);

  osDelay(5);

  while (true) {
    osThreadFlagsWait(core.flag(), osFlagsWaitAll, osWaitForever);
  }
}

Task storage{StorageTask, "Storage", 128 * 8, osPriorityNormal};
} // namespace bd::task

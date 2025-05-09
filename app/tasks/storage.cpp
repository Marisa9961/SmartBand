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

namespace bd::task {
void StorageTask(void *argument) {
  lfs_t lfs{};

  lfs_memory_init();

  int err = lfs_mount(&lfs, &lfs_config);

  if (err) {
    lfs_format(&lfs, &lfs_config);
    lfs_mount(&lfs, &lfs_config);
  }

  lfs_file_t file{};

  uint32_t boot_count = 0;
  lfs_file_open(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
  lfs_file_read(&lfs, &file, &boot_count, sizeof(boot_count));

  boot_count += 1;
  lfs_file_rewind(&lfs, &file);
  lfs_file_write(&lfs, &file, &boot_count, sizeof(boot_count));

  lfs_file_close(&lfs, &file);

  lfs_unmount(&lfs);

  while (true) {
    osThreadFlagsWait(core.flag(), osFlagsWaitAll, osWaitForever);
  }
}

Task storage{StorageTask, "Storage", 128 * 8, osPriorityNormal};
} // namespace bd::task

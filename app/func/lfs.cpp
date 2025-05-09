/**
 * @file   lfs.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  lfs wrapper impl
 * @date   2025-05-08
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "lfs.hpp"

#include "components/flash/w25q16.h"

constexpr uint32_t FLASH_BLOCK_SIZE = 4 * 1024;

int littlefs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off,
                  void *buffer, lfs_size_t size) {
  FLASH_read(block * FLASH_BLOCK_SIZE + off, (uint8_t *)buffer, size);
  return 0;
}

int littlefs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off,
                  const void *buffer, lfs_size_t size) {
  FLASH_prog(block * FLASH_BLOCK_SIZE + off, (uint8_t *)buffer, size);
  return 0;
}

int littlefs_erase(const struct lfs_config *c, lfs_block_t block) {
  FLASH_erase(block * FLASH_BLOCK_SIZE);
  return 0;
}

int littlefs_sync(const struct lfs_config *c) {
  FLASH_sync();
  return 0;
}

struct lfs_config littlefs_config = {
    // block device operations
    .read = littlefs_read,
    .prog = littlefs_prog,
    .erase = littlefs_erase,
    .sync = littlefs_sync,

    // block device configuration
    .read_size = 256,
    .prog_size = 256,
    .block_size = 4096,
    .block_count = 256,
    .block_cycles = 100,
    .cache_size = 256,
    .lookahead_size = 8,
};

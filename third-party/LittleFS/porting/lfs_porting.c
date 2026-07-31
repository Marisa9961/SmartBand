#include "lfs_porting.h"

#define FLASH_BLOCK_SIZE (4 * 1024)

static bool littlefs_accessValid(const struct lfs_config *c,
                                 lfs_block_t block, lfs_off_t off,
                                 lfs_size_t size) {
  return c != NULL && block < c->block_count && off <= c->block_size &&
         size <= c->block_size - off;
}

static int littlefs_read(const struct lfs_config *c, lfs_block_t block,
                         lfs_off_t off, void *buffer, lfs_size_t size) {
  if (!littlefs_accessValid(c, block, off, size)) {
    return LFS_ERR_INVAL;
  }
  return FLASH_read(block * FLASH_BLOCK_SIZE + off, (uint8_t *)buffer, size)
             ? LFS_ERR_OK
             : LFS_ERR_IO;
}

static int littlefs_prog(const struct lfs_config *c, lfs_block_t block,
                         lfs_off_t off, const void *buffer, lfs_size_t size) {
  if (!littlefs_accessValid(c, block, off, size)) {
    return LFS_ERR_INVAL;
  }
  return FLASH_prog(block * FLASH_BLOCK_SIZE + off,
                    (const uint8_t *)buffer, size)
             ? LFS_ERR_OK
             : LFS_ERR_IO;
}

static int littlefs_erase(const struct lfs_config *c, lfs_block_t block) {
  if (c == NULL || block >= c->block_count) {
    return LFS_ERR_INVAL;
  }
  return FLASH_erase(block * FLASH_BLOCK_SIZE) ? LFS_ERR_OK : LFS_ERR_IO;
}

static int littlefs_sync(const struct lfs_config *c) {
  if (c == NULL) {
    return LFS_ERR_INVAL;
  }
  return FLASH_sync() ? LFS_ERR_OK : LFS_ERR_IO;
}

const struct lfs_config lfs_config = {
    // block device operations
    .read = littlefs_read,
    .prog = littlefs_prog,
    .erase = littlefs_erase,
    .sync = littlefs_sync,

    // block device configuration
    .read_size = 256,
    .prog_size = 256,
    .block_size = 4096,
    .block_count = 512,
    .block_cycles = 500,
    .cache_size = 256,
    .lookahead_size = 256,
};

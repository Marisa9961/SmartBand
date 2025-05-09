/**
 * @file   lfs_conf.h
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  used to override part of lfs_util.h
 * @date   2025-05-09
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef __LFS_CONF_H__
#define __LFS_CONF_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LFS_NO_ERROR
#define LFS_NO_WARN
#define LFS_NO_DEBUG
#define LFS_NO_ASSERT

extern void *lfs_memory_alloc(size_t size);
#define LFS_MALLOC(size) lfs_memory_alloc(size)

extern void lfs_memory_free(void *p);
#define LFS_FREE(p) lfs_memory_free(p)

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

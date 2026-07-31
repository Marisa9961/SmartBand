#ifndef __LFS_HEAP_H__
#define __LFS_HEAP_H__

#include <stdbool.h>
#include <stddef.h>

#include <cmsis_os2.h>

#ifdef __cplusplus
extern "C" {
#endif

bool lfs_memory_init(void);
void *lfs_memory_alloc(size_t size);
void lfs_memory_free(void *p);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif

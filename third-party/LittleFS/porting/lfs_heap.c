#include "porting/lfs_heap.h"

#define MEMPOOL_OBJECTS_NUM 4

typedef struct {
  uint32_t Buf[64];
} LfsMemoryBlock;

static osMemoryPoolId_t lfs_memory;

void lfs_memory_init() {
  lfs_memory =
      osMemoryPoolNew(MEMPOOL_OBJECTS_NUM, sizeof(LfsMemoryBlock), nullptr);
}

void *lfs_memory_alloc(size_t size) {
  return osMemoryPoolAlloc(lfs_memory, osWaitForever);
}

void lfs_memory_free(void *p) { osMemoryPoolFree(lfs_memory, p); }

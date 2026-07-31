#include "porting/lfs_heap.h"

#define MEMPOOL_OBJECTS_NUM 4
#define MEMPOOL_BLOCK_SIZE 256U

typedef struct {
  uint32_t Buf[MEMPOOL_BLOCK_SIZE / sizeof(uint32_t)];
} LfsMemoryBlock;

static osMemoryPoolId_t lfs_memory;

bool lfs_memory_init(void) {
  if (lfs_memory != NULL) {
    return true;
  }

  lfs_memory =
      osMemoryPoolNew(MEMPOOL_OBJECTS_NUM, sizeof(LfsMemoryBlock), NULL);
  return lfs_memory != NULL;
}

void *lfs_memory_alloc(size_t size) {
  if (lfs_memory == NULL || size == 0U || size > sizeof(LfsMemoryBlock)) {
    return NULL;
  }
  return osMemoryPoolAlloc(lfs_memory, 0U);
}

void lfs_memory_free(void *p) {
  if (lfs_memory != NULL && p != NULL) {
    (void)osMemoryPoolFree(lfs_memory, p);
  }
}

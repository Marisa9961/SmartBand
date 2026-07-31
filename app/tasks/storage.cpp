/**
 * @file   storage.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  storage impl
 * @date   2025-05-08
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "tasks/storage.hpp"

#include "base/bootstrap.h"
#include "func/task.hpp"
#include "porting/lfs_heap.h"
#include "porting/lfs_porting.h"
#include "tasks/core_messages.hpp"

namespace bd::task {
namespace {
constexpr uint32_t storage_inbox_capacity{8U};
osMessageQueueId_t storage_inbox{nullptr};
}

bool prepareStorageChannel() {
  if (storage_inbox != nullptr) {
    return false;
  }

  storage_inbox = osMessageQueueNew(storage_inbox_capacity,
                                    sizeof(StorageCommand), nullptr);
  return storage_inbox != nullptr;
}

osStatus_t postStorageCommand(const StorageCommand &command, uint32_t timeout) {
  if (storage_inbox == nullptr) {
    return osErrorResource;
  }

  return osMessageQueuePut(storage_inbox, &command, 0U, timeout);
}

[[noreturn]] void StorageTask(void *argument) {
  static_cast<void>(argument);

  lfs_t lfs{};

  lfs_memory_init();

  int err = lfs_mount(&lfs, &lfs_config);

  if (err) {
    err = lfs_format(&lfs, &lfs_config);
    if (!err) {
      err = lfs_mount(&lfs, &lfs_config);
    }
  }

  if (err) {
    if (reportTaskFault(TaskId::Storage, err) != osOK) {
      app_fatal();
    }
    osThreadExit();
    app_fatal();
  }

  if (reportTaskReady(TaskId::Storage) != osOK) {
    app_fatal();
  }

  while (true) {
    StorageCommand command{};
    if (osMessageQueueGet(storage_inbox, &command, nullptr, osWaitForever) !=
        osOK) {
      if (reportTaskFault(TaskId::Storage, LFS_ERR_IO) != osOK) {
        app_fatal();
      }
      continue;
    }

    switch (command.type) {
    case StorageCommandType::MakeConsistent:
      err = lfs_fs_mkconsistent(&lfs);
      break;
    case StorageCommandType::GarbageCollect:
      err = lfs_fs_gc(&lfs);
      break;
    }

    if (err && reportTaskFault(TaskId::Storage, err) != osOK) {
      app_fatal();
    }
  }
}
} // namespace bd::task

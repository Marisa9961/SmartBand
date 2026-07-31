/**
 * @file   storage.hpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  storage define
 * @date   2025-05-08
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#ifndef __TASKS_STORAGE_HPP__
#define __TASKS_STORAGE_HPP__

#include <cstdint>

#include <cmsis_os2.h>

namespace bd::task {
enum class StorageCommandType : uint8_t {
  MakeConsistent,
  GarbageCollect,
};

struct StorageCommand {
  StorageCommandType type;
};

bool prepareStorageChannel();
osStatus_t postStorageCommand(const StorageCommand &command,
                              uint32_t timeout = 0);
[[noreturn]] void StorageTask(void *argument);
} // namespace bd::task

#endif

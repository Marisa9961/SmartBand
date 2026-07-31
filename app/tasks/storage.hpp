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
#include <span>
#include <string_view>

#include <cmsis_os2.h>

#include "func/storage/storage_types.hpp"

namespace bd::task {
bool prepareStorageChannel();

/**
 * Creates a single-item queue with the correct item size for StorageResponse.
 * A caller must keep at most one reply-producing request outstanding on each
 * response queue. StorageTask never blocks while publishing a response.
 */
osMessageQueueId_t createStorageResponseQueue();

osStatus_t requestStorageRead(
    std::string_view path, osMessageQueueId_t response_queue,
    uint32_t timeout = 0U);

osStatus_t requestStorageWrite(
    std::string_view path, std::span<const uint8_t> data,
    osMessageQueueId_t response_queue = nullptr, uint32_t timeout = 0U);

osStatus_t requestStorageRemove(
    std::string_view path, osMessageQueueId_t response_queue = nullptr,
    uint32_t timeout = 0U);

osStatus_t requestStorageMaintenance(
    storage::StorageOperation operation,
    osMessageQueueId_t response_queue = nullptr, uint32_t timeout = 0U);

[[noreturn]] void StorageTask(void *argument);
} // namespace bd::task

#endif

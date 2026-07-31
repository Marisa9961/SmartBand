/**
 * @file   storage.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  Serialized LittleFS request dispatcher.
 * @date   2025-05-08
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "tasks/storage.hpp"

#include <algorithm>
#include <type_traits>

#include "base/bootstrap.h"
#include "func/storage/littlefs_store.hpp"
#include "tasks/core.hpp"

namespace bd::task {
namespace {
constexpr uint32_t storage_inbox_capacity{4U};

struct StorageRequest {
  storage::StorageOperation operation{storage::StorageOperation::Read};
  uint16_t size{0U};
  osMessageQueueId_t response_queue{nullptr};
  storage::StoragePath path{};
  storage::StoragePayload data{};
};

static_assert(std::is_trivially_copyable_v<StorageRequest>);
static_assert(std::is_trivially_copyable_v<storage::StorageResponse>);

osMessageQueueId_t storage_inbox{nullptr};

osStatus_t submitStorageRequest(const StorageRequest &request,
                                uint32_t timeout) {
  if (storage_inbox == nullptr) {
    return osErrorResource;
  }

  return osMessageQueuePut(storage_inbox, &request, 0U, timeout);
}

[[noreturn]] void stopStorageTask() {
  if (reportTaskFault(TaskId::Storage) != osOK) {
    app_fatal();
  }
  osThreadExit();
  app_fatal();
}
} // namespace

bool prepareStorageChannel() {
  if (storage_inbox != nullptr) {
    return false;
  }

  storage_inbox = osMessageQueueNew(storage_inbox_capacity,
                                    sizeof(StorageRequest), nullptr);
  return storage_inbox != nullptr;
}

osMessageQueueId_t createStorageResponseQueue() {
  return osMessageQueueNew(1U, sizeof(storage::StorageResponse), nullptr);
}

osStatus_t requestStorageRead(std::string_view path,
                              osMessageQueueId_t response_queue,
                              uint32_t timeout) {
  StorageRequest request{};
  if (response_queue == nullptr ||
      !storage::copyPath(path, request.path)) {
    return osErrorParameter;
  }

  request.operation = storage::StorageOperation::Read;
  request.response_queue = response_queue;
  return submitStorageRequest(request, timeout);
}

osStatus_t requestStorageReplace(std::string_view path,
                                 std::span<const uint8_t> data,
                                 osMessageQueueId_t response_queue,
                                 uint32_t timeout) {
  StorageRequest request{};
  if (data.size() > storage::payload_capacity ||
      !storage::copyPath(path, request.path)) {
    return osErrorParameter;
  }

  request.operation = storage::StorageOperation::Replace;
  request.size = static_cast<uint16_t>(data.size());
  request.response_queue = response_queue;
  std::copy(data.begin(), data.end(), request.data.begin());
  return submitStorageRequest(request, timeout);
}

osStatus_t requestStorageRemove(std::string_view path,
                                osMessageQueueId_t response_queue,
                                uint32_t timeout) {
  StorageRequest request{};
  if (!storage::copyPath(path, request.path)) {
    return osErrorParameter;
  }

  request.operation = storage::StorageOperation::Remove;
  request.response_queue = response_queue;
  return submitStorageRequest(request, timeout);
}

[[noreturn]] void StorageTask(void *argument) {
  static_cast<void>(argument);

  storage::LittleFsStore store{};
  const storage::StorageOutcome initialization{store.initialize()};
  const osStatus_t lifecycle_status{
      initialization.ok()
          ? reportTaskReady(TaskId::Storage)
          : reportTaskFault(TaskId::Storage)};
  if (lifecycle_status != osOK) {
    app_fatal();
  }

  while (true) {
    StorageRequest request{};
    if (osMessageQueueGet(storage_inbox, &request, nullptr, osWaitForever) !=
        osOK) {
      stopStorageTask();
    }

    storage::StorageResponse response{};
    response.operation = request.operation;
    storage::StorageOutcome outcome{storage::StorageStatus::InvalidArgument,
                                    LFS_ERR_INVAL};

    const std::string_view path{request.path.data()};
    switch (request.operation) {
    case storage::StorageOperation::Read: {
      std::size_t bytes_read{0U};
      outcome = store.read(path, response.data, bytes_read);
      if (outcome.ok()) {
        response.size = static_cast<uint16_t>(bytes_read);
      }
      break;
    }
    case storage::StorageOperation::Replace:
      if (request.size > request.data.size()) {
        outcome = {storage::StorageStatus::InvalidArgument, LFS_ERR_INVAL};
      } else {
        outcome = store.replace(
            path,
            std::span<const uint8_t>{request.data.data(), request.size});
        if (outcome.ok()) {
          response.size = request.size;
        }
      }
      break;
    case storage::StorageOperation::Remove:
      outcome = store.remove(path);
      break;
    }

    response.status = outcome.status;
    response.detail = outcome.detail;

    if (request.response_queue != nullptr) {
      static_cast<void>(
          osMessageQueuePut(request.response_queue, &response, 0U, 0U));
    }
  }
}
} // namespace bd::task

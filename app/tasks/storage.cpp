/**
 * @file   storage.cpp
 * @author Marisa9961 (marisa15532@gmail.com)
 * @brief  storage impl
 * @date   2025-05-08
 *
 * @copyright Copyright (c) Marisa9961 2025
 */

#include "tasks/storage.hpp"

#include <algorithm>
#include <type_traits>

#include "base/bootstrap.h"
#include "func/task.hpp"
#include "func/storage/littlefs_store.hpp"
#include "tasks/core_messages.hpp"

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

bool validPath(std::string_view path) {
  return !path.empty() && path.size() <= storage::max_path_length &&
         path.find('\0') == std::string_view::npos;
}

osStatus_t submitStorageRequest(const StorageRequest& request,
                                uint32_t timeout) {
  if (storage_inbox == nullptr) {
    return osErrorResource;
  }

  return osMessageQueuePut(storage_inbox, &request, 0U, timeout);
}

void copyPath(std::string_view source, storage::StoragePath& destination) {
  std::copy(source.begin(), source.end(), destination.begin());
}

[[noreturn]] void stopStorageTask(int32_t detail) {
  if (reportTaskFault(TaskId::Storage, detail) != osOK) {
    app_fatal();
  }
  osThreadExit();
  app_fatal();
}
}  // namespace

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
  if (!validPath(path) || response_queue == nullptr) {
    return osErrorParameter;
  }

  StorageRequest request{};
  request.operation = storage::StorageOperation::Read;
  request.response_queue = response_queue;
  copyPath(path, request.path);
  return submitStorageRequest(request, timeout);
}

osStatus_t requestStorageWrite(std::string_view path,
                               std::span<const uint8_t> data,
                               osMessageQueueId_t response_queue,
                               uint32_t timeout) {
  if (!validPath(path) || data.size() > storage::payload_capacity) {
    return osErrorParameter;
  }

  StorageRequest request{};
  request.operation = storage::StorageOperation::Write;
  request.size = static_cast<uint16_t>(data.size());
  request.response_queue = response_queue;
  copyPath(path, request.path);
  std::copy(data.begin(), data.end(), request.data.begin());
  return submitStorageRequest(request, timeout);
}

osStatus_t requestStorageRemove(std::string_view path,
                                osMessageQueueId_t response_queue,
                                uint32_t timeout) {
  if (!validPath(path)) {
    return osErrorParameter;
  }

  StorageRequest request{};
  request.operation = storage::StorageOperation::Remove;
  request.response_queue = response_queue;
  copyPath(path, request.path);
  return submitStorageRequest(request, timeout);
}

osStatus_t requestStorageMaintenance(storage::StorageOperation operation,
                                     osMessageQueueId_t response_queue,
                                     uint32_t timeout) {
  if (operation != storage::StorageOperation::MakeConsistent &&
      operation != storage::StorageOperation::GarbageCollect) {
    return osErrorParameter;
  }

  StorageRequest request{};
  request.operation = operation;
  request.response_queue = response_queue;
  return submitStorageRequest(request, timeout);
}

[[noreturn]] void StorageTask(void* argument) {
  static_cast<void>(argument);

  storage::LittleFsStore store{};
  const storage::StorageOutcome initialization{store.initialize()};
  const osStatus_t lifecycle_status{
      initialization.ok()
          ? reportTaskReady(TaskId::Storage)
          : reportTaskFault(TaskId::Storage, initialization.detail)};
  if (lifecycle_status != osOK) {
    app_fatal();
  }

  while (true) {
    StorageRequest request{};
    if (osMessageQueueGet(storage_inbox, &request, nullptr, osWaitForever) !=
        osOK) {
      stopStorageTask(LFS_ERR_IO);
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
      case storage::StorageOperation::Write:
        if (request.size > request.data.size()) {
          outcome = {storage::StorageStatus::InvalidArgument, LFS_ERR_INVAL};
        } else {
          outcome = store.write(path, std::span<const uint8_t>{
                                          request.data.data(), request.size});
          if (outcome.ok()) {
            response.size = request.size;
          }
        }
        break;
      case storage::StorageOperation::Remove:
        outcome = store.remove(path);
        break;
      case storage::StorageOperation::MakeConsistent:
        outcome = store.makeConsistent();
        break;
      case storage::StorageOperation::GarbageCollect:
        outcome = store.garbageCollect();
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
}  // namespace bd::task

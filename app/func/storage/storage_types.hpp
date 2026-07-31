/**
 * @file storage_types.hpp
 * @brief Value types shared by the storage service and its clients.
 */

#ifndef BD_STORAGE_STORAGE_TYPES_HPP
#define BD_STORAGE_STORAGE_TYPES_HPP

#include <array>
#include <cstddef>
#include <cstdint>

namespace bd::storage {
// Small, bounded records keep RTOS queue allocation deterministic. Four extra
// path bytes are reserved internally for the atomic-write ".tmp" suffix.
inline constexpr std::size_t max_path_length{27U};
inline constexpr std::size_t payload_capacity{128U};

using StoragePath = std::array<char, max_path_length + 1U>;
using StoragePayload = std::array<uint8_t, payload_capacity>;

enum class StorageOperation : uint8_t {
  Read,
  Write,
  Remove,
  MakeConsistent,
  GarbageCollect,
};

enum class StorageStatus : uint8_t {
  Ok,
  NotReady,
  InvalidArgument,
  NotFound,
  AlreadyExists,
  NotEmpty,
  TooLarge,
  NoSpace,
  OutOfMemory,
  IoError,
  Corrupt,
  InternalError,
};

struct StorageOutcome {
  StorageStatus status{StorageStatus::InternalError};
  int32_t detail{0};

  [[nodiscard]] constexpr bool ok() const {
    return status == StorageStatus::Ok;
  }
};

struct StorageResponse {
  StorageOperation operation{StorageOperation::Read};
  StorageStatus status{StorageStatus::InternalError};
  int32_t detail{0};
  uint16_t size{0U};
  StoragePayload data{};
};
} // namespace bd::storage

#endif

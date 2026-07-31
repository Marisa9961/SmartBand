/**
 * @file storage_types.hpp
 * @brief Value types shared by the storage service and its clients.
 */

#ifndef BD_STORAGE_STORAGE_TYPES_HPP
#define BD_STORAGE_STORAGE_TYPES_HPP

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace bd::storage {
// Small, bounded records keep RTOS queue allocation deterministic. Four extra
// path bytes are reserved internally for the atomic-write ".tmp" suffix.
inline constexpr std::size_t max_path_length{27U};
inline constexpr std::size_t payload_capacity{128U};

using StoragePath = std::array<char, max_path_length + 1U>;
using StoragePayload = std::array<uint8_t, payload_capacity>;

[[nodiscard]] constexpr bool isValidPath(std::string_view path) {
  return !path.empty() && path.size() <= max_path_length &&
         path.find('\0') == std::string_view::npos;
}

[[nodiscard]] inline bool copyPath(std::string_view source,
                                   StoragePath &destination) {
  if (!isValidPath(source)) {
    return false;
  }

  destination.fill('\0');
  std::copy(source.begin(), source.end(), destination.begin());
  return true;
}

enum class StorageOperation : uint8_t {
  Read,
  Replace,
  Remove,
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

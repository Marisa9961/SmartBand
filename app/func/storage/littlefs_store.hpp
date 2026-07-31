/**
 * @file littlefs_store.hpp
 * @brief Single-owner LittleFS backend used by StorageTask.
 */

#ifndef BD_STORAGE_LITTLEFS_STORE_HPP
#define BD_STORAGE_LITTLEFS_STORE_HPP

#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>

#include <littlefs/lfs.h>

#include "func/storage/storage_types.hpp"

namespace bd::storage {
class LittleFsStore {
public:
  LittleFsStore() = default;
  LittleFsStore(const LittleFsStore &) = delete;
  LittleFsStore &operator=(const LittleFsStore &) = delete;

  StorageOutcome initialize();

  StorageOutcome read(std::string_view path, std::span<uint8_t> destination,
                      std::size_t &bytes_read);
  StorageOutcome replace(std::string_view path,
                         std::span<const uint8_t> source);
  StorageOutcome remove(std::string_view path);

private:
  [[nodiscard]] static StorageOutcome fromLittleFsError(int error);

  lfs_t filesystem_{};
  bool mounted_{false};
};
} // namespace bd::storage

#endif

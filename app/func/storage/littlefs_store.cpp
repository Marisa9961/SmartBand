/**
 * @file littlefs_store.cpp
 * @brief Single-owner LittleFS backend used by StorageTask.
 */

#include "func/storage/littlefs_store.hpp"

#include <algorithm>
#include <array>
#include <limits>

#include "porting/lfs_heap.h"
#include "porting/lfs_porting.h"

namespace bd::storage {
namespace {
constexpr std::string_view temporary_suffix{".tmp"};

std::array<char, max_path_length + temporary_suffix.size() + 1U>
temporaryPath(std::string_view path) {
  std::array<char, max_path_length + temporary_suffix.size() + 1U> result{};
  auto output{std::copy(path.begin(), path.end(), result.begin())};
  std::copy(temporary_suffix.begin(), temporary_suffix.end(), output);
  return result;
}
} // namespace

StorageOutcome LittleFsStore::initialize() {
  if (mounted_) {
    return {StorageStatus::Ok, LFS_ERR_OK};
  }

  if (!lfs_memory_init()) {
    return {StorageStatus::OutOfMemory, LFS_ERR_NOMEM};
  }

  int error{lfs_mount(&filesystem_, &lfs_config)};

  // A blank device and an unrecoverable filesystem both appear as corrupt.
  // Other failures (notably I/O) must never trigger an implicit erase.
  if (error == LFS_ERR_CORRUPT) {
    error = lfs_format(&filesystem_, &lfs_config);
    if (error == LFS_ERR_OK) {
      error = lfs_mount(&filesystem_, &lfs_config);
    }
  }

  mounted_ = error == LFS_ERR_OK;
  return fromLittleFsError(error);
}

StorageOutcome LittleFsStore::read(std::string_view path,
                                   std::span<uint8_t> destination,
                                   std::size_t &bytes_read) {
  bytes_read = 0U;
  if (!mounted_) {
    return {StorageStatus::NotReady, LFS_ERR_BADF};
  }
  StoragePath file_path{};
  if (!copyPath(path, file_path)) {
    return {StorageStatus::InvalidArgument, LFS_ERR_INVAL};
  }

  lfs_file_t file{};
  int error{lfs_file_open(&filesystem_, &file, file_path.data(),
                          LFS_O_RDONLY)};
  if (error != LFS_ERR_OK) {
    return fromLittleFsError(error);
  }

  const lfs_soff_t file_size{lfs_file_size(&filesystem_, &file)};
  if (file_size < 0) {
    error = static_cast<int>(file_size);
  } else if (static_cast<std::size_t>(file_size) > destination.size()) {
    error = LFS_ERR_FBIG;
  } else if (file_size > 0) {
    const lfs_ssize_t read_size{
        lfs_file_read(&filesystem_, &file, destination.data(),
                      static_cast<lfs_size_t>(file_size))};
    if (read_size < 0) {
      error = static_cast<int>(read_size);
    } else if (read_size != file_size) {
      error = LFS_ERR_IO;
    } else {
      bytes_read = static_cast<std::size_t>(read_size);
    }
  }

  const int close_error{lfs_file_close(&filesystem_, &file)};
  if (error == LFS_ERR_OK && close_error != LFS_ERR_OK) {
    error = close_error;
  }

  return fromLittleFsError(error);
}

StorageOutcome LittleFsStore::replace(std::string_view path,
                                      std::span<const uint8_t> source) {
  if (!mounted_) {
    return {StorageStatus::NotReady, LFS_ERR_BADF};
  }
  StoragePath file_path{};
  if (!copyPath(path, file_path)) {
    return {StorageStatus::InvalidArgument, LFS_ERR_INVAL};
  }
  if (source.size() > std::numeric_limits<lfs_size_t>::max()) {
    return {StorageStatus::TooLarge, LFS_ERR_FBIG};
  }

  const auto temporary_path{temporaryPath(path)};

  int error{lfs_remove(&filesystem_, temporary_path.data())};
  if (error != LFS_ERR_OK && error != LFS_ERR_NOENT) {
    return fromLittleFsError(error);
  }

  lfs_file_t file{};
  error = lfs_file_open(&filesystem_, &file, temporary_path.data(),
                        LFS_O_WRONLY | LFS_O_CREAT | LFS_O_TRUNC);
  if (error != LFS_ERR_OK) {
    return fromLittleFsError(error);
  }

  if (!source.empty()) {
    const lfs_ssize_t written{
        lfs_file_write(&filesystem_, &file, source.data(),
                       static_cast<lfs_size_t>(source.size()))};
    if (written < 0) {
      error = static_cast<int>(written);
    } else if (static_cast<std::size_t>(written) != source.size()) {
      error = LFS_ERR_IO;
    }
  }

  if (error == LFS_ERR_OK) {
    error = lfs_file_sync(&filesystem_, &file);
  }

  const int close_error{lfs_file_close(&filesystem_, &file)};
  if (error == LFS_ERR_OK && close_error != LFS_ERR_OK) {
    error = close_error;
  }

  if (error == LFS_ERR_OK) {
    error = lfs_rename(&filesystem_, temporary_path.data(), file_path.data());
  }

  if (error != LFS_ERR_OK) {
    static_cast<void>(lfs_remove(&filesystem_, temporary_path.data()));
  }

  return fromLittleFsError(error);
}

StorageOutcome LittleFsStore::remove(std::string_view path) {
  if (!mounted_) {
    return {StorageStatus::NotReady, LFS_ERR_BADF};
  }
  StoragePath file_path{};
  if (!copyPath(path, file_path)) {
    return {StorageStatus::InvalidArgument, LFS_ERR_INVAL};
  }

  return fromLittleFsError(lfs_remove(&filesystem_, file_path.data()));
}

StorageOutcome LittleFsStore::fromLittleFsError(int error) {
  switch (error) {
  case LFS_ERR_OK:
    return {StorageStatus::Ok, error};
  case LFS_ERR_NOENT:
    return {StorageStatus::NotFound, error};
  case LFS_ERR_EXIST:
    return {StorageStatus::AlreadyExists, error};
  case LFS_ERR_NOTEMPTY:
    return {StorageStatus::NotEmpty, error};
  case LFS_ERR_FBIG:
  case LFS_ERR_NAMETOOLONG:
    return {StorageStatus::TooLarge, error};
  case LFS_ERR_INVAL:
  case LFS_ERR_BADF:
  case LFS_ERR_NOTDIR:
  case LFS_ERR_ISDIR:
    return {StorageStatus::InvalidArgument, error};
  case LFS_ERR_NOSPC:
    return {StorageStatus::NoSpace, error};
  case LFS_ERR_NOMEM:
    return {StorageStatus::OutOfMemory, error};
  case LFS_ERR_IO:
    return {StorageStatus::IoError, error};
  case LFS_ERR_CORRUPT:
    return {StorageStatus::Corrupt, error};
  default:
    return {StorageStatus::InternalError, error};
  }
}

} // namespace bd::storage

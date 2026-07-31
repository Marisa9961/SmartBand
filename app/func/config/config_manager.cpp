/**
 * @file config_manager.cpp
 * @brief Runtime owner for loading and persisting DeviceConfig.
 */

#include "func/config/config_manager.hpp"

#include <array>
#include <span>
#include <string_view>

#include "func/config/config_codec.hpp"
#include "tasks/storage.hpp"

namespace bd::config {
namespace {
constexpr std::string_view config_path{"device.cfg"};
constexpr uint32_t retry_delay_ticks{1000U};

bool deadlineReached(uint32_t now, uint32_t deadline) {
  return static_cast<int32_t>(now - deadline) >= 0;
}
} // namespace

bool ConfigManager::initialize(uint32_t load_timeout) {
  if (response_queue_ != nullptr) {
    return false;
  }

  response_queue_ = task::createStorageResponseQueue();
  if (response_queue_ == nullptr) {
    return false;
  }

  if (!startLoad()) {
    scheduleRetry();
    return true;
  }

  storage::StorageResponse response{};
  if (osMessageQueueGet(response_queue_, &response, nullptr, load_timeout) ==
      osOK) {
    handleResponse(response, false);
  }

  return true;
}

void ConfigManager::process() {
  storage::StorageResponse response{};
  if (osMessageQueueGet(response_queue_, &response, nullptr, 0U) == osOK) {
    handleResponse(response, true);
  }

  const uint32_t now{osKernelGetTickCount()};
  if (!deadlineReached(now, retry_after_)) {
    return;
  }

  switch (io_state_) {
  case IoState::NeedLoad:
    if (!startLoad()) {
      scheduleRetry();
    }
    break;
  case IoState::Idle:
    if (dirty_ && !startSave()) {
      scheduleRetry();
    }
    break;
  case IoState::Loading:
  case IoState::Saving:
    break;
  }
}

bool ConfigManager::setBrightnessPercent(uint8_t brightness_percent) {
  if (!isValidBrightness(brightness_percent) ||
      config_.display.brightness_percent == brightness_percent) {
    return false;
  }

  config_.display.brightness_percent = brightness_percent;
  user_modified_ = true;
  dirty_ = true;
  retry_after_ = 0U;
  return true;
}

bool ConfigManager::setScreenTimeoutMs(uint32_t timeout_ms) {
  if (!isValidScreenTimeout(timeout_ms) ||
      config_.display.screen_timeout_ms == timeout_ms) {
    return false;
  }

  config_.display.screen_timeout_ms = timeout_ms;
  user_modified_ = true;
  dirty_ = true;
  retry_after_ = 0U;
  return true;
}

bool ConfigManager::takeConfigurationUpdate() {
  const bool updated{configuration_update_};
  configuration_update_ = false;
  return updated;
}

bool ConfigManager::startLoad() {
  if (task::requestStorageRead(config_path, response_queue_) != osOK) {
    io_state_ = IoState::NeedLoad;
    return false;
  }

  io_state_ = IoState::Loading;
  return true;
}

bool ConfigManager::startSave() {
  std::array<uint8_t, storage::payload_capacity> encoded{};
  const std::size_t encoded_size{encodeDeviceConfig(config_, encoded)};
  if (encoded_size == 0U ||
      task::requestStorageWrite(
          config_path,
          std::span<const uint8_t>{encoded.data(), encoded_size},
          response_queue_) != osOK) {
    io_state_ = IoState::Idle;
    return false;
  }

  dirty_ = false;
  io_state_ = IoState::Saving;
  return true;
}

void ConfigManager::handleResponse(const storage::StorageResponse &response,
                                   bool notify_change) {
  switch (response.operation) {
  case storage::StorageOperation::Read:
    handleLoadResponse(response, notify_change);
    break;

  case storage::StorageOperation::Write:
    io_state_ = IoState::Idle;
    if (response.status != storage::StorageStatus::Ok) {
      dirty_ = true;
      scheduleRetry();
    }
    break;

  case storage::StorageOperation::Remove:
  case storage::StorageOperation::MakeConsistent:
  case storage::StorageOperation::GarbageCollect:
    break;
  }
}

void ConfigManager::handleLoadResponse(
    const storage::StorageResponse &response, bool notify_change) {
  if (response.status == storage::StorageStatus::Ok) {
    DeviceConfig loaded{};
    const ConfigDecodeStatus decode_status{
        response.size <= response.data.size()
            ? decodeDeviceConfig(
                  std::span<const uint8_t>{response.data.data(), response.size},
                  loaded)
            : ConfigDecodeStatus::Invalid};

    io_state_ = IoState::Idle;
    if (!user_modified_) {
      switch (decode_status) {
      case ConfigDecodeStatus::Valid:
        config_ = loaded;
        break;
      case ConfigDecodeStatus::NeedsRewrite:
        config_ = loaded;
        dirty_ = true;
        break;
      case ConfigDecodeStatus::Invalid:
        config_ = DeviceConfig{};
        dirty_ = true;
        break;
      case ConfigDecodeStatus::UnsupportedVersion:
        config_ = DeviceConfig{};
        break;
      }

      configuration_update_ = notify_change;
    } else {
      // User changes made while storage was loading always win.
      dirty_ = true;
    }
    return;
  }

  if (response.status == storage::StorageStatus::NotFound) {
    io_state_ = IoState::Idle;
    dirty_ = true;
    if (!user_modified_) {
      config_ = DeviceConfig{};
      configuration_update_ = notify_change;
    }
    return;
  }

  if (response.status == storage::StorageStatus::TooLarge) {
    // Treat a larger record as a possible newer firmware format. Run with
    // defaults instead of repeatedly reading or automatically erasing it.
    io_state_ = IoState::Idle;
    if (!user_modified_) {
      config_ = DeviceConfig{};
      configuration_update_ = notify_change;
    }
    return;
  }

  io_state_ = IoState::NeedLoad;
  scheduleRetry();
}

void ConfigManager::scheduleRetry() {
  retry_after_ = osKernelGetTickCount() + retry_delay_ticks;
}
} // namespace bd::config

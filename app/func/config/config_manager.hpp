/**
 * @file config_manager.hpp
 * @brief Runtime owner for loading and persisting DeviceConfig.
 */

#ifndef BD_CONFIG_CONFIG_MANAGER_HPP
#define BD_CONFIG_CONFIG_MANAGER_HPP

#include <cstdint>

#include <cmsis_os2.h>

#include "func/config/device_config.hpp"
#include "func/storage/storage_types.hpp"

namespace bd::config {
class ConfigManager {
public:
  ConfigManager() = default;
  ConfigManager(const ConfigManager &) = delete;
  ConfigManager &operator=(const ConfigManager &) = delete;

  bool initialize(uint32_t load_timeout);
  void process();

  [[nodiscard]] constexpr const DeviceConfig &current() const {
    return config_;
  }

  bool setBrightnessPercent(uint8_t brightness_percent);
  bool setScreenTimeoutMs(uint32_t timeout_ms);

  /**
   * Returns true once when a delayed startup read replaced the live defaults.
   */
  bool takeConfigurationUpdate();

private:
  enum class IoState : uint8_t {
    NeedLoad,
    Loading,
    Idle,
    Saving,
  };

  bool startLoad();
  bool startSave();
  void handleResponse(const storage::StorageResponse &response,
                      bool notify_change);
  void handleLoadResponse(const storage::StorageResponse &response,
                          bool notify_change);
  void scheduleRetry();

  osMessageQueueId_t response_queue_{nullptr};
  DeviceConfig config_{};
  IoState io_state_{IoState::NeedLoad};
  uint32_t retry_after_{0U};
  bool dirty_{false};
  bool user_modified_{false};
  bool configuration_update_{false};
};
} // namespace bd::config

#endif

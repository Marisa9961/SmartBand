/**
 * @file device_config.hpp
 * @brief Application configuration model and validation rules.
 */

#ifndef BD_CONFIG_DEVICE_CONFIG_HPP
#define BD_CONFIG_DEVICE_CONFIG_HPP

#include <array>
#include <cstdint>

namespace bd::config {
inline constexpr uint8_t minimum_brightness_percent{5U};
inline constexpr uint8_t maximum_brightness_percent{100U};
inline constexpr uint8_t default_brightness_percent{50U};

inline constexpr std::array<uint32_t, 3U> screen_timeout_options_ms{
    3000U, 5000U, 10000U};
inline constexpr uint32_t default_screen_timeout_ms{
    screen_timeout_options_ms[1U]};

struct DisplaySettings {
  uint32_t screen_timeout_ms{default_screen_timeout_ms};
  uint8_t brightness_percent{default_brightness_percent};

  friend constexpr bool operator==(const DisplaySettings &,
                                   const DisplaySettings &) = default;
};

struct DeviceConfig {
  DisplaySettings display{};

  friend constexpr bool operator==(const DeviceConfig &,
                                   const DeviceConfig &) = default;
};

[[nodiscard]] constexpr bool isValidScreenTimeout(uint32_t timeout_ms) {
  for (const uint32_t option : screen_timeout_options_ms) {
    if (timeout_ms == option) {
      return true;
    }
  }
  return false;
}

[[nodiscard]] constexpr bool isValidBrightness(uint8_t brightness_percent) {
  return brightness_percent >= minimum_brightness_percent &&
         brightness_percent <= maximum_brightness_percent;
}

[[nodiscard]] constexpr DisplaySettings
normalized(DisplaySettings settings) {
  if (!isValidScreenTimeout(settings.screen_timeout_ms)) {
    settings.screen_timeout_ms = default_screen_timeout_ms;
  }
  if (!isValidBrightness(settings.brightness_percent)) {
    settings.brightness_percent = default_brightness_percent;
  }
  return settings;
}
} // namespace bd::config

#endif

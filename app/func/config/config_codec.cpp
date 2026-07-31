/**
 * @file config_codec.cpp
 * @brief Stable on-flash encoding for DeviceConfig.
 */

#include "func/config/config_codec.hpp"

#include <algorithm>
#include <array>

namespace bd::config {
namespace {
constexpr std::array<uint8_t, 4U> config_magic{'S', 'B', 'C', 'F'};
constexpr uint16_t format_version{1U};
constexpr std::size_t header_size{12U};
constexpr std::size_t field_header_size{4U};

enum class ConfigField : uint16_t {
  DisplayScreenTimeoutMs = 0x0101U,
  DisplayBrightnessPercent = 0x0102U,
};

uint16_t readUint16(std::span<const uint8_t> source, std::size_t offset) {
  return static_cast<uint16_t>(
      static_cast<uint16_t>(source[offset]) |
      static_cast<uint16_t>(static_cast<uint16_t>(source[offset + 1U]) << 8U));
}

uint32_t readUint32(std::span<const uint8_t> source, std::size_t offset) {
  return static_cast<uint32_t>(source[offset]) |
         (static_cast<uint32_t>(source[offset + 1U]) << 8U) |
         (static_cast<uint32_t>(source[offset + 2U]) << 16U) |
         (static_cast<uint32_t>(source[offset + 3U]) << 24U);
}

void writeUint16(std::span<uint8_t> destination, std::size_t offset,
                 uint16_t value) {
  destination[offset] = static_cast<uint8_t>(value);
  destination[offset + 1U] = static_cast<uint8_t>(value >> 8U);
}

void writeUint32(std::span<uint8_t> destination, std::size_t offset,
                 uint32_t value) {
  destination[offset] = static_cast<uint8_t>(value);
  destination[offset + 1U] = static_cast<uint8_t>(value >> 8U);
  destination[offset + 2U] = static_cast<uint8_t>(value >> 16U);
  destination[offset + 3U] = static_cast<uint8_t>(value >> 24U);
}

uint32_t crc32(std::span<const uint8_t> data) {
  uint32_t crc{0xFFFFFFFFU};
  for (const uint8_t byte : data) {
    crc ^= byte;
    for (uint8_t bit{0U}; bit < 8U; ++bit) {
      const uint32_t mask{0U - (crc & 1U)};
      crc = (crc >> 1U) ^ (0xEDB88320U & mask);
    }
  }
  return ~crc;
}

bool appendFieldHeader(std::span<uint8_t> destination, std::size_t &cursor,
                       ConfigField field, uint16_t field_size) {
  if (destination.size() - cursor <
      field_header_size + static_cast<std::size_t>(field_size)) {
    return false;
  }

  writeUint16(destination, cursor, static_cast<uint16_t>(field));
  writeUint16(destination, cursor + 2U, field_size);
  cursor += field_header_size;
  return true;
}
} // namespace

std::size_t encodeDeviceConfig(const DeviceConfig &config,
                               std::span<uint8_t> destination) {
  if (destination.size() < header_size) {
    return 0U;
  }

  std::fill(destination.begin(), destination.end(), 0U);
  std::copy(config_magic.begin(), config_magic.end(), destination.begin());
  writeUint16(destination, 4U, format_version);

  const DisplaySettings display{normalized(config.display)};
  std::size_t cursor{header_size};

  if (!appendFieldHeader(destination, cursor,
                         ConfigField::DisplayScreenTimeoutMs, 4U)) {
    return 0U;
  }
  writeUint32(destination, cursor, display.screen_timeout_ms);
  cursor += 4U;

  if (!appendFieldHeader(destination, cursor,
                         ConfigField::DisplayBrightnessPercent, 1U)) {
    return 0U;
  }
  destination[cursor++] = display.brightness_percent;

  const auto payload_size{
      static_cast<uint16_t>(cursor - header_size)};
  writeUint16(destination, 6U, payload_size);
  writeUint32(destination, 8U,
              crc32(destination.subspan(header_size, payload_size)));
  return cursor;
}

ConfigDecodeStatus decodeDeviceConfig(std::span<const uint8_t> source,
                                      DeviceConfig &config) {
  config = DeviceConfig{};
  if (source.size() < header_size ||
      !std::equal(config_magic.begin(), config_magic.end(), source.begin())) {
    return ConfigDecodeStatus::Invalid;
  }

  if (readUint16(source, 4U) != format_version) {
    return ConfigDecodeStatus::UnsupportedVersion;
  }

  const uint16_t payload_size{readUint16(source, 6U)};
  if (source.size() != header_size + payload_size) {
    return ConfigDecodeStatus::Invalid;
  }

  const auto payload{source.subspan(header_size, payload_size)};
  if (readUint32(source, 8U) != crc32(payload)) {
    return ConfigDecodeStatus::Invalid;
  }

  bool needs_rewrite{false};
  bool timeout_seen{false};
  bool brightness_seen{false};
  std::size_t cursor{0U};

  while (cursor < payload.size()) {
    if (payload.size() - cursor < field_header_size) {
      return ConfigDecodeStatus::Invalid;
    }

    const auto field{
        static_cast<ConfigField>(readUint16(payload, cursor))};
    const uint16_t field_size{readUint16(payload, cursor + 2U)};
    cursor += field_header_size;
    if (payload.size() - cursor < field_size) {
      return ConfigDecodeStatus::Invalid;
    }

    switch (field) {
    case ConfigField::DisplayScreenTimeoutMs:
      if (timeout_seen || field_size != 4U) {
        needs_rewrite = true;
      } else {
        timeout_seen = true;
        const uint32_t timeout{readUint32(payload, cursor)};
        if (isValidScreenTimeout(timeout)) {
          config.display.screen_timeout_ms = timeout;
        } else {
          needs_rewrite = true;
        }
      }
      break;

    case ConfigField::DisplayBrightnessPercent:
      if (brightness_seen || field_size != 1U) {
        needs_rewrite = true;
      } else {
        brightness_seen = true;
        const uint8_t brightness{payload[cursor]};
        if (isValidBrightness(brightness)) {
          config.display.brightness_percent = brightness;
        } else {
          needs_rewrite = true;
        }
      }
      break;

    default:
      // Unknown TLVs are intentionally skipped for forward extensibility.
      break;
    }

    cursor += field_size;
  }

  if (!timeout_seen || !brightness_seen) {
    needs_rewrite = true;
  }

  config.display = normalized(config.display);
  return needs_rewrite ? ConfigDecodeStatus::NeedsRewrite
                       : ConfigDecodeStatus::Valid;
}
} // namespace bd::config

/**
 * @file config_codec.hpp
 * @brief Stable on-flash encoding for DeviceConfig.
 */

#ifndef BD_CONFIG_CONFIG_CODEC_HPP
#define BD_CONFIG_CONFIG_CODEC_HPP

#include <cstddef>
#include <cstdint>
#include <span>

#include "func/config/device_config.hpp"

namespace bd::config {
/**
 * All integers are little-endian. On-flash layout:
 *   magic[4] | format_version:u16 | payload_size:u16 | crc32:u32 | TLVs...
 *
 * Each TLV is field_id:u16 | field_size:u16 | field bytes. New compatible
 * fields may be appended without moving existing fields. Incompatible format
 * changes must increment format_version and add an explicit migration decoder.
 */
enum class ConfigDecodeStatus : uint8_t {
  Valid,
  NeedsRewrite,
  Invalid,
  UnsupportedVersion,
};

[[nodiscard]] std::size_t
encodeDeviceConfig(const DeviceConfig &config,
                   std::span<uint8_t> destination);

[[nodiscard]] ConfigDecodeStatus
decodeDeviceConfig(std::span<const uint8_t> source, DeviceConfig &config);
} // namespace bd::config

#endif

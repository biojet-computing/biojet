#pragma once

#include <cstdint>
#include <string_view>
#include <utility>


namespace biojet
{
enum class status_code : std::uint8_t
{
  success                  = 0x00, ///< operation completed successfully
  connection_refused       = 0x01, ///< connection refused: sensor could not connect to host
  finger_not_detected      = 0x02, ///< no finger detected on sensor
  capture_failed           = 0x03, ///< failed to capture fingerprint image
  finger_too_dry           = 0x04, ///< capture failed: finger too dry, image too faint
  finger_too_wet           = 0x05, ///< capture failed: finger too wet, image blurred
  finger_too_dirty         = 0x06, ///< capture failed: finger too dirty, image distorted
  insufficient_features    = 0x07, ///< template generation failed: image too small or lacking features
  no_match_found           = 0x08, ///< finger does not match
  finger_not_found         = 0x09, ///< finger not found in database
  enrollment_mismatch      = 0x0a, ///< enrollment failed: mismatch during capture
  index_out_of_range       = 0x0b, ///< index is out of range.
  storage_access_failure   = 0x0c, ///< failed to access or read database
  template_upload_failed   = 0x0d, ///< failed to upload fingerprint template
  device_busy              = 0x0e, ///< sensor is busy and cannot accept new requests
  image_upload_failed      = 0x0f, ///< failed to upload fingerprint image
  template_deletion_failed = 0x10, ///< failed to delete template
  database_clear_failed    = 0x11, ///< failed to clear database
  cannot_enter_low_power   = 0x12, ///< unable to switch to low-power mode
  bad_password             = 0x13, ///< password required before use
  bad_image_format         = 0x15, ///< template generation failed: invalid image format
  flash_error              = 0x18, ///< flash write error
  unknown_error            = 0x19, ///< unknown error
  bad_register             = 0x1a, ///< invalid device register
  bad_device_configuration = 0x1b, ///< invalid device configuration
  port_error               = 0x1d, ///< port or serial communication error
  no_space_left            = 0x1f, ///< enrollment failed: database full
  bad_address              = 0x20, ///< invalid device address
  device_lock_out          = 0x21, ///< password verification required before use
  hardware_error           = 0x29, ///< hardware malfunction detected
  bad_packet               = 0xfe, ///< invalid or malformed packet sent
  timeout                  = 0xff  ///< operation timed out
};

namespace internal
{
///////////////////////////////////////////////////////////////////////
/// @brief Converts error code to snake_case name
/// @param value Error code
/// @return Snake_case identifier name
///////////////////////////////////////////////////////////////////////
constexpr std::string_view message(status_code value) noexcept
{
  using namespace std::literals;
  switch (value)
  {
    case status_code::success:
      return "operation completed successfully"sv;
    case status_code::connection_refused:
      return "connection refused: sensor could not connect to host"sv;
    case status_code::finger_not_detected:
      return "no finger detected on sensor"sv;
    case status_code::capture_failed:
      return "failed to capture fingerprint image"sv;
    case status_code::finger_too_dry:
      return "image is too faint to extract features"sv;
    case status_code::finger_too_wet:
      return "image is too blurry to extract features"sv;
    case status_code::finger_too_dirty:
      return "image is too distorted to extract features"sv;
    case status_code::insufficient_features:
      return "image has too few features to generate template"sv;
    case status_code::no_match_found:
      return "finger does not match"sv;
    case status_code::finger_not_found:
      return "finger is not found in data store"sv;
    case status_code::enrollment_mismatch:
      return "enrollment failed: mismatch during capture"sv;
    case status_code::index_out_of_range:
      return "index is out of range"sv;
    case status_code::storage_access_failure:
      return "failed to access or read database"sv;
    case status_code::template_upload_failed:
      return "failed to upload template"sv;
    case status_code::device_busy:
      return "sensor is busy and cannot accept new requests"sv;
    case status_code::image_upload_failed:
      return "failed to upload fingerprint image"sv;
    case status_code::template_deletion_failed:
      return "failed to delete template"sv;
    case status_code::database_clear_failed:
      return "failed to clear database"sv;
    case status_code::cannot_enter_low_power:
      return "unable to switch to low-power mode"sv;
    case status_code::bad_password:
      return "password required before use"sv;
    case status_code::bad_image_format:
      return "template generation failed: invalid image format"sv;
    case status_code::flash_error:
      return "flash write error"sv;
    case status_code::bad_register:
      return "invalid device register"sv;
    case status_code::bad_device_configuration:
      return "invalid device configuration"sv;
    case status_code::port_error:
      return "port or serial communication error"sv;
    case status_code::no_space_left:
      return "enrollment failed: database full"sv;
    case status_code::bad_address:
      return "invalid device address"sv;
    case status_code::device_lock_out:
      return "password verification required before use"sv;
    case status_code::hardware_error:
      return "hardware malfunction detected"sv;
    case status_code::bad_packet:
      return "invalid or malformed packet sent"sv;
    case status_code::timeout:
      return "operation timed out"sv;
    default:
    case status_code::unknown_error:
      return "unknown error"sv;
  }
}

///////////////////////////////////////////////////////////////////////
/// @brief Converts error code to snake_case name
/// @param value Error code
/// @return Snake_case identifier name
///////////////////////////////////////////////////////////////////////
constexpr std::string_view name(status_code value) noexcept
{
  using namespace std::literals;
  switch (value)
  {
    case status_code::success:
      return "success"sv;
    case status_code::connection_refused:
      return "connection_refused"sv;
    case status_code::finger_not_detected:
      return "finger_not_detected"sv;
    case status_code::capture_failed:
      return "capture_failed"sv;
    case status_code::finger_too_dry:
      return "finger_too_dry"sv;
    case status_code::finger_too_wet:
      return "finger_too_wet"sv;
    case status_code::finger_too_dirty:
      return "finger_too_dirty"sv;
    case status_code::insufficient_features:
      return "insufficient_features"sv;
    case status_code::no_match_found:
      return "no_match_found"sv;
    case status_code::finger_not_found:
      return "finger_not_found"sv;
    case status_code::enrollment_mismatch:
      return "enrollment_mismatch"sv;
    case status_code::index_out_of_range:
      return "index_out_of_range"sv;
    case status_code::storage_access_failure:
      return "storage_access_failure"sv;
    case status_code::template_upload_failed:
      return "template_upload_failed"sv;
    case status_code::device_busy:
      return "device_busy"sv;
    case status_code::image_upload_failed:
      return "image_upload_failed";
    case status_code::template_deletion_failed:
      return "template_deletion_failed"sv;
    case status_code::database_clear_failed:
      return "database_clear_failed"sv;
    case status_code::cannot_enter_low_power:
      return "cannot_enter_low_power"sv;
    case status_code::bad_password:
      return "bad_password"sv;
    case status_code::bad_image_format:
      return "bad_image_format"sv;
    case status_code::flash_error:
      return "flash_error"sv;
    case status_code::bad_register:
      return "bad_register"sv;
    case status_code::bad_device_configuration:
      return "bad_device_configuration"sv;
    case status_code::port_error:
      return "port_error"sv;
    case status_code::no_space_left:
      return "no_space_left"sv;
    case status_code::bad_address:
      return "bad_address"sv;
    case status_code::device_lock_out:
      return "device_lock_out"sv;
    case status_code::hardware_error:
      return "hardware_error"sv;
    case status_code::bad_packet:
      return "bad_packet"sv;
    case status_code::timeout:
      return "timeout"sv;
    default:
    case status_code::unknown_error:
      return "unknown_error"sv;
  }
}

constexpr status_code to_status_code(std::uint8_t v) noexcept
{
  switch (v)
  {
    case 0x00:
      return status_code::success;
    case 0x01:
      return status_code::connection_refused;
    case 0x02:
      return status_code::finger_not_detected;
    case 0x03:
      return status_code::capture_failed;
    case 0x04:
      return status_code::finger_too_dry;
    case 0x05:
      return status_code::finger_too_wet;
    case 0x06:
      return status_code::finger_too_dirty;
    case 0x07:
      return status_code::insufficient_features;
    case 0x08:
      return status_code::no_match_found;
    case 0x09:
      return status_code::finger_not_found;
    case 0x0A:
      return status_code::enrollment_mismatch;
    case 0x0B:
      return status_code::index_out_of_range;
    case 0x0C:
      return status_code::storage_access_failure;
    case 0x0D:
      return status_code::template_upload_failed;
    case 0x0E:
      return status_code::device_busy;
    case 0x0F:
      return status_code::image_upload_failed;
    case 0x10:
      return status_code::template_deletion_failed;
    case 0x11:
      return status_code::database_clear_failed;
    case 0x12:
      return status_code::cannot_enter_low_power;
    case 0x13:
      return status_code::bad_password;
    case 0x15:
      return status_code::bad_image_format;
    case 0x18:
      return status_code::flash_error;
    case 0x1A:
      return status_code::bad_register;
    case 0x1B:
      return status_code::bad_device_configuration;
    case 0x1D:
      return status_code::port_error;
    case 0x1F:
      return status_code::no_space_left;
    case 0x20:
      return status_code::bad_address;
    case 0x21:
      return status_code::device_lock_out;
    case 0x29:
      return status_code::hardware_error;
    case 0xfe:
      return status_code::bad_packet;
    case 0xff:
      return status_code::timeout;
    case 0x19:
    default:
      return status_code::unknown_error;
  }
}
} // namespace internal

constexpr std::string_view name(status_code v) noexcept
{
  return internal::name(v);
}

constexpr std::string_view message(status_code v) noexcept
{
  return internal::message(v);
}

constexpr status_code to_status_code(std::uint8_t v) noexcept
{
  return internal::to_status_code(v);
}

inline constexpr std::uint8_t to_byte(status_code v) noexcept
{
  return std::to_underlying(v);
}

constexpr bool is_success(status_code v) noexcept
{
  return status_code::success == v;
}

constexpr bool is_error(status_code v) noexcept
{
  return !is_success(v);
}
} // namespace biojet

#pragma once

#include <cstdint>
#include <string_view>
#include <utility>

namespace biojet
{
enum class errc : std::uint8_t
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
constexpr std::string_view
message(errc code) noexcept
{
  switch (code)
  {
    case errc::success:
      return "operation completed successfully";
    case errc::connection_refused:
      return "connection refused: sensor could not connect to host";
    case errc::finger_not_detected:
      return "no finger detected on sensor";
    case errc::capture_failed:
      return "failed to capture fingerprint image";
    case errc::finger_too_dry:
      return "image is too faint to extract features";
    case errc::finger_too_wet:
      return "image is too blurry to extract features";
    case errc::finger_too_dirty:
      return "image is too distorted to extract features";
    case errc::insufficient_features:
      return "image has too few features to generate template";
    case errc::no_match_found:
      return "finger does not match";
    case errc::finger_not_found:
      return "finger is not found in data store";
    case errc::enrollment_mismatch:
      return "enrollment failed: mismatch during capture";
    case errc::index_out_of_range:
      return "index is out of range";
    case errc::storage_access_failure:
      return "failed to access or read database";
    case errc::template_upload_failed:
      return "failed to upload template";
    case errc::device_busy:
      return "sensor is busy and cannot accept new requests";
    case errc::image_upload_failed:
      return "failed to upload fingerprint image";
    case errc::template_deletion_failed:
      return "failed to delete template";
    case errc::database_clear_failed:
      return "failed to clear database";
    case errc::cannot_enter_low_power:
      return "unable to switch to low-power mode";
    case errc::bad_password:
      return "password required before use";
    case errc::bad_image_format:
      return "template generation failed: invalid image format";
    case errc::flash_error:
      return "flash write error";
    case errc::bad_register:
      return "invalid device register";
    case errc::bad_device_configuration:
      return "invalid device configuration";
    case errc::port_error:
      return "port or serial communication error";
    case errc::no_space_left:
      return "enrollment failed: database full";
    case errc::bad_address:
      return "invalid device address";
    case errc::device_lock_out:
      return "password verification required before use";
    case errc::hardware_error:
      return "hardware malfunction detected";
    case errc::bad_packet:
      return "invalid or malformed packet sent";
    case errc::timeout:
      return "operation timed out";
    default:
    case errc::unknown_error:
      return "unknown error";
  }
}

///////////////////////////////////////////////////////////////////////
/// @brief Converts error code to snake_case name
/// @param code Error code
/// @return Snake_case identifier name
///////////////////////////////////////////////////////////////////////
constexpr std::string_view
name(errc code) noexcept
{
  switch (code)
  {
    case errc::success:
      return "success";
    case errc::connection_refused:
      return "connection_refused";
    case errc::finger_not_detected:
      return "finger_not_detected";
    case errc::capture_failed:
      return "capture_failed";
    case errc::finger_too_dry:
      return "finger_too_dry";
    case errc::finger_too_wet:
      return "finger_too_wet";
    case errc::finger_too_dirty:
      return "finger_too_dirty";
    case errc::insufficient_features:
      return "insufficient_features";
    case errc::no_match_found:
      return "no_match_found";
    case errc::finger_not_found:
      return "finger_not_found";
    case errc::enrollment_mismatch:
      return "enrollment_mismatch";
    case errc::index_out_of_range:
      return "index_out_of_range";
    case errc::storage_access_failure:
      return "storage_access_failure";
    case errc::template_upload_failed:
      return "template_upload_failed";
    case errc::device_busy:
      return "device_busy";
    case errc::image_upload_failed:
      return "image_upload_failed";
    case errc::template_deletion_failed:
      return "template_deletion_failed";
    case errc::database_clear_failed:
      return "database_clear_failed";
    case errc::cannot_enter_low_power:
      return "cannot_enter_low_power";
    case errc::bad_password:
      return "bad_password";
    case errc::bad_image_format:
      return "bad_image_format";
    case errc::flash_error:
      return "flash_error";
    case errc::bad_register:
      return "bad_register";
    case errc::bad_device_configuration:
      return "bad_device_configuration";
    case errc::port_error:
      return "port_error";
    case errc::no_space_left:
      return "no_space_left";
    case errc::bad_address:
      return "bad_address";
    case errc::device_lock_out:
      return "device_lock_out";
    case errc::hardware_error:
      return "hardware_error";
    case errc::bad_packet:
      return "bad_packet";
    case errc::timeout:
      return "timeout";
    default:
    case errc::unknown_error:
      return "unknown_error";
  }
}

constexpr errc
to_errc(std::uint8_t byte) noexcept
{
  switch (byte)
  {
    case 0x00:
      return errc::success;
    case 0x01:
      return errc::connection_refused;
    case 0x02:
      return errc::finger_not_detected;
    case 0x03:
      return errc::capture_failed;
    case 0x04:
      return errc::finger_too_dry;
    case 0x05:
      return errc::finger_too_wet;
    case 0x06:
      return errc::finger_too_dirty;
    case 0x07:
      return errc::insufficient_features;
    case 0x08:
      return errc::no_match_found;
    case 0x09:
      return errc::finger_not_found;
    case 0x0A:
      return errc::enrollment_mismatch;
    case 0x0B:
      return errc::index_out_of_range;
    case 0x0C:
      return errc::storage_access_failure;
    case 0x0D:
      return errc::template_upload_failed;
    case 0x0E:
      return errc::device_busy;
    case 0x0F:
      return errc::image_upload_failed;
    case 0x10:
      return errc::template_deletion_failed;
    case 0x11:
      return errc::database_clear_failed;
    case 0x12:
      return errc::cannot_enter_low_power;
    case 0x13:
      return errc::bad_password;
    case 0x15:
      return errc::bad_image_format;
    case 0x18:
      return errc::flash_error;
    case 0x1A:
      return errc::bad_register;
    case 0x1B:
      return errc::bad_device_configuration;
    case 0x1D:
      return errc::port_error;
    case 0x1F:
      return errc::no_space_left;
    case 0x20:
      return errc::bad_address;
    case 0x21:
      return errc::device_lock_out;
    case 0x29:
      return errc::hardware_error;
    case 0xfe:
      return errc::bad_packet;
    case 0xff:
      return errc::timeout;
    case 0x19:
    default:
      return errc::unknown_error;
  }
}
} // namespace internal

constexpr std::string_view
name(errc e) noexcept
{
  return internal::name(e);
}

constexpr std::string_view
message(errc e) noexcept
{
  return internal::message(e);
}

constexpr errc
to_errc(std::uint8_t b) noexcept
{
  return internal::to_errc(b);
}

inline constexpr std::uint8_t
to_byte(errc e) noexcept
{
  return std::to_underlying(e);
}

constexpr bool
is_success(errc code) noexcept
{
  return code == errc::success;
}

constexpr bool
is_error(errc code) noexcept
{
  return !is_success(code);
}
} /* namespace biojet */
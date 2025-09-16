#pragma once

#include <cstdint>
#include <string_view>

namespace biojet::io
{
enum class data_bits : std::uint8_t
{
  _5 = 0x05,
  _6 = 0x06,
  _7 = 0x07,
  _8 = 0x08,
};

enum class stop_bits : std::uint8_t
{
  _1 = 0x01,
  _2 = 0x02,
};

enum class parity_mode : std::uint8_t
{
  none = 0x01,
  odd  = 0x02,
  even = 0x03,
};

enum class flow_control : std::uint8_t
{
  none     = 0x00,
  software = 0x01,
  hardware = 0x02,
};

struct serial_configuration
{
  std::string_view path{"/dev/ttyAMA0"};
  std::uint32_t    baud{57600};
  data_bits        bits{data_bits::_8};
  stop_bits        stop{stop_bits::_1};
  parity_mode      parity{parity_mode::none};
  flow_control     flow{flow_control::none};
  std::uint32_t    write_timeout_ms{1000};
  std::uint32_t    read_timeout_ms{1000};
};
} // namespace biojet::io
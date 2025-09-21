#pragma once

#include "biojet/result.hpp"
#include "biojet/transport.hpp"

#include <experimental/propagate_const>

#include <future>
#include <memory>
#include <span>
#include <thread>

namespace biojet
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

class serial_port
{
  class impl;
  std::experimental::propagate_const<std::unique_ptr<impl>> impl_;

public:
  serial_port() noexcept;
  explicit serial_port(serial_configuration config) noexcept;
  ~serial_port() noexcept;

  result<bool>                     open() noexcept;
  result<bool>                     open(serial_configuration config) noexcept;
  void                             close() noexcept;
  bool                             is_open() const noexcept;
  result<std::size_t>              send(const std::span<const std::uint8_t> &buffer) noexcept;
  result<std::size_t>              recv(std::span<std::uint8_t> &buffer) noexcept;
  std::future<result<std::size_t>> send_async(const std::span<const std::uint8_t> &buffer) noexcept;
  std::future<result<std::size_t>> recv_async(std::span<std::uint8_t> &buffer) noexcept;
  void                             flush() noexcept;

  serial_port(const serial_port &)                = delete;
  serial_port &operator=(const serial_port &)     = delete;
  serial_port(serial_port &&) noexcept            = default;
  serial_port &operator=(serial_port &&) noexcept = default;
};
} // namespace biojet

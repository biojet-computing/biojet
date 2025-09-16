#pragma once

#include "biojet/io/serial_configuration.hpp"
#include "biojet/result.hpp"

#include <experimental/propagate_const>

#include <memory>
#include <span>

namespace biojet::io
{
class serial_port
{
  class impl;
  std::experimental::propagate_const<std::unique_ptr<impl>> impl_;

public:
  serial_port() noexcept;
  explicit serial_port(serial_configuration config) noexcept;
  serial_port(serial_port &&) noexcept;
  serial_port &operator=(serial_port &&) noexcept;
  ~serial_port() noexcept;

  result<bool>        open() noexcept;
  result<bool>        open(serial_configuration config) noexcept;
  void                close() noexcept;
  bool                is_open() const noexcept;
  result<std::size_t> write(const std::span<const std::uint8_t> &buffer) noexcept;
  result<std::size_t> read(std::span<std::uint8_t> &buffer) noexcept;
  void                flush() noexcept;

  serial_port(const serial_port &)            = delete;
  serial_port &operator=(const serial_port &) = delete;
};
} // namespace biojet::io
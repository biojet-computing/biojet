#pragma once

#include "biojet/io/serial_port.hpp"
#include "biojet/utility/unique_handle.hpp"

#include <unistd.h>

namespace biojet::io
{
struct policy
{
  using handle_type = int;

  inline static constexpr handle_type invalid_handle() noexcept
  {
    return -1;
  }

  inline static constexpr bool valid(handle_type handle) noexcept
  {
    return invalid_handle() < handle;
  }

  inline static void close(handle_type handle) noexcept
  {
    ::close(handle);
  }
};

class serial_port::impl
{
  serial_configuration                   config_{};
  biojet::utility::unique_handle<policy> fd_{};
  char                                   unused_[4]{};

public:
  impl() noexcept;
  explicit impl(serial_configuration config) noexcept;
  impl(impl &&) noexcept;
  impl &operator=(impl &&) noexcept;
  ~impl() noexcept;

  result<bool>        open() noexcept;
  result<bool>        open(serial_configuration config) noexcept;
  void                close() noexcept;
  bool                is_open() const noexcept;
  result<std::size_t> write(const std::span<const std::uint8_t> &buffer) noexcept;
  result<std::size_t> read(std::span<std::uint8_t> &buffer) noexcept;
  void                flush() noexcept;

private:
  result<bool> configure() noexcept;

  impl(const impl &)            = delete;
  impl &operator=(const impl &) = delete;
};
} // namespace biojet::io
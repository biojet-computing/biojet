#pragma once

#include <utility>

namespace biojet::utility
{
template <typename Policy>
class unique_handle
{
  typename Policy::handle_type handle_;

public:
  using policy_type = Policy;

  constexpr unique_handle() noexcept : handle_(Policy::invalid_handle())
  {
  }

  explicit constexpr unique_handle(typename Policy::handle_type handle) noexcept : handle_(handle)
  {
  }

  constexpr unique_handle(unique_handle &&other) noexcept
      : handle_(std::exchange(other.handle_, Policy::invalid_handle()))
  {
  }

  constexpr unique_handle &operator=(unique_handle &&other) noexcept
  {
    swap(other);
    return *this;
  }

  ~unique_handle() noexcept
  {
    reset();
  }

  explicit constexpr operator bool() const noexcept
  {
    return Policy::valid(handle_);
  }

  constexpr bool operator!() const noexcept
  {
    return !Policy::valid(handle_);
  }

  constexpr bool is_valid() const noexcept
  {
    return Policy::valid(handle_);
  }

  constexpr typename Policy::handle_type get() const noexcept
  {
    return handle_;
  }

  constexpr typename Policy::handle_type release() noexcept
  {
    return std::exchange(handle_, Policy::invalid_handle());
  }

  void reset() noexcept
  {
    reset(Policy::invalid_handle());
  }

  void reset(typename Policy::handle_type handle) noexcept
  {
    if (handle != handle_)
    {
      if (auto old_handle = std::exchange(handle_, handle))
        Policy::close(old_handle);
    }
  }

  constexpr void swap(unique_handle &other) noexcept
  {
    std::swap(handle_, other.handle_);
  }

  unique_handle(const unique_handle &)                          = delete;
  unique_handle &operator=(const unique_handle &)               = delete;
  unique_handle &operator=(typename Policy::handle_type handle) = delete;
};
} // namespace biojet::utility
#pragma once

#include "biojet/status_code.hpp"

#include <expected>
#include <functional>
#include <type_traits>
#include <utility>

namespace biojet
{
template <typename T>
using result = std::expected<T, status_code>;

using void_result = result<void>;

template <typename T>
constexpr result<std::decay_t<T>>
make_success(T &&v) noexcept(std::is_nothrow_constructible_v<std::decay_t<T>, T &&>)
{
  return result<std::decay_t<T>>(std::in_place, std::forward<T>(v));
}

constexpr void_result make_success() noexcept
{
  return void_result(std::in_place);
}

struct make_error_t
{
  status_code v;

  template<typename T>
  constexpr operator result<T>() const noexcept
  {
    return result<T>(std::unexpected<status_code>(v));
  }
};

constexpr make_error_t make_error(status_code v) noexcept
{
  return make_error_t{v};
}

template <typename T, typename E, typename F>
constexpr auto operator>>=(std::expected<T, E> &&r,
                           F &&f) noexcept(noexcept(std::forward<std::expected<T, E>>(r).and_then(std::forward<F>(f))))
    -> decltype(std::forward<std::expected<T, E>>(r).and_then(std::forward<F>(f)))
{
  return std::forward<std::expected<T, E>>(r).and_then(std::forward<F>(f));
}

template <typename T, typename E, typename F>
constexpr auto operator|(std::expected<T, E> &&r,
                         F &&f) noexcept(noexcept(std::forward<std::expected<T, E>>(r).transform(std::forward<F>(f))))
    -> decltype(std::forward<std::expected<T, E>>(r).transform(std::forward<F>(f)))
{
  return std::forward<std::expected<T, E>>(r).transform(std::forward<F>(f));
}

} // namespace biojet

#pragma once

#include "biojet/result.hpp"

#include <concepts>
#include <future>
#include <span>

namespace biojet
{
template <typename T>
concept transport =
    requires(T t, const std::span<const std::uint8_t> &immutable_data, std::span<std::uint8_t> &mutable_data) {
      { t.open() } -> std::same_as<result<bool>>;
      { t.close() } -> std::same_as<void>;
      { t.is_open() } -> std::same_as<bool>;
      { t.send(immutable_data) } -> std::same_as<result<std::size_t>>;
      { t.recv(mutable_data) } -> std::same_as<result<std::size_t>>;
      { t.send_async(immutable_data) } -> std::same_as<std::future<result<std::size_t>>>;
      { t.recv_async(mutable_data) } -> std::same_as<std::future<result<std::size_t>>>;
      { t.flush() } -> std::same_as<void>;
    };
} // namespace biojet

#include "biojet/serial_port.hpp"
#if defined(unix) || defined(__unix) || defined(__unix__)
#include "serial_port_unix.hpp"
#endif

namespace biojet
{
serial_port::serial_port() noexcept : impl_(std::make_unique<impl>())
{
}

serial_port::serial_port(serial_configuration config) noexcept : impl_(std::make_unique<impl>(std::move(config)))
{
}

result<bool> serial_port::open() noexcept
{
  return impl_->open();
}

result<bool> serial_port::open(serial_configuration configuration) noexcept
{
  return impl_->open(std::move(configuration));
}

void serial_port::close() noexcept
{
  impl_->close();
}

bool serial_port::is_open() const noexcept
{
  return impl_->is_open();
}

result<std::size_t> serial_port::send(const std::span<const std::uint8_t> &buffer) noexcept
{
  return impl_->send(buffer);
}

result<std::size_t> serial_port::recv(std::span<std::uint8_t> &buffer) noexcept
{
  return impl_->recv(buffer);
}

void serial_port::flush() noexcept
{
  impl_->flush();
}

std::future<result<std::size_t>> serial_port::send_async(const std::span<const std::uint8_t> &buffer) noexcept
{
  return impl_->send_async(buffer);
}

std::future<result<std::size_t>> serial_port::recv_async(std::span<std::uint8_t> &buffer) noexcept
{
  return impl_->recv_async(buffer);
}

serial_port::~serial_port() = default;
} // namespace biojet

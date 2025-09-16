#include "biojet/io/serial_port.hpp"
#if defined(unix) || defined(__unix) || defined(__unix__)
#include "io/serial_port_unix.hpp"
#endif

namespace biojet::io
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

result<std::size_t> serial_port::write(const std::span<const std::uint8_t> &buffer) noexcept
{
  return impl_->write(buffer);
}

result<std::size_t> serial_port::read(std::span<std::uint8_t> &buffer) noexcept
{
  return impl_->read(buffer);
}

void serial_port::flush() noexcept
{
  impl_->flush();
}

serial_port::serial_port(serial_port &&) noexcept            = default;
serial_port &serial_port::operator=(serial_port &&) noexcept = default;
serial_port::~serial_port()                                  = default;
} // namespace biojet::io
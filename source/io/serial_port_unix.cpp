#include "biojet/result.hpp"

#include "io/serial_port_unix.hpp"
#include <spdlog/spdlog.h>

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <termios.h>

#include <algorithm>
#include <ranges>

namespace biojet::io
{
serial_port::impl::impl(serial_configuration configuration) noexcept
{
  open(std::move(configuration));
}

serial_port::impl::~impl()
{
  close();
}

result<bool> serial_port::impl::open() noexcept
{
  spdlog::debug("Opening serial port: {}, with baud {}", config_.path.data(), config_.baud);
  if (is_open())
  {
    spdlog::debug("Serial port already open");
    return true;
  }

  fd_.reset(::open(config_.path.data(), O_RDWR | O_NOCTTY | O_NONBLOCK));
  if (!fd_.is_valid())
  {
    spdlog::error("Opening serial port failed");
    return make_error(errc::port_error);
  }

  pollfd pfd{};
  pfd.fd     = fd_.get();
  pfd.events = POLLOUT;

  int poll_result = ::poll(&pfd, 1, config_.read_timeout_ms);
  if (poll_result == 0)
  {
    spdlog::error("Timeout while waiting for serial port");
    fd_.reset();
    return make_error(errc::timeout);
  }
  else if (poll_result < 0)
  {
    spdlog::error("Error during poll on serial port");
    fd_.reset();
    return make_error(errc::port_error);
  }

  spdlog::debug("Serial port open");

  auto config_result = configure();
  if (!config_result)
  {
    spdlog::error("Failed setting serial port...");
    return config_result;
  }

  spdlog::debug("Opening port done");
  return true;
}

result<bool> serial_port::impl::open(serial_configuration config) noexcept
{
  config_ = std::move(config);
  return open();
}

void serial_port::impl::close() noexcept
{
  spdlog::debug("Closing port...");
  if (!is_open())
  {
    spdlog::debug("Port already closed");
  }
  fd_.reset();
  spdlog::debug("Closing port done");
}

bool serial_port::impl::is_open() const noexcept
{
  return fd_.is_valid();
}

inline void log_hex(std::span<const std::uint8_t> data, std::size_t count, const char *prefix)
{
#ifndef NDEBUG
  static constexpr char hex_chars[] = "0123456789ABCDEF";
  char                  buf[256];
  char                 *p = buf;

  for (std::size_t i = 0; i < count && p + 3 < buf + sizeof(buf); ++i)
  {
    if (i != 0)
      *p++ = ' ';
    *p++ = hex_chars[data[i] >> 4];
    *p++ = hex_chars[data[i] & 0xF];
  }

  spdlog::debug("{} ({} bytes): [{}]", prefix, count, std::string_view(buf, p - buf));
#endif
}

result<std::size_t> serial_port::impl::write(const std::span<const std::uint8_t> &data) noexcept
{
  spdlog::debug("Writing bytes...");

  if (!is_open())
  {
    spdlog::error("Port not open");
    return make_error(errc::port_error);
  }

  fd_set writefds;
  FD_ZERO(&writefds);
  FD_SET(fd_.get(), &writefds);

  struct timeval timeout;
  timeout.tv_sec  = config_.write_timeout_ms / 1000;
  timeout.tv_usec = (config_.write_timeout_ms % 1000) * 1000;

  const auto select_result = ::select(fd_.get() + 1, nullptr, &writefds, nullptr, &timeout);
  if (select_result < 0)
  {
    spdlog::error("Select failed");
    return make_error(errc::port_error);
  }

  const auto bytes_written = ::write(fd_.get(), data.data(), data.size());
  if (bytes_written < 0)
  {
    spdlog::error("Write failed");
    return make_error(errc::port_error);
  }
  log_hex(data, static_cast<std::size_t>(bytes_written), "Serial write");
  return make_success(static_cast<std::size_t>(bytes_written));
}

result<std::size_t> serial_port::impl::read(std::span<std::uint8_t> &data) noexcept
{
  if (!is_open())
  {
    perror("serial_port::read - port not open");
    return make_error(errc::port_error);
  }

  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(fd_.get(), &readfds);

  struct timeval timeout;
  timeout.tv_sec  = config_.read_timeout_ms / 1000;
  timeout.tv_usec = (config_.read_timeout_ms % 1000) * 1000;

  const auto select_result = ::select(fd_.get() + 1, &readfds, nullptr, nullptr, &timeout);
  if (select_result < 0)
  {
    spdlog::error("Select failed");
    return make_error(errc::port_error);
  }

  const auto bytes_read = ::read(fd_.get(), data.data(), data.size());
  if (bytes_read < 0)
  {
    spdlog::error("Read failed");
    return make_error(errc::timeout);
  }
  log_hex(data, static_cast<std::size_t>(bytes_read), "Serial read");
  return make_success(static_cast<std::size_t>(bytes_read));
}

void serial_port::impl::flush() noexcept
{
  spdlog::debug("Flushing...");

  if (!is_open())
  {
    spdlog::error("Flush failed - port not open");
    return;
  }

  if (::tcflush(fd_.get(), TCIOFLUSH) != 0)
  {
    spdlog::error("Flush failed");
  }

  spdlog::debug("Flush done");
}

result<bool> serial_port::impl::configure() noexcept
{
  spdlog::debug("Port configuration initiated...");

  termios tty{};

  /* set cfg */
  if (::tcgetattr(fd_.get(), &tty) != 0)
  {
    spdlog::error("Get cfg failed");
    return make_error(errc::port_error);
  }

  /* set raw mode */
  cfmakeraw(&tty);

  /* set the baud rate */
  speed_t speed;
  switch (config_.baud)
  {
    case 2400:
    {
      speed = B2400;
      break;
    }
    case 4800:
    {
      speed = B4800;
      break;
    }
    case 9600:
    {
      speed = B9600;
      break;
    }
    case 19200:
    {
      speed = B19200;
      break;
    }
    case 38400:
    {
      speed = B38400;
      break;
    }
    case 57600:
    {
      speed = B57600;
      break;
    }
    case 115200:
    {
      speed = B115200;
      break;
    }
    default:
    {
      spdlog::error("Baud rate is invalid");
      return make_error(errc::port_error);
    }
  }

  /* set input speed */
  if (::cfsetispeed(&tty, speed) != 0)
  {
    spdlog::error("Set input speed failed");
    return make_error(errc::port_error);
  }

  /* set output speed */
  if (::cfsetospeed(&tty, speed) != 0)
  {
    spdlog::error("Set output speed failed");
    return make_error(errc::port_error);
  }

  // Set data bits
  switch (config_.bits)
  {
    case data_bits::_5:
    {
      tty.c_cflag &= ~CSIZE;
      tty.c_cflag |= CS5;
      break;
    }
    case data_bits::_6:
    {
      tty.c_cflag &= ~CSIZE;
      tty.c_cflag |= CS6;
      break;
    }
    case data_bits::_7:
    {
      tty.c_cflag &= ~CSIZE;
      tty.c_cflag |= CS7;
      break;
    }
    case data_bits::_8:
    {
      tty.c_cflag &= ~CSIZE;
      tty.c_cflag |= CS8;
      break;
    }
    default:
    {
      spdlog::error("Data bits is invalid");
      return make_error(errc::port_error);
    }
  }

  // Set parity
  switch (config_.parity)
  {
    case parity_mode::none:
    {
      tty.c_cflag &= ~PARENB;
      tty.c_iflag &= ~INPCK;
      break;
    }
    case parity_mode::odd:
    {
      tty.c_cflag |= (PARODD | PARENB);
      tty.c_iflag |= INPCK;
      break;
    }
    case parity_mode::even:
    {
      tty.c_cflag |= PARENB;
      tty.c_cflag &= ~PARODD;
      tty.c_iflag |= INPCK;
      break;
    }
    default:
    {
      spdlog::error("Parity is invalid");
      return make_error(errc::port_error);
    }
  }

  // Set stop bits
  switch (config_.stop)
  {
    case stop_bits::_1:
    {
      tty.c_cflag &= ~CSTOPB;
      break;
    }
    case stop_bits::_2:
    {
      tty.c_cflag |= CSTOPB;
      break;
    }
    default:
    {
      spdlog::error("Stop bits is invalid");
      return make_error(errc::port_error);
    }
  }

  // Set control flow
  switch (config_.flow)
  {
    case flow_control::none:
    {
      tty.c_cflag &= ~CRTSCTS;
      tty.c_iflag &= ~(IXON | IXOFF | IXANY);
      break;
    }
    case flow_control::software:
    {
      tty.c_cflag &= ~CRTSCTS;
      tty.c_iflag |= (IXON | IXOFF);
      break;
    }
    case flow_control::hardware:
    {
      tty.c_cflag |= CRTSCTS;
      tty.c_iflag &= ~(IXON | IXOFF | IXANY);
      break;
    }
    default:
    {
      spdlog::error("Control flow is invalid");
      return make_error(errc::port_error);
    }
  }

  // Set timeouts for non-blocking reads (since we use select() for timeout control)
  tty.c_cc[VTIME] = 0; /* No timeout - we handle this with select() */
  tty.c_cc[VMIN]  = 0; /* Non-blocking read - return immediately */

  // Flush data
  flush();

  // Finalize serial configuration
  if (::tcsetattr(fd_.get(), TCSANOW, &tty) != 0)
  {
    spdlog::error("Write cfg failed");
    return make_error(errc::port_error);
  }

  spdlog::debug("Port configuration done");
  return true;
}

serial_port::impl::impl() noexcept                                = default;
serial_port::impl::impl(impl &&) noexcept                         = default;
serial_port::impl &serial_port::impl::operator=(impl &&) noexcept = default;
} // namespace biojet::io
#include "biojet/serial_port.hpp"
#include "biojet/result.hpp"
#include "biojet/status_code.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <array>
#include <cstdint>
#include <future>
#include <span>
#include <string_view>

// Forward declare to avoid template instantiation issues
namespace testing::internal {
template <> struct FunctionMocker<std::future<biojet::result<std::size_t>>(const std::span<const std::uint8_t> &)>;
template <> struct FunctionMocker<std::future<biojet::result<std::size_t>>(std::span<std::uint8_t> &)>;
}

namespace biojet::tests
{

class mock_serial_impl
{
public:
  MOCK_METHOD(result<bool>, open, (), (noexcept));
  MOCK_METHOD(result<bool>, open, (serial_configuration config), (noexcept));
  MOCK_METHOD(void, close, (), (noexcept));
  MOCK_METHOD(bool, is_open, (), (const, noexcept));
  MOCK_METHOD(result<std::size_t>, send, (const std::span<const std::uint8_t> &buffer), (noexcept));
  MOCK_METHOD(result<std::size_t>, recv, (std::span<std::uint8_t> &buffer), (noexcept));
  MOCK_METHOD(void, flush, (), (noexcept));

  // For async methods, use typedef to avoid template issues
  using async_send_result = std::future<result<std::size_t>>;
  using async_recv_result = std::future<result<std::size_t>>;

  MOCK_METHOD(async_send_result, send_async, (const std::span<const std::uint8_t> &buffer), (noexcept));
  MOCK_METHOD(async_recv_result, recv_async, (std::span<std::uint8_t> &buffer), (noexcept));
};

class serial_port_unit_test : public testing::Test
{
protected:
  testing::StrictMock<mock_serial_impl> mock_impl_;

  serial_configuration default_config()
  {
    return {
      .path = "/dev/ttyUSB0",
      .baud = 115200,
      .bits = data_bits::_8,
      .stop = stop_bits::_1,
      .parity = parity_mode::none,
      .flow = flow_control::none,
      .write_timeout_ms = 1000,
      .read_timeout_ms = 1000
    };
  }
};

class serial_port_error_handling_test : public testing::Test
{
protected:
  serial_port port_;
};

TEST_F(serial_port_error_handling_test, recv_fails_when_port_not_open)
{
  std::array<std::uint8_t, 10> buffer;
  std::span<std::uint8_t> span(buffer);

  auto result = port_.recv(span);
  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), status_code::port_error);
}

TEST_F(serial_port_error_handling_test, send_fails_when_port_not_open)
{
  std::array<std::uint8_t, 5> data = {0x01, 0x02, 0x03, 0x04, 0x05};
  std::span<const std::uint8_t> span(data);

  auto result = port_.send(span);
  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), status_code::port_error);
}

TEST_F(serial_port_error_handling_test, async_recv_fails_when_port_not_open)
{
  std::array<std::uint8_t, 10> buffer;
  std::span<std::uint8_t> span(buffer);

  auto future = port_.recv_async(span);
  auto result = future.get();

  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), status_code::port_error);
}

TEST_F(serial_port_error_handling_test, async_send_fails_when_port_not_open)
{
  std::array<std::uint8_t, 5> data = {0x01, 0x02, 0x03, 0x04, 0x05};
  std::span<const std::uint8_t> span(data);

  auto future = port_.send_async(span);
  auto result = future.get();

  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), status_code::port_error);
}

TEST_F(serial_port_error_handling_test, is_open_returns_false_initially)
{
  EXPECT_FALSE(port_.is_open());
}

TEST_F(serial_port_error_handling_test, close_does_not_throw_when_not_open)
{
  EXPECT_NO_THROW(port_.close());
}

TEST_F(serial_port_error_handling_test, flush_does_not_throw_when_not_open)
{
  EXPECT_NO_THROW(port_.flush());
}

class span_behavior_test : public testing::Test {};

TEST_F(span_behavior_test, send_span_accepts_const_data)
{
  serial_port port;
  const std::array<std::uint8_t, 3> data = {0xAA, 0xBB, 0xCC};
  std::span<const std::uint8_t> span(data);

  auto result = port.send(span);
  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), status_code::port_error);
}

TEST_F(span_behavior_test, recv_span_requires_mutable_data)
{
  serial_port port;
  std::array<std::uint8_t, 10> buffer;
  std::span<std::uint8_t> span(buffer);

  auto result = port.recv(span);
  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), status_code::port_error);
}

TEST_F(span_behavior_test, empty_spans_handled_correctly)
{
  serial_port port;

  std::array<std::uint8_t, 0> empty_send_data{};
  std::span<const std::uint8_t> empty_send_span(empty_send_data);
  auto send_result = port.send(empty_send_span);
  ASSERT_FALSE(send_result.has_value());
  EXPECT_EQ(send_result.error(), status_code::port_error);

  std::array<std::uint8_t, 0> empty_recv_buffer{};
  std::span<std::uint8_t> empty_recv_span(empty_recv_buffer);
  auto recv_result = port.recv(empty_recv_span);
  ASSERT_FALSE(recv_result.has_value());
  EXPECT_EQ(recv_result.error(), status_code::port_error);
}
} // namespace biojet::tests
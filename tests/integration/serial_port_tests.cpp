#include "biojet/serial_port.hpp"

#include <gtest/gtest.h>

#include <array>
#include <chrono>
#include <cstdint>
#include <future>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

namespace biojet::tests
{
class serial_port_test : public testing::Test
{
protected:
  serial_port port_;

  void SetUp() override
  {
    auto result = port_.open();
    ASSERT_TRUE(result.has_value()) << "Failed to open serial port: " << message(result.error());
    port_.flush();
  }

  void TearDown() override
  {
    port_.close();
  }

  ~serial_port_test() override;
};

serial_port_test::~serial_port_test() = default;

TEST_F(serial_port_test, can_open_serial_device)
{
  EXPECT_TRUE(port_.is_open());
}

TEST_F(serial_port_test, can_send_single_byte)
{
  std::array<uint8_t, 1>   data = {0xEF};
  std::span<const uint8_t> span(data);

  auto result = port_.send(span);
  ASSERT_TRUE(result.has_value()) << "Send should succeed";
  EXPECT_EQ(*result, 1u) << "Should send exactly 1 byte";
}

TEST_F(serial_port_test, can_send_multiple_bytes)
{
  std::array<uint8_t, 4>   data = {0xEF, 0x01, 0xFF, 0xFF};
  std::span<const uint8_t> span(data);

  auto result = port_.send(span);
  ASSERT_TRUE(result.has_value()) << "Send should succeed";
  EXPECT_EQ(*result, 4u) << "Should send exactly 4 bytes";
}

TEST_F(serial_port_test, recv_timeout_works)
{
  std::array<uint8_t, 1> buffer;
  std::span<uint8_t>     span(buffer);

  auto start   = std::chrono::steady_clock::now();
  auto result  = port_.recv(span);
  auto elapsed = std::chrono::steady_clock::now() - start;

  ASSERT_TRUE(result.has_value()) << "Receive should not return error on timeout";
  EXPECT_EQ(*result, 0) << "Receive should have timed out";

  auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
  EXPECT_GE(elapsed_ms, 900) << "Should wait close to configured timeout";
  EXPECT_LE(elapsed_ms, 1200) << "Should timeout within reasonable margin";
}

TEST_F(serial_port_test, flush_operations_work)
{
  std::array<uint8_t, 2>   data = {0xEF, 0x01};
  std::span<const uint8_t> span(data);
  auto                     send_result = port_.send(span);
  ASSERT_TRUE(send_result.has_value());

  port_.flush();

  std::array<uint8_t, 1> buffer;
  std::span<uint8_t>     recv_span(buffer);
  auto                   recv_result = port_.recv(recv_span);
  ASSERT_TRUE(recv_result.has_value());
  EXPECT_EQ(*recv_result, 0) << "Receive should have timed out";
}

TEST_F(serial_port_test, reconnection_works)
{
  ASSERT_TRUE(port_.is_open());

  port_.close();
  EXPECT_FALSE(port_.is_open());

  auto result = port_.open();
  ASSERT_TRUE(result.has_value()) << "Failed to re-open: " << message(result.error());
  EXPECT_TRUE(port_.is_open());

  std::array<uint8_t, 2>   data = {0xEF, 0x01};
  std::span<const uint8_t> span(data);
  auto                     send_result = port_.send(span);
  EXPECT_TRUE(send_result.has_value()) << "Should work after reconnection";
}

TEST_F(serial_port_test, operations_fail_when_disconnected)
{
  port_.close();
  EXPECT_FALSE(port_.is_open());

  std::array<uint8_t, 1> recv_buffer;
  std::span<uint8_t>     recv_span(recv_buffer);
  auto                   recv_result = port_.recv(recv_span);
  EXPECT_FALSE(recv_result.has_value()) << "Receive should fail when disconnected";

  std::array<uint8_t, 2>   send_data = {0x01, 0x02};
  std::span<const uint8_t> send_span(send_data);
  auto                     send_result = port_.send(send_span);
  EXPECT_FALSE(send_result.has_value()) << "Send should fail when disconnected";
}

TEST_F(serial_port_test, async_send_single_byte)
{
  std::array<uint8_t, 1>   data = {0xEF};
  std::span<const uint8_t> span(data);

  auto future = port_.send_async(span);
  auto result = future.get();

  ASSERT_TRUE(result.has_value()) << "Async write should succeed";
  EXPECT_EQ(*result, 1u) << "Should write exactly 1 byte";
}

TEST_F(serial_port_test, async_send_multiple_bytes)
{
  std::array<uint8_t, 6>   data = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF};
  std::span<const uint8_t> span(data);

  auto future = port_.send_async(span);
  auto result = future.get();

  ASSERT_TRUE(result.has_value()) << "Async write should succeed";
  EXPECT_EQ(*result, 6u) << "Should write exactly 6 bytes";
}

TEST_F(serial_port_test, async_recv_timeout_works)
{
  std::array<uint8_t, 1> buffer;
  std::span<uint8_t>     span(buffer);

  auto start   = std::chrono::steady_clock::now();
  auto future  = port_.recv_async(span);
  auto result  = future.get();
  auto elapsed = std::chrono::steady_clock::now() - start;

  ASSERT_TRUE(result.has_value()) << "Async receive should not return error on timeout";
  EXPECT_EQ(*result, 0) << "Async receive should have timed out";

  auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
  EXPECT_GE(elapsed_ms, 900) << "Should wait close to configured timeout";
  EXPECT_LE(elapsed_ms, 1200) << "Should timeout within reasonable margin";
}

TEST_F(serial_port_test, async_operations_fail_when_disconnected)
{
  port_.close();
  EXPECT_FALSE(port_.is_open());

  std::array<uint8_t, 1> recv_buffer;
  std::span<uint8_t>     recv_span(recv_buffer);
  auto                   async_recv_future = port_.recv_async(recv_span);
  auto                   async_recv_result = async_recv_future.get();
  EXPECT_FALSE(async_recv_result.has_value()) << "Async receive should fail when disconnected";

  std::array<uint8_t, 2>   send_data = {0x01, 0x02};
  std::span<const uint8_t> send_span(send_data);
  auto                     async_send_future = port_.send_async(send_span);
  auto                     async_send_result = async_send_future.get();
  EXPECT_FALSE(async_send_result.has_value()) << "Async send should fail when disconnected";
}

TEST_F(serial_port_test, large_data_async_write)
{
  std::vector<uint8_t> large_data(1024);
  std::iota(large_data.begin(), large_data.end(), 0);
  std::span<const uint8_t> span(large_data);

  auto future = port_.send_async(span);
  auto result = future.get();

  ASSERT_TRUE(result.has_value()) << "Large async send should succeed";
  EXPECT_EQ(*result, large_data.size()) << "Should have sent all bytes";
}

TEST_F(serial_port_test, async_write_recv_round_trip)
{
  std::array<uint8_t, 8>   send_data = {0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0xFE, 0xBA, 0xBE};
  std::span<const uint8_t> send_span(send_data);

  auto send_future = port_.send_async(send_span);
  auto send_result = send_future.get();

  ASSERT_TRUE(send_result.has_value()) << "Async send should succeed";
  EXPECT_EQ(*send_result, send_data.size()) << "Should have sent all bytes";

  std::array<uint8_t, 32> recv_buffer;
  std::span<uint8_t>      recv_span(recv_buffer);

  auto recv_future = port_.recv_async(recv_span);
  auto recv_result = recv_future.get();

  ASSERT_TRUE(recv_result.has_value()) << "Async receive should not error (may timeout)";
}
} // namespace biojet::tests

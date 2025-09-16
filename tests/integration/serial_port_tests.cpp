#include "biojet/io/serial_port.hpp"

#include <gtest/gtest.h>

#include <array>
#include <chrono>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace biojet::io::tests
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
};

TEST_F(serial_port_test, can_open_serial_device)
{
  EXPECT_TRUE(port_.is_open());
}

TEST_F(serial_port_test, can_write_single_byte)
{
  std::array<uint8_t, 1>   data = {0xEF};
  std::span<const uint8_t> span(data);

  auto result = port_.write(span);
  ASSERT_TRUE(result.has_value()) << "Write should succeed";
  EXPECT_EQ(*result, 1u) << "Should write exactly 1 byte";
}

TEST_F(serial_port_test, can_write_multiple_bytes)
{
  std::array<uint8_t, 4>   data = {0xEF, 0x01, 0xFF, 0xFF};
  std::span<const uint8_t> span(data);

  auto result = port_.write(span);
  ASSERT_TRUE(result.has_value()) << "Write should succeed";
  EXPECT_EQ(*result, 4u) << "Should write exactly 4 bytes";
}

TEST_F(serial_port_test, read_timeout_works)
{
  std::array<uint8_t, 1> buffer;
  std::span<uint8_t>     span(buffer);

  auto start   = std::chrono::steady_clock::now();
  auto result  = port_.read(span);
  auto elapsed = std::chrono::steady_clock::now() - start;

  ASSERT_TRUE(result.has_value()) << "Read should not return error on timeout";
  EXPECT_EQ(*result, 0) << "Read should have timed out";

  auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
  EXPECT_GE(elapsed_ms, 900) << "Should wait close to configured timeout";
  EXPECT_LE(elapsed_ms, 1200) << "Should timeout within reasonable margin";
}

TEST_F(serial_port_test, flush_operations_work)
{
  std::array<uint8_t, 2>   data = {0xEF, 0x01};
  std::span<const uint8_t> span(data);
  auto                     write_result = port_.write(span);
  ASSERT_TRUE(write_result.has_value());

  port_.flush();

  std::array<uint8_t, 1> buffer;
  std::span<uint8_t>     read_span(buffer);
  auto                   read_result = port_.read(read_span);
  ASSERT_TRUE(read_result.has_value());
  EXPECT_EQ(*read_result, 0) << "Read should have timed out";
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
  auto                     write_result = port_.write(span);
  EXPECT_TRUE(write_result.has_value()) << "Should work after reconnection";
}

TEST_F(serial_port_test, operations_fail_when_disconnected)
{
  port_.close();
  EXPECT_FALSE(port_.is_open());

  std::array<uint8_t, 1> read_buffer;
  std::span<uint8_t>     read_span(read_buffer);
  auto                   read_result = port_.read(read_span);
  EXPECT_FALSE(read_result.has_value()) << "Read should fail when disconnected";

  std::array<uint8_t, 2>   write_data = {0x01, 0x02};
  std::span<const uint8_t> write_span(write_data);
  auto                     write_result = port_.write(write_span);
  EXPECT_FALSE(write_result.has_value()) << "Write should fail when disconnected";
}
} // namespace biojet::io::tests
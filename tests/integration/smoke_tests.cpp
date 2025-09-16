#include "biojet/io/serial_port.hpp"

#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <vector>

namespace biojet::io::tests
{
class smoke_test : public testing::Test
{
protected:
  serial_port port_{};

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

  serial_configuration create_config(uint32_t baud_rate) const
  {
    return {
      .path = "/dev/ttyAMA0",
      .baud = baud_rate,
      .bits = data_bits::_8,
      .stop = stop_bits::_1,
      .parity = parity_mode::none,
      .flow = flow_control::none,
      .write_timeout_ms = 1000,
      .read_timeout_ms = 1000
    };
  }

  std::array<uint8_t, 12> create_echo_command() const
  {
    return {
      0xEF, 0x01,             // Start code
      0xFF, 0xFF, 0xFF, 0xFF, // Device address (default broadcast)
      0x01,                   // Packet type: CMD
      0x00, 0x03,             // Data length: 1 data byte + 2 checksum bytes = 3 total
      0x53,                   // Command: CMD_GETECHO (simpler sensor ping)
      0x00, 0x57              // Checksum: packet_type + length + command = 0x01 + 0x00 + 0x03 + 0x53 = 0x57
    };
  }

  bool is_valid_response_header(const std::array<uint8_t, 32>& response) const
  {
    return response[0] == 0xEF && response[1] == 0x01 &&
           response[2] == 0xFF && response[3] == 0xFF &&
           response[4] == 0xFF && response[5] == 0xFF &&
           response[6] == 0x07; // ACK packet
  }


  bool try_baud_rate(uint32_t baud_rate)
  {
    port_.close();

    auto config = create_config(baud_rate);
    auto open_result = port_.open(config);
    if (!open_result.has_value()) {
      return false;
    }

    port_.flush();

    auto command_packet = create_echo_command();
    std::span<const uint8_t> cmd_span(command_packet);
    auto write_result = port_.write(cmd_span);
    if (!write_result.has_value() || *write_result != command_packet.size()) {
      return false;
    }

    std::array<uint8_t, 32> response_buffer;
    std::span<uint8_t> response_span(response_buffer);
    auto read_result = port_.read(response_span);
    if (!read_result.has_value() || *read_result < 9) {
      return false;
    }

    return is_valid_response_header(response_buffer);
  }
};

TEST_F(smoke_test, check_sensor_connection)
{
  const std::vector<uint32_t> baud_rates = {9600, 19200, 38400, 57600, 115200};

  for (auto baud_rate : baud_rates) {
    if (try_baud_rate(baud_rate)) {
      SUCCEED();
      return;
    }
  }

  FAIL() << "No fingerprint sensor found on any supported baud rate";
}
} // namespace biojet::io::tests
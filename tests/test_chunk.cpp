#include <doctest/doctest.h>
#include <stdint.h>
#include <stdio.h>

#include <bits.hpp>
#include <chunk.hpp>
#include <value.hpp>

using lox::Chunk;
using lox::decode_bits;
using lox::number_val;
using lox::OpCode;

TEST_CASE("write constant than return") {
  auto chunk = Chunk{};
  write(chunk, number_val(1.3), 123);
  write(chunk, static_cast<uint8_t>(OpCode::RETURN), 456);
  CHECK(chunk.code.count == 3);
  CHECK(chunk.code.data[0] == static_cast<uint8_t>(OpCode::CONSTANT));
  CHECK(chunk.code.data[1] == 0);
  CHECK(chunk.code.data[2] == static_cast<uint8_t>(OpCode::RETURN));
  CHECK(chunk.constants.count == 1);
  CHECK(chunk.constants.data[0] == number_val(1.3));
}

TEST_CASE("write 256 constants than return") {
  auto chunk = Chunk{};
  for (double i = 0; i < 2 << 7; ++i)
    write(chunk, number_val(i), 123);
  write(chunk, number_val(128.3), 123);
  write(chunk, static_cast<uint8_t>(OpCode::RETURN), 456);
  CHECK(chunk.code.count == 517);
  auto const data = chunk.code.data;
  CHECK(data[510] == static_cast<uint8_t>(OpCode::CONSTANT));
  CHECK(data[511] == 255);
  CHECK(data[512] == static_cast<uint8_t>(OpCode::CONSTANT_LONG));
  CHECK(decode_bits(data[513], data[514], data[515]) == 256);
  CHECK(data[516] == static_cast<uint8_t>(OpCode::RETURN));
  CHECK(chunk.constants.count == 257);
}

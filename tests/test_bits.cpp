#include <doctest/doctest.h>

#include <bits.hpp>

using lox::decode_bits;
using lox::encode_bits;

TEST_CASE("encode_binary") {
  auto const [actual1, actual2, actual3] = encode_bits(7432);
  CHECK(actual1 == 0);
  CHECK(actual2 == 29);
  CHECK(actual3 == 8);
}

TEST_CASE("decode_binary") {
  auto const actual = decode_bits(0, 29, 8);
  CHECK(actual == 7432);
}


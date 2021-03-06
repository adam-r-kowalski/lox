#include <bits.hpp>

namespace lox {

auto encode_bits(int32_t number) -> std::tuple<uint8_t, uint8_t, uint8_t> {
  return {number >> 16, number >> 8, number};
}

auto decode_bits(uint8_t a, uint8_t b, uint8_t c) -> int32_t {
  return (a << 16) | (b << 8) | c;
}

auto print_bits(int32_t number) -> void {
  if (!number)
    return;
  print_bits(number >> 1);
  putc((number & 1) ? '1' : '0', stdout);
}

} // namespace lox

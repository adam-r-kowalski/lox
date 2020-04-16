#pragma once

#include <stdint.h>
#include <tuple>

namespace lox {

auto encode_bits(int32_t number) -> std::tuple<uint8_t, uint8_t, uint8_t>;
auto decode_bits(uint8_t a, uint8_t b, uint8_t c) -> int32_t;
auto print_bits(int32_t number) -> void;

} // namespace lox

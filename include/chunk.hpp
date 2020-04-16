#pragma once

#include <stdint.h>

#include <array.hpp>
#include <value.hpp>

namespace lox {

enum struct OpCode : uint8_t { CONSTANT, CONSTANT_LONG, RETURN };

struct Chunk {
  Array<uint8_t> code;
  Array<Value> constants;
  Array<int> lines;
};

auto write(Chunk &chunk, uint8_t byte, int line) -> void;
auto write(Chunk &chunk, Value value, int line) -> void;
auto add_constant(Chunk &chunk, Value value) -> int;

} // namespace lox

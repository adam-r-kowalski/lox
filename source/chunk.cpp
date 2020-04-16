#include <bits.hpp>
#include <chunk.hpp>

namespace lox {

auto write(Chunk &chunk, uint8_t byte, int line) -> void {
  write(chunk.code, byte);
  write(chunk.lines, line);
}

auto write(Chunk &chunk, Value value, int line) -> void {
  auto constant = add_constant(chunk, value);
  if (constant < (2 << 7)) {
    write(chunk, static_cast<uint8_t>(OpCode::CONSTANT), line);
    write(chunk, static_cast<uint8_t>(constant), line);
  } else {
    write(chunk, static_cast<uint8_t>(OpCode::CONSTANT_LONG), line);
    auto const [a, b, c] = encode_bits(constant);
    write(chunk, a, line);
    write(chunk, b, line);
    write(chunk, c, line);
  }
}

auto add_constant(Chunk &chunk, Value value) -> int {
  write(chunk.constants, value);
  return chunk.constants.count - 1;
}

} // namespace lox

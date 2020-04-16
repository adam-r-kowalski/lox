#include <stdlib.h>

#include <chunk.hpp>
#include <debug.hpp>

using lox::add_constant;
using lox::Chunk;
using lox::disassemble;
using lox::OpCode;
using lox::write;

auto main() -> int {
  auto chunk = Chunk{};
  for (double i = 0; i < (2 << 7); ++i)
    write(chunk, i, 123);
  write(chunk, 3000.0, 200);
  write(chunk, (uint8_t)OpCode::RETURN, 456);
  disassemble(chunk, "test chunk");
  return 0;
}

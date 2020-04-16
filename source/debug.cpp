#include <stdio.h>

#include <bits.hpp>
#include <debug.hpp>
#include <value.hpp>

namespace lox {

auto disassemble(const Chunk &chunk, const char *name) -> void {
  printf("== %s ==\n", name);
  for (int offset = 0; offset < chunk.code.count;)
    offset = disassemble(chunk, offset);
}

auto simple_instruction(const char *name, int offset) -> int {
  printf("%s\n", name);
  return offset + 1;
}

auto constant_instruction(const char *name, const Chunk &chunk, int offset)
    -> int {
  auto const constant = chunk.code.data[offset + 1];
  printf("%-16s %4d '", name, constant);
  print(chunk.constants.data[constant]);
  printf("'\n");
  return offset + 2;
}

auto constant_long_instruction(const char *name, const Chunk &chunk, int offset)
    -> int {
  auto const data = chunk.code.data;
  auto const constant =
      decode_bits(data[offset + 1], data[offset + 2], data[offset + 3]);
  printf("%-16s %4d '", name, constant);
  print(chunk.constants.data[constant]);
  printf("'\n");
  return offset + 4;
}

auto disassemble(const Chunk &chunk, int offset) -> int {
  printf("%04d ", offset);
  if (offset > 0 && chunk.lines.data[offset] == chunk.lines.data[offset - 1])
    printf("   | ");
  else
    printf("%4d ", chunk.lines.data[offset]);

  auto const instruction = chunk.code.data[offset];
  switch (instruction) {
  case static_cast<uint8_t>(OpCode::CONSTANT):
    return constant_instruction("CONSTANT", chunk, offset);
  case static_cast<uint8_t>(OpCode::CONSTANT_LONG):
    return constant_long_instruction("CONSTANT LONG", chunk, offset);
  case static_cast<uint8_t>(OpCode::RETURN):
    return simple_instruction("RETURN", offset);
  default:
    printf("Unknown opcode %d\n", instruction);
    return offset + 1;
  }
}

} // namespace lox
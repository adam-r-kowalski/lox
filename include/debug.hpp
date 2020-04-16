#pragma once

#include <chunk.hpp>

namespace lox {

auto disassemble(const Chunk &chunk, const char *name) -> void;
auto disassemble(const Chunk &chunk, int offset) -> int;

} // namespace lox

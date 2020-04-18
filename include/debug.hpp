#pragma once

#include <chunk.hpp>

namespace lox {

auto constexpr trace_execution = true;

auto disassemble(Chunk const &chunk, char const *name) -> void;
auto disassemble(Chunk const &chunk, int offset) -> int;

} // namespace lox

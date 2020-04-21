#pragma once

#include <string_view>

#include <chunk.hpp>

namespace lox {

auto compile(std::string_view source, Chunk &chunk) -> bool;

} // namespace lox

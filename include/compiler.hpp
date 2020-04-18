#pragma once

#include <string_view>

#include <virtual_machine.hpp>

namespace lox {

auto compile(VirtualMachine const &vm, std::string_view source) -> void;

} // namespace lox

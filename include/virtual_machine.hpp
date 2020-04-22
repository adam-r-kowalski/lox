#pragma once

#include <string_view>

#include <chunk.hpp>
#include <value.hpp>

namespace lox {

auto constexpr stack_max = 256;

struct VirtualMachine {
  Chunk *chunk;
  uint8_t *instruction_pointer;
  Value stack[stack_max];
  Value *stack_top;

  VirtualMachine();
};

enum class InterpretResult { OK, COMPILE_ERROR, RUNTIME_ERROR };

auto reset_stack(VirtualMachine &vm) -> void;
auto interpret(VirtualMachine &vm, std::string_view source) -> InterpretResult;
auto push(VirtualMachine &vm, Value value) -> void;
auto pop(VirtualMachine &vm) -> Value;
auto peek(VirtualMachine &vm, int distance) -> Value;

} // namespace lox

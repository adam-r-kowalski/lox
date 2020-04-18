#include <functional>
#include <stdio.h>

#include <compiler.hpp>
#include <debug.hpp>
#include <virtual_machine.hpp>

namespace lox {

VirtualMachine::VirtualMachine() { reset_stack(*this); }

auto reset_stack(VirtualMachine &vm) -> void { vm.stack_top = vm.stack; }

auto run(VirtualMachine &vm) -> InterpretResult {
  auto const read_byte = [&]() -> uint8_t { return *vm.instruction_pointer++; };
  auto const read_constant = [&]() -> Value {
    return vm.chunk->constants.data[read_byte()];
  };
  auto const binary_op = [&](auto op) {
    auto rhs = pop(vm);
    auto lhs = pop(vm);
    push(vm, op(lhs, rhs));
  };

  for (;;) {
    if constexpr (trace_execution) {
      printf("          ");
      for (Value *slot = vm.stack; slot < vm.stack_top; ++slot) {
        printf("[ ");
        print(*slot);
        printf(" ]");
      }
      printf("\n");
      int const offset = vm.instruction_pointer - vm.chunk->code.data;
      disassemble(*vm.chunk, offset);
    }
    auto const instruction = read_byte();
    switch (instruction) {
    case static_cast<uint8_t>(OpCode::CONSTANT): {
      auto const constant = read_constant();
      push(vm, constant);
      break;
    }
    case static_cast<uint8_t>(OpCode::ADD):
      binary_op(std::plus<double>());
      break;
    case static_cast<uint8_t>(OpCode::SUBTRACT):
      binary_op(std::minus<double>());
      break;
    case static_cast<uint8_t>(OpCode::MULTIPLY):
      binary_op(std::multiplies<double>());
      break;
    case static_cast<uint8_t>(OpCode::DIVIDE):
      binary_op(std::divides<double>());
      break;
    case static_cast<uint8_t>(OpCode::NEGATE):
      push(vm, -pop(vm));
      break;
    case static_cast<uint8_t>(OpCode::RETURN): {
      print(pop(vm));
      printf("\n");
      return InterpretResult::OK;
    }
    }
  }
}

auto interpret(VirtualMachine &vm, std::string_view source) -> InterpretResult {
  compile(vm, source);
  return InterpretResult::OK;
}

auto push(VirtualMachine &vm, Value value) -> void {
  *vm.stack_top = value;
  vm.stack_top++;
}

auto pop(VirtualMachine &vm) -> Value {
  vm.stack_top--;
  return *vm.stack_top;
}

} // namespace lox

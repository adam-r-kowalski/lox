#include <functional>
#include <stdarg.h>
#include <stdio.h>

#include <compiler.hpp>
#include <debug.hpp>
#include <value.hpp>
#include <virtual_machine.hpp>

namespace lox {

auto runtime_error(VirtualMachine &vm, const char *format, ...) -> void;
auto is_falsey(Value value) -> bool;

VirtualMachine::VirtualMachine() { reset_stack(*this); }

auto reset_stack(VirtualMachine &vm) -> void { vm.stack_top = vm.stack; }

auto runtime_error(VirtualMachine &vm, const char *format, ...) -> void {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputs("\n", stderr);

  auto const instruction = vm.instruction_pointer - vm.chunk->code.data - 1;
  auto const line = vm.chunk->lines.data[instruction];
  fprintf(stderr, "[line %d] in script\n", line);
  reset_stack(vm);
}

auto run(VirtualMachine &vm) -> InterpretResult {
  auto const read_byte = [&]() -> uint8_t { return *vm.instruction_pointer++; };
  auto const read_constant = [&]() -> Value {
    return vm.chunk->constants.data[read_byte()];
  };
  auto const binary_op = [&](auto value_type, auto op) -> bool {
    if (!is_number(peek(vm, 0)) || !is_number(peek(vm, 1))) {
      runtime_error(vm, "Operands must be numbers.");
      return false;
    }
    auto const rhs = pop(vm).as.number;
    auto const lhs = pop(vm).as.number;
    push(vm, value_type(op(lhs, rhs)));
    return true;
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
    case static_cast<uint8_t>(OpCode::FALSE):
      push(vm, bool_val(false));
      break;
    case static_cast<uint8_t>(OpCode::TRUE):
      push(vm, bool_val(true));
      break;
    case static_cast<uint8_t>(OpCode::NIL):
      push(vm, nil_val);
      break;
    case static_cast<uint8_t>(OpCode::EQUAL):
      push(vm, bool_val(pop(vm) == pop(vm)));
      break;
    case static_cast<uint8_t>(OpCode::GREATER):
      if (!binary_op(bool_val, std::greater<double>()))
        return InterpretResult::RUNTIME_ERROR;
      break;
    case static_cast<uint8_t>(OpCode::LESS):
      if (!binary_op(bool_val, std::less<double>()))
        return InterpretResult::RUNTIME_ERROR;
      break;
    case static_cast<uint8_t>(OpCode::ADD):
      if (!binary_op(number_val, std::plus<double>()))
        return InterpretResult::RUNTIME_ERROR;
      break;
    case static_cast<uint8_t>(OpCode::SUBTRACT):
      if (!binary_op(number_val, std::minus<double>()))
        return InterpretResult::RUNTIME_ERROR;
      break;
    case static_cast<uint8_t>(OpCode::MULTIPLY):
      if (!binary_op(number_val, std::multiplies<double>()))
        return InterpretResult::RUNTIME_ERROR;
      break;
    case static_cast<uint8_t>(OpCode::DIVIDE):
      if (!binary_op(number_val, std::divides<double>()))
        return InterpretResult::RUNTIME_ERROR;
      break;
    case static_cast<uint8_t>(OpCode::NOT):
      push(vm, bool_val(is_falsey(pop(vm))));
      break;
    case static_cast<uint8_t>(OpCode::NEGATE):
      if (!is_number(peek(vm, 0))) {
        runtime_error(vm, "Operand must be a number.");
        return InterpretResult::RUNTIME_ERROR;
      }
      push(vm, number_val(-pop(vm).as.number));
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
  auto chunk = Chunk{};
  if (!compile(source, chunk))
    return InterpretResult::COMPILE_ERROR;
  vm.chunk = &chunk;
  vm.instruction_pointer = vm.chunk->code.data;
  return run(vm);
}

auto push(VirtualMachine &vm, Value value) -> void {
  *vm.stack_top = value;
  vm.stack_top++;
}

auto pop(VirtualMachine &vm) -> Value {
  vm.stack_top--;
  return *vm.stack_top;
}

auto peek(VirtualMachine &vm, int distance) -> Value {
  return vm.stack_top[-1 - distance];
}

auto is_falsey(Value value) -> bool {
  return is_nil(value) || (is_bool(value) && !value.as.boolean);
}

} // namespace lox

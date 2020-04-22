#include <stdio.h>

#include <value.hpp>

namespace lox {

auto is_bool(Value const &value) -> bool {
  return value.type == ValueType::BOOL;
}

auto is_nil(Value const &value) -> bool { return value.type == ValueType::NIL; }

auto is_number(Value const &value) -> bool {
  return value.type == ValueType::NUMBER;
}

auto bool_val(bool value) -> Value {
  return Value{ValueType::BOOL, {.boolean = value}};
}

auto number_val(double value) -> Value {
  return Value{ValueType::NUMBER, {.number = value}};
}

auto print(Value const &value) -> void { printf("%g", value.as.number); }

} // namespace lox

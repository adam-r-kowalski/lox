#include <stdio.h>

#include <value.hpp>

namespace lox
{

auto operator==(Value const &lhs, Value const &rhs) -> bool
{
  if (lhs.type != rhs.type)
    return false;
  switch (lhs.type)
  {
  case ValueType::BOOL:
    return lhs.as.boolean == rhs.as.boolean;
  case ValueType::NUMBER:
    return lhs.as.number == rhs.as.number;
  case ValueType::NIL:
    return true;
  }
}

auto is_bool(Value const &value) -> bool
{
  return value.type == ValueType::BOOL;
}

auto is_nil(Value const &value) -> bool { return value.type == ValueType::NIL; }

auto is_number(Value const &value) -> bool
{
  return value.type == ValueType::NUMBER;
}

auto bool_val(bool value) -> Value
{
  return Value{ValueType::BOOL, {.boolean = value}};
}

auto number_val(double value) -> Value
{
  return Value{ValueType::NUMBER, {.number = value}};
}

auto print(Value const &value) -> void
{
  switch (value.type)
  {
  case ValueType::BOOL:
    printf(value.as.boolean ? "true" : "false");
    break;
  case ValueType::NIL:
    printf("nil");
    break;
  case ValueType::NUMBER:
    printf("%g", value.as.number);
    break;
  }
}

} // namespace lox

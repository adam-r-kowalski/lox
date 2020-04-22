#pragma once

#include <array.hpp>

namespace lox {

enum class ValueType { BOOL, NIL, NUMBER };

struct Value {
  ValueType type;
  union {
    bool boolean;
    double number;
  } as;
};

auto is_bool(Value const &value) -> bool;
auto is_nil(Value const &value) -> bool;
auto is_number(Value const &value) -> bool;
auto constexpr nil_val = Value{ValueType::NIL, {.number = 0}};
auto bool_val(bool value) -> Value;
auto number_val(double value) -> Value;
auto print(Value const &value) -> void;

} // namespace lox

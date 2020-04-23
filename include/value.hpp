#pragma once

#include <array.hpp>

namespace lox {

enum class ValueType { BOOL, NIL, NUMBER, OBJ };

struct Obj;
struct ObjString;

struct Value {
  ValueType type;
  union {
    bool boolean;
    double number;
    Obj *obj;
  } as;
};

auto operator==(Value const &lhs, Value const &rhs) -> bool;
auto is_bool(Value const &value) -> bool;
auto is_nil(Value const &value) -> bool;
auto is_number(Value const &value) -> bool;
auto is_obj(Value const &value) -> bool;
auto constexpr nil_val = Value{ValueType::NIL, {.number = 0}};
auto bool_val(bool value) -> Value;
auto number_val(double value) -> Value;

template <typename T> auto obj_val(T value) -> Value {
  return Value{ValueType::OBJ, {.obj = reinterpret_cast<Obj *>(value)}};
}

auto print(Value const &value) -> void;

} // namespace lox

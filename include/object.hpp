#pragma once

#include <string_view>

#include <value.hpp>

namespace lox {

enum class ObjType { STRING };

struct Obj {
  ObjType type;
};

struct ObjString {
  Obj obj;
  int length;
  char *chars;
};

auto obj_type(Value const &value) -> ObjType;
auto is_string(Value const &value) -> bool;
auto as_string(Value const &value) -> ObjString *;
auto as_string(Value &value) -> ObjString *;
auto as_cstring(Value const &value) -> char *;
auto as_cstring(Value &value) -> char *;
auto copy_string(std::string_view chars) -> ObjString *;

} // namespace lox
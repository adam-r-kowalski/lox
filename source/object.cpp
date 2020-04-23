#include <object.hpp>

namespace lox {

auto is_obj_type(Value const &value, ObjType type) -> bool;
auto allocate_string(char *chars, unsigned int length) -> ObjString *;
template <typename T> auto allocate_obj(ObjType type) -> T *;

auto obj_type(Value const &value) -> ObjType { return value.as.obj->type; }

auto is_string(Value const &value) -> bool {
  return is_obj_type(value, ObjType::STRING);
}

auto is_obj_type(Value const &value, ObjType type) -> bool {
  return is_obj(value) && obj_type(value) == type;
}

auto as_string(Value const &value) -> ObjString * {
  return reinterpret_cast<ObjString *>(value.as.obj);
}

auto as_string(Value &value) -> ObjString * {
  return reinterpret_cast<ObjString *>(value.as.obj);
}

auto as_cstring(Value const &value) -> char * {
  return as_string(value)->chars;
}
auto as_cstring(Value &value) -> char * { return as_string(value)->chars; }

auto copy_string(std::string_view chars) -> ObjString * {
  auto const length = chars.length();
  auto heap_chars = allocate<char>(length + 1);
  memcpy(heap_chars, chars.data(), length);
  heap_chars[length] = '\0';
  return allocate_string(heap_chars, length);
}

auto allocate_string(char *chars, unsigned int length) -> ObjString * {
  auto string = allocate_obj<ObjString>(ObjType::STRING);
  string->length = length;
  string->chars = chars;
  return string;
}

template <typename T> auto allocate_obj(ObjType type) -> T * {
  auto object = reallocate<Obj>(NULL, 0, sizeof(T));
  object->type = type;
  return reinterpret_cast<T *>(object);
}

} // namespace lox

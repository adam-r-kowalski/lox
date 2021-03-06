#pragma once

#include <stdlib.h>

namespace lox {

auto grow_capacity(int capacity) -> int;

template <typename T>
inline auto reallocate(T *previous, size_t, size_t new_size) -> T * {
  if (new_size == 0) {
    free(previous);
    return nullptr;
  }
  return (T *)realloc(previous, new_size);
}

template <typename T>
inline auto grow_array(T *previous, int old_count, int count) -> T * {
  return reallocate(previous, sizeof(T) * old_count, sizeof(T) * count);
}

template <typename T> inline auto free_array(T *pointer, int old_count) -> T * {
  return reallocate(pointer, sizeof(T) * old_count, 0);
}

template <typename T> inline auto allocate(unsigned int count) -> T * {
  return reallocate<T>(nullptr, 0, sizeof(T) * count);
}

} // namespace lox

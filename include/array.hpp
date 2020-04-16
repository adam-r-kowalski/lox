#pragma once

#include <memory.hpp>

namespace lox {

template <typename T> struct Array {
  int count = 0;
  int capacity = 0;
  T *data = nullptr;

  ~Array() { free_array(data, capacity); }
};

template <typename T> inline auto write(Array<T> &array, T value) -> void {
  if (array.capacity < array.count + 1) {
    auto const old_capacity = array.capacity;
    array.capacity = grow_capacity(old_capacity);
    array.data = grow_array(array.data, old_capacity, array.capacity);
  }
  array.data[array.count++] = value;
}

} // namespace lox

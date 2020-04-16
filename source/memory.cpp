#include <memory.hpp>

namespace lox {

auto grow_capacity(int capacity) -> int {
  return capacity < 8 ? 8 : capacity * 2;
}

} // namespace lox

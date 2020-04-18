#include <stdio.h>

#include <value.hpp>

namespace lox {

auto print(Value const &value) -> void { printf("%g", value); }

} // namespace lox

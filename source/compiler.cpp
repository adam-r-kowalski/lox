#include <compiler.hpp>
#include <scanner.hpp>

namespace lox {

auto compile(VirtualMachine const &vm, std::string_view source) -> void {
  auto scanner = Scanner{source};
  int line = -1;
  for (;;) {
    auto const token = scan_token(scanner);
    if (token.line != line) {
      printf("%4d ", token.line);
      line = token.line;
    } else
      printf("   | ");
    int length = token.start.length();
    printf("%2d '%.*s'\n", token.type, length, token.start.begin());
    if (token.type == TokenType::END_OF_FILE)
      break;
  }
}

} // namespace lox

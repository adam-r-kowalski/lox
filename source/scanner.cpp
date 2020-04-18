#include <scanner.hpp>

namespace lox {

auto is_at_end(Scanner const &scanner) -> bool;
auto make_token(Scanner const &scanner, TokenType type) -> Token;
auto error_token(Scanner const &scanner, std::string_view message) -> Token;

Scanner::Scanner(std::string_view source)
    : start{source}, current{source}, line{1} {}

auto scan_token(Scanner &scanner) -> Token {
  scanner.start = scanner.current;
  if (is_at_end(scanner))
    return make_token(scanner, TokenType::END_OF_FILE);
  return error_token(scanner, "Unexpected character.");
}

auto is_at_end(Scanner const &scanner) -> bool {
  return scanner.current.empty();
}

auto make_token(Scanner const &scanner, TokenType type) -> Token {
  auto const length = scanner.current.length() - scanner.start.length();
  return Token{
      .type = type,
      .start = scanner.start.substr(0, length),
      .line = scanner.line,
  };
}

auto error_token(Scanner const &scanner, std::string_view message) -> Token {
  return Token{
      .type = TokenType::ERROR,
      .start = message,
      .line = scanner.line,
  };
}

} // namespace lox

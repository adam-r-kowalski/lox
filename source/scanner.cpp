#include <scanner.hpp>

namespace lox {

auto is_at_end(Scanner const &scanner) -> bool;
auto make_token(Scanner const &scanner, TokenType type) -> Token;
auto error_token(Scanner const &scanner, std::string_view message) -> Token;
auto advance(Scanner &scanner) -> char;
auto match(Scanner &scanner, char expected) -> bool;
auto skip_whitespace(Scanner &scanner) -> void;
auto peek(Scanner const &scanner) -> char;
auto peek_next(Scanner const &scanner) -> char;
auto string(Scanner &scanner) -> Token;
auto is_digit(char c) -> bool;
auto number(Scanner &scanner) -> Token;
auto is_alpha(char c) -> bool;
auto identifier(Scanner &scanner) -> Token;
auto identifier_type(Scanner const &scanner) -> TokenType;
auto check_keyword(Scanner const &scanner, unsigned int start,
                   std::string_view rest, TokenType type) -> TokenType;

Scanner::Scanner(std::string_view source)
    : start{source}, current{source}, line{1} {}

auto scan_token(Scanner &scanner) -> Token {
  skip_whitespace(scanner);
  scanner.start = scanner.current;
  if (is_at_end(scanner))
    return make_token(scanner, TokenType::END_OF_FILE);
  auto const c = advance(scanner);
  if (is_alpha(c))
    return identifier(scanner);
  if (is_digit(c))
    return number(scanner);
  switch (c) {
  case '(':
    return make_token(scanner, TokenType::LEFT_PAREN);
  case ')':
    return make_token(scanner, TokenType::RIGHT_PAREN);
  case '{':
    return make_token(scanner, TokenType::LEFT_BRACE);
  case '}':
    return make_token(scanner, TokenType::RIGHT_BRACE);
  case ';':
    return make_token(scanner, TokenType::SEMICOLON);
  case ',':
    return make_token(scanner, TokenType::COMMA);
  case '.':
    return make_token(scanner, TokenType::DOT);
  case '-':
    return make_token(scanner, TokenType::MINUS);
  case '+':
    return make_token(scanner, TokenType::PLUS);
  case '/':
    if (peek_next(scanner) == '/')
      while (peek(scanner) != '\n' && !is_at_end(scanner))
        advance(scanner);
    else
      return make_token(scanner, TokenType::SLASH);
    break;
  case '*':
    return make_token(scanner, TokenType::STAR);
  case '!':
    return make_token(scanner, match(scanner, '=') ? TokenType::BANG_EQUAL
                                                   : TokenType::BANG);
  case '=':
    return make_token(scanner, match(scanner, '=') ? TokenType::EQUAL_EQUAL
                                                   : TokenType::EQUAL);
  case '<':
    return make_token(scanner, match(scanner, '=') ? TokenType::LESS_EQUAL
                                                   : TokenType::LESS);
  case '>':
    return make_token(scanner, match(scanner, '=') ? TokenType::GREATER_EQUAL
                                                   : TokenType::GREATER);
  case '"':
    return string(scanner);
  }
  return error_token(scanner, "Unexpected character.");
}

auto is_at_end(Scanner const &scanner) -> bool {
  return scanner.current.empty();
}

auto make_token(Scanner const &scanner, TokenType type) -> Token {
  auto const length = scanner.start.length() - scanner.current.length();
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

auto advance(Scanner &scanner) -> char {
  auto const value = peek(scanner);
  scanner.current = scanner.current.substr(1);
  return value;
}

auto match(Scanner &scanner, char expected) -> bool {
  if (is_at_end(scanner))
    return false;
  if (peek(scanner) != expected)
    return false;
  scanner.current = scanner.current.substr(1);
  return true;
}

auto skip_whitespace(Scanner &scanner) -> void {
  for (;;) {
    switch (peek(scanner)) {
    case ' ':
    case '\r':
    case '\t':
      advance(scanner);
      break;
    case '\n':
      ++scanner.line;
      advance(scanner);
      break;
    default:
      return;
    }
  }
}

auto peek(Scanner const &scanner) -> char { return scanner.current.at(0); }

auto peek_next(Scanner const &scanner) -> char {
  return is_at_end(scanner) ? '\0' : scanner.current.at(1);
}

auto string(Scanner &scanner) -> Token {
  while (peek(scanner) != '"' && !is_at_end(scanner)) {
    if (peek(scanner) == '\n')
      ++scanner.line;
    advance(scanner);
  }
  if (is_at_end(scanner))
    return error_token(scanner, "Unterminated string.");
  advance(scanner);
  return make_token(scanner, TokenType::STRING);
}

auto is_digit(char c) -> bool { return c >= '0' && c <= '9'; }

auto number(Scanner &scanner) -> Token {
  while (is_digit(peek(scanner)))
    advance(scanner);
  if (peek(scanner) == '.' && is_digit(peek_next(scanner))) {
    advance(scanner);
    while (is_digit(peek(scanner)))
      advance(scanner);
  }
  return make_token(scanner, TokenType::NUMBER);
}

auto is_alpha(char c) -> bool {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

auto identifier(Scanner &scanner) -> Token {
  while (is_alpha(peek(scanner)) || is_digit(peek(scanner)))
    advance(scanner);
  return make_token(scanner, identifier_type(scanner));
}

auto identifier_type(Scanner const &scanner) -> TokenType {
  switch (scanner.start.at(0)) {
  case 'a':
    return check_keyword(scanner, 1, "nd", TokenType::AND);
  case 'c':
    return check_keyword(scanner, 1, "lass", TokenType::CLASS);
  case 'e':
    return check_keyword(scanner, 1, "lse", TokenType::ELSE);
  case 'f':
    if (scanner.start.length() > scanner.current.length()) {
      switch (scanner.start.at(1)) {
      case 'a':
        return check_keyword(scanner, 2, "lse", TokenType::FALSE);
      case 'o':
        return check_keyword(scanner, 2, "r", TokenType::FOR);
      case 'u':
        return check_keyword(scanner, 2, "n", TokenType::FUN);
      }
    }
    break;
  case 'i':
    return check_keyword(scanner, 1, "f", TokenType::IF);
  case 'n':
    return check_keyword(scanner, 1, "il", TokenType::NIL);
  case 'o':
    return check_keyword(scanner, 1, "r", TokenType::OR);
  case 'p':
    return check_keyword(scanner, 1, "rint", TokenType::PRINT);
  case 'r':
    return check_keyword(scanner, 1, "eturn", TokenType::RETURN);
  case 's':
    return check_keyword(scanner, 1, "uper", TokenType::SUPER);
  case 't':
    if (scanner.start.length() > scanner.current.length()) {
      switch (scanner.start.at(1)) {
      case 'h':
        return check_keyword(scanner, 2, "is", TokenType::THIS);
      case 'r':
        return check_keyword(scanner, 2, "ue", TokenType::TRUE);
      }
    }
    break;
  case 'v':
    return check_keyword(scanner, 1, "ar", TokenType::VAR);
  case 'w':
    return check_keyword(scanner, 1, "hile", TokenType::WHILE);
  }

  return TokenType::IDENTIFIER;
}

auto check_keyword(Scanner const &scanner, unsigned int start,
                   std::string_view rest, TokenType type) -> TokenType {
  return scanner.start.substr(start, rest.length()) == rest
             ? type
             : TokenType::IDENTIFIER;
}

} // namespace lox

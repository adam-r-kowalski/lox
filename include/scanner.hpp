#pragma once

#include <string_view>

namespace lox {

struct Scanner {
  std::string_view start;
  std::string_view current;
  int line;

  Scanner(std::string_view source);
};

enum class TokenType {
  // single character tokens
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLOR,
  SLASH,
  STAR,

  // one or two character tokens
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // literals
  IDENTIFIER,
  STRING,
  NUMBER,

  // keywords
  AND,
  CLASS,
  ELSE,
  FALSE,
  FOR,
  FUN,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,

  // sentinal
  ERROR,
  END_OF_FILE,
};

struct Token {
  TokenType type;
  std::string_view start;
  int line;
};

auto scan_token(Scanner &scanner) -> Token;

} // namespace lox

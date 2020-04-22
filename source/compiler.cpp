#include <string>

#include <compiler.hpp>
#include <debug.hpp>
#include <scanner.hpp>

namespace lox {

struct Parser {
  Token current;
  Token previous;
  bool had_error = false;
  bool panic_mode = false;
};

enum class Precedence {
  NONE,
  ASSIGNMENT, // =
  OR,         // or
  AND,        // and
  EQUALITY,   // == !=
  COMPARISON, // < > <= >=
  TERM,       // + -
  FACTOR,     // * /
  UNARY,      // ! -
  CALL,       // . ()
  PRIMARY
};

using ParseFn = auto (*)(Chunk &, Parser &, Scanner &) -> void;

struct ParseRule {
  ParseFn prefix;
  ParseFn infix;
  Precedence precedence;
};

auto advance(Parser &parser, Scanner &scanner) -> void;
auto expression(Chunk &chunk, Parser &parser, Scanner &scanner) -> void;
auto consume(Parser &parser, Scanner &scanner, TokenType type,
             std::string_view message) -> void;
auto error_at_current(Parser &parser, std::string_view message) -> void;
auto error(Parser &parser, std::string_view message) -> void;
auto error_at(Parser &parser, Token const &token, std::string_view message)
    -> void;
auto end_compiler(Chunk &chunk, Parser const &parser) -> void;
auto emit_return(Chunk &chunk, Parser const &parser) -> void;

auto emit_bytes(Chunk &chunk, Parser const &parser, uint8_t byte) -> void;
template <typename... Bytes>
auto emit_bytes(Chunk &chunk, Parser const &parser, uint8_t byte,
                Bytes... bytes) -> void;

auto number(Chunk &chunk, Parser &parser, Scanner &scanner) -> void;
auto grouping(Chunk &chunk, Parser &parser, Scanner &scanner) -> void;
auto unary(Chunk &chunk, Parser &parser, Scanner &scanner) -> void;
auto binary(Chunk &chunk, Parser &parser, Scanner &scanner) -> void;
auto parse_precedence(Chunk &chunk, Parser &parser, Scanner &scanner,
                      Precedence precedence) -> void;
auto get_rule(TokenType type) -> ParseRule const &;

auto compile(std::string_view source, Chunk &chunk) -> bool {
  auto parser = Parser{};
  auto scanner = Scanner{source};
  advance(parser, scanner);
  expression(chunk, parser, scanner);
  consume(parser, scanner, TokenType::END_OF_FILE, "Expect end of expression.");
  end_compiler(chunk, parser);
  return !parser.had_error;
}

auto advance(Parser &parser, Scanner &scanner) -> void {
  parser.previous = parser.current;
  for (;;) {
    parser.current = scan_token(scanner);
    if (parser.current.type != TokenType::ERROR)
      break;
    error_at_current(parser, parser.current.start);
  }
}

auto expression(Chunk &chunk, Parser &parser, Scanner &scanner) -> void {
  parse_precedence(chunk, parser, scanner, Precedence::ASSIGNMENT);
}

auto consume(Parser &parser, Scanner &scanner, TokenType type,
             std::string_view message) -> void {
  if (parser.current.type == type) {
    advance(parser, scanner);
    return;
  }
  error_at_current(parser, message);
}

auto error_at_current(Parser &parser, std::string_view message) -> void {
  error_at(parser, parser.current, message);
}

auto error(Parser &parser, std::string_view message) -> void {
  error_at(parser, parser.previous, message);
}

auto error_at(Parser &parser, Token const &token, std::string_view message)
    -> void {
  parser.panic_mode = true;
  fprintf(stderr, "[line %d] Error", token.line);
  if (token.type == TokenType::END_OF_FILE)
    fprintf(stderr, " at end");
  else if (token.type == TokenType::ERROR) {
  } else {
    int const length = token.start.length();
    fprintf(stderr, " at '%.*s'", length, token.start.begin());
  }
  int const length = message.length();
  fprintf(stderr, ": %.*s\n", length, message.begin());
  parser.had_error = true;
}

auto end_compiler(Chunk &chunk, Parser const &parser) -> void {
  emit_return(chunk, parser);
  if constexpr (print_code)
    if (!parser.had_error)
      disassemble(chunk, "code");
}

auto emit_return(Chunk &chunk, Parser const &parser) -> void {
  return emit_bytes(chunk, parser, static_cast<uint8_t>(OpCode::RETURN));
}

auto emit_bytes(Chunk &chunk, Parser const &parser, uint8_t byte) -> void {
  write(chunk, byte, parser.previous.line);
}

template <typename... Bytes>
auto emit_bytes(Chunk &chunk, Parser const &parser, uint8_t byte,
                Bytes... bytes) -> void {
  write(chunk, byte, parser.previous.line);
  emite_bytes(chunk, parser, bytes...);
}

auto number(Chunk &chunk, Parser &parser, Scanner &) -> void {
  auto const value = std::stod(std::string{parser.previous.start});
  write(chunk, number_val(value), parser.previous.line);
}

auto grouping(Chunk &chunk, Parser &parser, Scanner &scanner) -> void {
  expression(chunk, parser, scanner);
  consume(parser, scanner, TokenType::RIGHT_PAREN,
          "Expect ')' after expression.");
}

auto unary(Chunk &chunk, Parser &parser, Scanner &scanner) -> void {
  auto const operator_type = parser.previous.type;
  parse_precedence(chunk, parser, scanner, Precedence::UNARY);
  switch (operator_type) {
  case TokenType::MINUS:
    emit_bytes(chunk, parser, static_cast<uint8_t>(OpCode::NEGATE));
    break;
  default:
    return;
  }
}

auto binary(Chunk &chunk, Parser &parser, Scanner &scanner) -> void {
  auto const operator_type = parser.previous.type;
  auto const rule = get_rule(operator_type);
  auto const precedence = static_cast<uint8_t>(rule.precedence) + 1;
  parse_precedence(chunk, parser, scanner, static_cast<Precedence>(precedence));
  switch (operator_type) {
  case TokenType::PLUS:
    emit_bytes(chunk, parser, static_cast<uint8_t>(OpCode::ADD));
    break;
  case TokenType::MINUS:
    emit_bytes(chunk, parser, static_cast<uint8_t>(OpCode::SUBTRACT));
    break;
  case TokenType::STAR:
    emit_bytes(chunk, parser, static_cast<uint8_t>(OpCode::MULTIPLY));
    break;
  case TokenType::SLASH:
    emit_bytes(chunk, parser, static_cast<uint8_t>(OpCode::DIVIDE));
    break;
  default:
    return;
  }
}

constexpr ParseRule rules[] = {
    {grouping, NULL, Precedence::NONE}, // TOKEN_LEFT_PAREN
    {NULL, NULL, Precedence::NONE},     // TOKEN_RIGHT_PAREN
    {NULL, NULL, Precedence::NONE},     // TOKEN_LEFT_BRACE
    {NULL, NULL, Precedence::NONE},     // TOKEN_RIGHT_BRACE
    {NULL, NULL, Precedence::NONE},     // TOKEN_COMMA
    {NULL, NULL, Precedence::NONE},     // TOKEN_DOT
    {unary, binary, Precedence::TERM},  // TOKEN_MINUS
    {NULL, binary, Precedence::TERM},   // TOKEN_PLUS
    {NULL, NULL, Precedence::NONE},     // TOKEN_SEMICOLON
    {NULL, binary, Precedence::FACTOR}, // TOKEN_SLASH
    {NULL, binary, Precedence::FACTOR}, // TOKEN_STAR
    {NULL, NULL, Precedence::NONE},     // TOKEN_BANG
    {NULL, NULL, Precedence::NONE},     // TOKEN_BANG_EQUAL
    {NULL, NULL, Precedence::NONE},     // TOKEN_EQUAL
    {NULL, NULL, Precedence::NONE},     // TOKEN_EQUAL_EQUAL
    {NULL, NULL, Precedence::NONE},     // TOKEN_GREATER
    {NULL, NULL, Precedence::NONE},     // TOKEN_GREATER_EQUAL
    {NULL, NULL, Precedence::NONE},     // TOKEN_LESS
    {NULL, NULL, Precedence::NONE},     // TOKEN_LESS_EQUAL
    {NULL, NULL, Precedence::NONE},     // TOKEN_IDENTIFIER
    {NULL, NULL, Precedence::NONE},     // TOKEN_STRING
    {number, NULL, Precedence::NONE},   // TOKEN_NUMBER
    {NULL, NULL, Precedence::NONE},     // TOKEN_AND
    {NULL, NULL, Precedence::NONE},     // TOKEN_CLASS
    {NULL, NULL, Precedence::NONE},     // TOKEN_ELSE
    {NULL, NULL, Precedence::NONE},     // TOKEN_FALSE
    {NULL, NULL, Precedence::NONE},     // TOKEN_FOR
    {NULL, NULL, Precedence::NONE},     // TOKEN_FUN
    {NULL, NULL, Precedence::NONE},     // TOKEN_IF
    {NULL, NULL, Precedence::NONE},     // TOKEN_NIL
    {NULL, NULL, Precedence::NONE},     // TOKEN_OR
    {NULL, NULL, Precedence::NONE},     // TOKEN_PRINT
    {NULL, NULL, Precedence::NONE},     // TOKEN_RETURN
    {NULL, NULL, Precedence::NONE},     // TOKEN_SUPER
    {NULL, NULL, Precedence::NONE},     // TOKEN_THIS
    {NULL, NULL, Precedence::NONE},     // TOKEN_TRUE
    {NULL, NULL, Precedence::NONE},     // TOKEN_VAR
    {NULL, NULL, Precedence::NONE},     // TOKEN_WHILE
    {NULL, NULL, Precedence::NONE},     // TOKEN_ERROR
    {NULL, NULL, Precedence::NONE},     // TOKEN_EOF
};

auto get_rule(TokenType type) -> ParseRule const & {
  return rules[static_cast<uint8_t>(type)];
}

auto parse_precedence(Chunk &chunk, Parser &parser, Scanner &scanner,
                      Precedence precedence) -> void {
  advance(parser, scanner);
  auto const prefix_rule = get_rule(parser.previous.type).prefix;
  if (prefix_rule == nullptr) {
    error(parser, "Expected expression.");
    return;
  }
  prefix_rule(chunk, parser, scanner);

  while (precedence <= get_rule(parser.current.type).precedence) {
    advance(parser, scanner);
    auto const infix_rule = get_rule(parser.previous.type).infix;
    infix_rule(chunk, parser, scanner);
  }
}

} // namespace lox

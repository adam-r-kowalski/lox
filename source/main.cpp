#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <streambuf>
#include <string>

#include <chunk.hpp>
#include <debug.hpp>
#include <virtual_machine.hpp>

using lox::add_constant;
using lox::Chunk;
using lox::disassemble;
using lox::interpret;
using lox::InterpretResult;
using lox::OpCode;
using lox::VirtualMachine;
using lox::write;

auto repl(VirtualMachine &vm) -> void;
auto run_file(VirtualMachine &vm, char const *path) -> void;

auto main(int argc, char const *argv[]) -> int {
  auto vm = VirtualMachine{};
  if (argc == 1)
    repl(vm);
  else if (argc == 2)
    run_file(vm, argv[1]);
  else {
    fprintf(stderr, "Usage: lox [path]\n");
    exit(64);
  }
  return 0;
}

auto repl(VirtualMachine &vm) -> void {
  char line[1024];
  for (;;) {
    printf("> ");
    if (!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }
  }
  interpret(vm, line);
}

auto run_file(VirtualMachine &vm, char const *path) -> void {
  auto file = std::ifstream{path};
  auto const source = std::string{std::istreambuf_iterator<char>(file),
                                  std::istreambuf_iterator<char>()};
  auto const result = interpret(vm, source);
  if (result == InterpretResult::COMPILE_ERROR)
    exit(65);
  if (result == InterpretResult::RUNTIME_ERROR)
    exit(70);
}

#include <iostream>

#include "interpreter.hpp"
#include "print.hpp"

auto main() -> int
{
  const std::string program = "(begin (define r 10) (* pi (* r r)))";
  auto tokens = interpreter::lex(program);

  std::cout << pretty_print_tokens(tokens) << std::endl;

  auto ast = interpreter::parse(tokens);

  std::cout << pretty_print(ast) << std::endl;

  return 1;
}

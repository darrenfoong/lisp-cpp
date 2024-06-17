#include <iostream>
#include <string>

#include "interpreter.hpp"

auto main() -> int
{
  const std::string program = "(begin (define r 10) (* pi (* r r)))";
  auto tokens = interpreter::lex(program);
  auto ast = interpreter::parse(tokens);
  auto const output = interpreter::eval(ast);
  std::cout << output << '\n';

  return 0;
}

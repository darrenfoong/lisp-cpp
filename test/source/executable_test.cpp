#include <iostream>

#include "interpreter.hpp"

auto main() -> int
{
  const std::string program = "(begin (define r 10) (* pi (* r r)))";
  auto tokens = interpreter::lex(program);

  for (const std::string& token : tokens) {
    std::cout << token << std::endl;
  }

  return 1;
}

#include <iostream>
#include <string>

#include "interpreter.hpp"
#include "print.hpp"

auto main() -> int
{
  const std::string program = "(begin (define r 10) (* pi (* r r)))";
  auto tokens = interpreter::lex(program);
  auto ast = interpreter::parse(tokens);
  auto env = interpreter::make_env();
  auto output = interpreter::eval(ast, env);

  std::cout << pretty_print(output) << std::endl;

  return 0;
}

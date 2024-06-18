#include <iostream>

#include "interpreter.hpp"

// TODO remove
auto pretty_print_tokens(const std::vector<std::string>& tokens) -> std::string;
auto pretty_print(const lisp::expr& expr, const int depth = 0) -> std::string;

auto main() -> int
{
  const std::string program = "(begin (define r 10) (* pi (* r r)))";
  auto tokens = interpreter::lex(program);

  std::cout << pretty_print_tokens(tokens) << std::endl;

  auto ast = interpreter::parse(tokens);

  std::cout << pretty_print(ast) << std::endl;

  return 1;
}

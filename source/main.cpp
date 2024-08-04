#include <iostream>

#include "interpreter.hpp"
#include "print.hpp"

auto main() -> int
{
  std::cout << "lisp-cpp: Lisp interpreter in C++" << std::endl;

  auto env = interpreter::make_env();
  std::string program;

  while (std::cout << "lisp-cpp> " && std::getline(std::cin, program)) {
    try {
      auto tokens = interpreter::lex(program);
      auto ast = interpreter::parse(tokens);
      auto output = interpreter::eval(ast, env);
      std::cout << pretty_print(output) << std::endl;
    } catch (const std::invalid_argument& e) {
      std::cout << "error: " << e.what() << std::endl;
    }
  }

  return 0;
}

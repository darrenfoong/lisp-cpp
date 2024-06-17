#pragma once

#include <string>
#include <vector>

#include "lisp.hpp"

class interpreter
{
public:
  static auto lex(const std::string& program) -> std::vector<std::string>;
  static auto parse(std::vector<std::string>& tokens) -> lisp::expr;
  static auto eval(lisp::expr& ast) -> std::string;
};

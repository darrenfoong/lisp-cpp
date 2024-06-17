#pragma once

#include <string>
#include <vector>

class interpreter
{
public:
  static auto lex(const std::string& program) -> std::vector<std::string>;
  static auto parse(std::vector<std::string>& tokens) -> int;
  static auto eval() -> std::string;
};

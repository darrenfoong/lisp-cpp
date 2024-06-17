#include <iterator>
#include <regex>
#include <sstream>
#include <vector>

#include "interpreter.hpp"

auto split(const std::string& input) -> std::vector<std::string>;

auto interpreter::lex(const std::string& program) -> std::vector<std::string>
{
  auto p = program;
  p = std::regex_replace(p, std::regex("\\("), " ( ");
  p = std::regex_replace(p, std::regex("\\)"), " ) ");

  return split(p);
}

auto interpreter::parse(std::vector<std::string>& tokens) -> int
{
  return 0;
}

auto interpreter::eval() -> std::string
{
  return "Hello world!";
}

auto split(const std::string& input) -> std::vector<std::string>
{
  std::istringstream buffer(input);
  std::vector<std::string> ret {std::istream_iterator<std::string>(buffer),
                                std::istream_iterator<std::string>()};
  return ret;
}

#include <algorithm>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "interpreter.hpp"

constexpr std::string_view lparen = "(";
constexpr std::string_view rparen = ")";

auto split(const std::string& input) -> std::vector<std::string>;
auto make_atom(const std::string& token) -> lisp::atom;

auto interpreter::lex(const std::string& program) -> std::vector<std::string>
{
  auto processed = program;
  processed = std::regex_replace(processed, std::regex("\\("), " ( ");
  processed = std::regex_replace(processed, std::regex("\\)"), " ) ");

  auto splitted = split(processed);
  std::reverse(splitted.begin(), splitted.end());

  return splitted;
}

// NOLINTNEXTLINE(misc-no-recursion)
auto interpreter::parse(std::vector<std::string>& tokens) -> lisp::expr
{
  if (tokens.empty()) {
    throw std::invalid_argument("no tokens");
  }

  auto token = tokens.back();
  tokens.pop_back();

  std::cout << "Processing token: " << token << std::endl;

  lisp::expr res;

  if (token == lparen) {
    auto list = lisp::list {};

    while (tokens.back() != rparen) {
      list.elems.emplace_back(interpreter::parse(tokens));
    }

    tokens.pop_back();

    res = lisp::expr {list};
  } else if (token == rparen) {
    throw std::invalid_argument("unexpected rparen");
  } else {
    auto atom = make_atom(token);

    res = lisp::expr {atom};
  }

  return res;
}

auto interpreter::eval(lisp::expr& ast) -> std::string
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

auto make_atom(const std::string& token) -> lisp::atom
{
  // TODO
  return lisp::atom {token};
}

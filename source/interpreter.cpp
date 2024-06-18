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
auto pretty_print_tokens(const std::vector<std::string>& tokens) -> std::string;
auto pretty_print(const lisp::atom& atom, int depth = 0) -> std::string;
auto pretty_print(const lisp::list& list, int depth = 0) -> std::string;
auto pretty_print(const lisp::expr& expr, int depth = 0) -> std::string;
auto indent(int depth) -> std::string;

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

// NOLINTNEXTLINE(misc-no-recursion)
auto pretty_print_tokens(const std::vector<std::string>& tokens) -> std::string
{
  std::ostringstream oss;

  for (auto it = tokens.crbegin(); it != tokens.crend(); it++) {
    oss << *it << std::endl;
  }

  return oss.str();
}

// NOLINTNEXTLINE(misc-no-recursion)
auto pretty_print(const lisp::atom& atom, const int depth) -> std::string
{
  std::ostringstream oss;

  if (std::holds_alternative<lisp::symbol>(atom)) {
    oss << indent(depth);
    oss << std::get<lisp::symbol>(atom);
  } else if (std::holds_alternative<lisp::number>(atom)) {
    auto number = std::get<lisp::number>(atom);
    if (std::holds_alternative<int>(number)) {
      oss << indent(depth);
      oss << std::get<int>(number);
    } else if (std::holds_alternative<double>(number)) {
      oss << indent(depth);
      oss << std::get<double>(number);
    } else {
      throw std::invalid_argument("invalid number");
    }
  } else {
    throw std::invalid_argument("invalid atom");
  }

  return oss.str();
}

// NOLINTNEXTLINE(misc-no-recursion)
auto pretty_print(const lisp::list& list, const int depth) -> std::string
{
  std::ostringstream oss;

  oss << indent(depth);
  oss << lparen << std::endl;

  for (const lisp::expr& elem : list.elems) {
    oss << pretty_print(elem, depth + 1) << std::endl;
  }

  oss << indent(depth);
  oss << rparen;

  return oss.str();
}

// NOLINTNEXTLINE(misc-no-recursion)
auto pretty_print(const lisp::expr& expr, const int depth) -> std::string
{
  std::ostringstream oss;

  if (std::holds_alternative<lisp::atom>(expr)) {
    oss << pretty_print(std::get<lisp::atom>(expr), depth);
  } else if (std::holds_alternative<lisp::list>(expr)) {
    oss << pretty_print(std::get<lisp::list>(expr), depth);
  } else {
    throw std::invalid_argument("invalid expr");
  }

  return oss.str();
}

auto indent(const int depth) -> std::string
{
  std::ostringstream oss;

  for (int i = 0; i < depth; i++) {
    oss << " ";
  }

  return oss.str();
}

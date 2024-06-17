#include <algorithm>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "interpreter.hpp"

const std::string LPAREN = "(";
const std::string RPAREN = ")";

auto split(const std::string& input) -> std::vector<std::string>;
auto make_atom(const std::string& token) -> lisp::atom;
auto pretty_print_tokens(const std::vector<std::string> tokens) -> std::string;
auto pretty_print(const lisp::atom& atom, const int depth = 0) -> std::string;
auto pretty_print(const lisp::list& list, const int depth = 0) -> std::string;
auto pretty_print(const lisp::expr& expr, const int depth = 0) -> std::string;
auto indent(const int depth) -> std::string;

auto interpreter::lex(const std::string& program) -> std::vector<std::string>
{
  auto processed = program;
  processed = std::regex_replace(processed, std::regex("\\("), " ( ");
  processed = std::regex_replace(processed, std::regex("\\)"), " ) ");

  auto splitted = split(processed);
  std::reverse(splitted.begin(), splitted.end());

  return splitted;
}

auto interpreter::parse(std::vector<std::string>& tokens) -> lisp::expr
{
  if (tokens.empty()) {
    throw std::invalid_argument("no tokens");
  }

  auto token = tokens.back();
  tokens.pop_back();

  std::cout << "Processing token: " << token << std::endl;

  if (token == LPAREN) {
    auto list = lisp::list {};

    while (tokens.back() != RPAREN) {
      list.elems.emplace_back(interpreter::parse(tokens));
    }

    tokens.pop_back();

    return lisp::expr {list};
  } else if (token == RPAREN) {
    throw std::invalid_argument("unexpected RPAREN");
  } else {
    auto atom = make_atom(token);

    return lisp::expr {atom};
  }
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

auto pretty_print_tokens(const std::vector<std::string> tokens) -> std::string
{
  std::ostringstream os;

  for (auto it = tokens.crbegin(); it != tokens.crend(); it++) {
    os << *it << std::endl;
  }

  return os.str();
}

auto pretty_print(const lisp::atom& atom, const int depth) -> std::string
{
  std::ostringstream os;

  if (std::holds_alternative<lisp::symbol>(atom)) {
    os << indent(depth);
    os << std::get<lisp::symbol>(atom);
  } else if (std::holds_alternative<lisp::number>(atom)) {
    auto number = std::get<lisp::number>(atom);
    if (std::holds_alternative<int>(number)) {
      os << indent(depth);
      os << std::get<int>(number);
    } else if (std::holds_alternative<double>(number)) {
      os << indent(depth);
      os << std::get<double>(number);
    } else {
      throw std::invalid_argument("invalid number");
    }
  } else {
    throw std::invalid_argument("invalid atom");
  }

  return os.str();
}

auto pretty_print(const lisp::list& list, const int depth) -> std::string
{
  std::ostringstream os;

  os << indent(depth);
  os << LPAREN << std::endl;

  for (const lisp::expr& elem : list.elems) {
    os << pretty_print(elem, depth + 1) << std::endl;
  }

  os << indent(depth);
  os << RPAREN;

  return os.str();
}

auto pretty_print(const lisp::expr& expr, const int depth) -> std::string
{
  std::ostringstream os;

  if (std::holds_alternative<lisp::atom>(expr)) {
    os << pretty_print(std::get<lisp::atom>(expr), depth);
  } else if (std::holds_alternative<lisp::list>(expr)) {
    os << pretty_print(std::get<lisp::list>(expr), depth);
  } else {
    throw std::invalid_argument("invalid expr");
  }

  return os.str();
}

auto indent(const int depth) -> std::string
{
  std::ostringstream os;

  for (int i = 0; i < depth; i++) {
    os << " ";
  }

  return os.str();
}

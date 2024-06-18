#include <iostream>
#include <sstream>
#include <vector>

#include "print.hpp"

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
  oss << "(" << std::endl;

  for (const lisp::expr& elem : list.elems) {
    oss << pretty_print(elem, depth + 1) << std::endl;
  }

  oss << indent(depth);
  oss << ")";

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

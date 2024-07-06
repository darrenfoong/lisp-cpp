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

  if (const auto* symbol_p = std::get_if<lisp::symbol>(&atom)) {
    oss << indent(depth);
    oss << *symbol_p;
  } else if (const auto* number_p = std::get_if<lisp::number>(&atom)) {
    if (const auto* int_p = std::get_if<int>(number_p)) {
      oss << indent(depth);
      oss << *int_p;
    } else if (const auto* double_p = std::get_if<double>(number_p)) {
      oss << indent(depth);
      oss << *double_p;
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

  if (const auto* atom_p = std::get_if<lisp::atom>(&expr)) {
    oss << pretty_print(*atom_p, depth);
  } else if (const auto* list_p = std::get_if<lisp::list>(&expr)) {
    oss << pretty_print(*list_p, depth);
  } else {
    throw std::invalid_argument("invalid expr");
  }

  return oss.str();
}

// NOLINTNEXTLINE(misc-no-recursion)
auto pretty_print(const lisp::exprfunc& exprfunc, const int depth)
    -> std::string
{
  if (const auto* expr_p = std::get_if<lisp::expr>(&exprfunc)) {
    return pretty_print(*expr_p, depth);
  } else {
    return "[func]";
  }
}

auto indent(const int depth) -> std::string
{
  std::ostringstream oss;

  for (int i = 0; i < depth; i++) {
    oss << " ";
  }

  return oss.str();
}

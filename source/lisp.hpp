#pragma once

#include <variant>

namespace lisp
{
using symbol = std::string;
using number = std::variant<int, double>;
using atom = std::variant<symbol, number>;

// hack for recursive data type
struct list  // NOLINT(misc-no-recursion)
{
  // actually std::vector<expr> elems
  std::vector<std::variant<atom, list>> elems;
};

using expr = std::variant<atom, list>;
}  // namespace lisp

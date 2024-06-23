#pragma once

#include <functional>
#include <unordered_map>
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
using func = std::function<expr(expr)>;
using env = std::unordered_map<std::string, std::variant<expr, func>>;
}  // namespace lisp

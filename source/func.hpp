#pragma once

#include <functional>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include "lisp.hpp"

template<typename T>
// NOLINTNEXTLINE(misc-unused-parameters, clang-diagnostic-unused-parameter)
inline auto parse_arg(const lisp::expr* arg) -> std::optional<T>
{
  return {};
}

template<>
inline auto parse_arg(const lisp::expr* arg) -> std::optional<double>
{
  if (const auto* arg_atom_p = std::get_if<lisp::atom>(arg)) {
    if (const auto* arg_number_p = std::get_if<lisp::number>(arg_atom_p)) {
      if (const auto* arg_double_p = std::get_if<double>(arg_number_p)) {
        return *arg_double_p;
      } else if (const auto* arg_int_p = std::get_if<int>(arg_number_p)) {
        return static_cast<double>(*arg_int_p);
      }
    }
  }

  return {};
}

// NOLINTNEXTLINE(performance-unnecessary-value-param)
template<typename T, typename U, typename R>
auto make_binary_op(std::function<R(const T&, const U&)> func) -> lisp::func
{
  return [=](const std::vector<lisp::expr>& args)
  {
    if (args.size() != 2) {
      throw std::invalid_argument("invalid num of args: "
                                  + std::to_string(args.size()));
    }

    // NOLINTNEXTLINE(readability-container-data-pointer)
    auto arg0_opt = parse_arg<T>(&args[0]);
    auto arg1_opt = parse_arg<U>(&args[1]);

    if (!arg0_opt) {
      throw std::invalid_argument("invalid arg: arg0");
    }
    if (!arg1_opt) {
      throw std::invalid_argument("invalid arg: arg1");
    }

    return lisp::expr {
        lisp::atom {lisp::number {func(arg0_opt.value(), arg1_opt.value())}}};
  };
}

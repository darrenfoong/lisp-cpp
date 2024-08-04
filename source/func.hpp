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

template<typename R>
// NOLINTNEXTLINE(misc-unused-parameters, clang-diagnostic-unused-parameter)
inline auto make_res(R res) -> lisp::expr
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

template<>
inline auto make_res(double res) -> lisp::expr
{
  return lisp::expr {lisp::atom {lisp::number {res}}};
}

template<>
inline auto make_res(bool res) -> lisp::expr
{
  // true is a list containing one element, 1
  // false is an empty list
  lisp::list list {};
  if (res) {
    list.elems.push_back(lisp::atom {lisp::number {1}});
  }
  return lisp::expr {list};
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

    return make_res<R>(func(arg0_opt.value(), arg1_opt.value()));
  };
}

// NOLINTNEXTLINE(performance-unnecessary-value-param)
template<typename T, typename R>
auto make_unary_op(std::function<R(const T&)> func) -> lisp::func
{
  return [=](const std::vector<lisp::expr>& args)
  {
    if (args.size() != 1) {
      throw std::invalid_argument("invalid num of args: "
                                  + std::to_string(args.size()));
    }

    // NOLINTNEXTLINE(readability-container-data-pointer)
    auto arg0_opt = parse_arg<T>(&args[0]);

    if (!arg0_opt) {
      throw std::invalid_argument("invalid arg: arg0");
    }

    return make_res<R>(func(arg0_opt.value()));
  };
}

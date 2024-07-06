#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include "func.hpp"

#include "lisp.hpp"

// NOLINTNEXTLINE(performance-unnecessary-value-param)
auto make_binary_op(std::function<double(const double&, const double&)> func)
    -> lisp::func
{
  return [=](const std::vector<lisp::expr>& args)
  {
    if (args.size() != 2) {
      throw std::invalid_argument("invalid num of args: "
                                  + std::to_string(args.size()));
    }

    double arg0, arg1;
    // TODO int
    // TODO error handling

    if (const auto* arg0_atom_p = std::get_if<lisp::atom>(&args[0])) {
      if (const auto* arg0_number_p = std::get_if<lisp::number>(arg0_atom_p)) {
        if (const auto* arg0_double_p = std::get_if<double>(arg0_number_p)) {
          arg0 = *arg0_double_p;
        } else if (const auto* arg0_int_p = std::get_if<int>(arg0_number_p)) {
          arg0 = static_cast<double>(*arg0_int_p);
        }
      }
    }

    if (const auto* arg1_atom_p = std::get_if<lisp::atom>(&args[1])) {
      if (const auto* arg1_number_p = std::get_if<lisp::number>(arg1_atom_p)) {
        if (const auto* arg1_double_p = std::get_if<double>(arg1_number_p)) {
          arg1 = *arg1_double_p;
        } else if (const auto* arg1_int_p = std::get_if<int>(arg1_number_p)) {
          arg1 = static_cast<double>(*arg1_int_p);
        }
      }
    }

    return lisp::expr {lisp::atom {lisp::number {func(arg0, arg1)}}};
  };
}

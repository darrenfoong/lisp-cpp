#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <optional>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <variant>
#include <vector>

#include "interpreter.hpp"

#include "func.hpp"

constexpr std::string_view lparen = "(";
constexpr std::string_view rparen = ")";

auto split(const std::string& input) -> std::vector<std::string>;
auto make_atom(const std::string& token) -> lisp::atom;
auto get_symbol(const lisp::expr& expr) -> std::optional<std::string>;

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

// NOLINTNEXTLINE(misc-no-recursion, readability-function-cognitive-complexity)
auto interpreter::eval(lisp::expr& ast, lisp::env& env) -> lisp::exprfunc
{
  if (const auto* atom_p = std::get_if<lisp::atom>(&ast)) {
    // atom
    if (const auto* symbol_p = std::get_if<lisp::symbol>(atom_p)) {
      // symbol
      auto var_kv = env.find(*symbol_p);

      if (var_kv == env.end()) {
        throw std::invalid_argument("invalid variable: " + *symbol_p);
      }

      return var_kv->second;
    } else {
      // number
      return ast;
    }
  } else if (const auto* list_p = std::get_if<lisp::list>(&ast)) {
    // list
    auto list = list_p->elems;

    if (list.empty()) {
      // empty list
      return ast;
    }

    auto symbol_opt = get_symbol(list[0]);

    if (symbol_opt) {
      auto symbol = symbol_opt.value();

      if (symbol == "begin") {
        lisp::exprfunc eval_res;
        for (std::size_t i = 1; i < list.size(); i++) {
          eval_res = eval(list[i], env);
        }
        // return last eval_res
        return eval_res;
      } else if (symbol == "define") {
        if (list.size() == 3) {
          if (const auto* var_atom_p = std::get_if<lisp::atom>(&list[1])) {
            if (const auto* var_symbol_p =
                    std::get_if<lisp::symbol>(var_atom_p))
            {
              env[*var_symbol_p] = eval(list[2], env);

              return lisp::exprfunc {};
            }
          }
        }

        throw std::invalid_argument("invalid define expr");
      }
    }

    // func application
    auto exprfunc = eval(list[0], env);

    if (const auto* func_p = std::get_if<lisp::func>(&exprfunc)) {
      std::vector<lisp::expr> args;

      for (std::size_t i = 1; i < list.size(); i++) {
        auto eval_res = eval(list[i], env);
        if (const auto* expr_p = std::get_if<lisp::expr>(&eval_res)) {
          args.emplace_back(*expr_p);
        } else {
          throw std::invalid_argument("invalid arg: [func]");
        }
      }

      return (*func_p)(args);
    } else {
      throw std::invalid_argument("invalid head of list: not a [func]");
    }

  } else {
    throw std::invalid_argument("invalid expr");
  }
}

auto interpreter::make_env() -> lisp::env
{
  lisp::env env;

  env["pi"] = lisp::exprfunc {lisp::expr {lisp::atom {lisp::number {3.14159}}}};
  auto m = std::multiplies<>();
  env["*"] = lisp::exprfunc {make_binary_op(m)};

  return env;
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
  try {
    int i = std::stoi(token);  // NOLINT(readability-identifier-length)
    return lisp::atom {lisp::number {i}};
  } catch (const std::invalid_argument& ex) {
    try {
      double d = std::stod(token);  // NOLINT(readability-identifier-length)
      return lisp::atom {lisp::number {d}};
    } catch (const std::invalid_argument& ex2) {
      return lisp::atom {token};
    }
  }
}

auto get_symbol(const lisp::expr& expr) -> std::optional<std::string>
{
  if (const auto* atom_p = std::get_if<lisp::atom>(&expr)) {
    if (const auto* symbol_p = std::get_if<lisp::symbol>(atom_p)) {
      return *symbol_p;
    }
  }

  return std::nullopt;
}

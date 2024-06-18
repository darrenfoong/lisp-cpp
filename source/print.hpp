#pragma once

#include <vector>

#include "lisp.hpp"

auto pretty_print_tokens(const std::vector<std::string>& tokens) -> std::string;
auto pretty_print(const lisp::atom& atom, int depth = 0) -> std::string;
auto pretty_print(const lisp::list& list, int depth = 0) -> std::string;
auto pretty_print(const lisp::expr& expr, int depth = 0) -> std::string;
auto indent(int depth) -> std::string;

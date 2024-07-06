#pragma once

#include <functional>

#include "lisp.hpp"

auto make_binary_op(std::function<double(const double&, const double&)> func)
    -> lisp::func;

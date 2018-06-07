#pragma once

#include "Message.hpp"

#include <string_view>

namespace Chut {
  template<typename T>
  auto ask(std::istream &is) {
    T val;
    is >> val;
    return val;
  }

  template<typename T>
  auto ask(std::istream &is, std::ostream &os, std::string_view prompt = "") {
    os << prompt;
    return ask<T>(is);
  }
}

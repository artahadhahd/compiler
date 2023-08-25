#pragma once
#include "tokenizer.hpp"
#include <array>
union Node {
  Tokens token;
  std::string expression;
  std::array<std::string, 2> pair;
};
using AST = std::vector<Node>;

#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "tokenizer.hpp"

int main() {
  std::string test1 = "let a int = j12 + 12 * b - a ()\nlet b int = 12;";
  for (auto a : lexer(test1)) {
    std::cout << a.token << '\n';
  }
}

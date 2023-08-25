#include <iostream>

#include "tokenizer.hpp"

int main() {
  std::string test1 = "let a int = j12 + 12 * b - a ()\nlet b int = 12;";
  for (auto a : lexer("0x123456890abcdef 0x1fgo")) {
    // for (auto a : lexer(test1)) {
    if (a.tokentype == Tokens::BadToken) {
      std::cout << a.token << ": invalid token at " << a.line_number << ":"
                << a.position << "\n";
      continue;
    }
    std::cout << a.token << '\n';
    std::cout << (int)a.tokentype << '\n';
  }
}

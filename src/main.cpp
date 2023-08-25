#include <iostream>

#include "tokenizer.hpp"

int main() {
  std::string test1 = "let a int = j12 + 12 * b - a ()\nlet b int = 12;";
  for (auto a :
       lexer("fib :: [Integer]\nfib = 1 : 1 : zipWith (+) fib (tail fib)")) {
    std::cout << a.token << '\n';
    std::cout << (int)a.tokentype << '\n';
  }
}

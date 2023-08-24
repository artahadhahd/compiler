#pragma once
#include <string>
#include <vector>
enum class Tokens {
  NewLine,
  Equals,
  Int,
  String,
  Bool,
  Plus,
  Minus,
  LogicNot,
  Times,
  LParen,
  RParen,
  AssignPlus,
  Regular,
  Lt,
};
struct Token {
  std::string token;
  std::size_t line_number, position;
};
auto lexer(std::string_view s) -> std::vector<Token>;

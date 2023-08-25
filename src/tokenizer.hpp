#pragma once
/*
 * Author: artahadhahd
 * Under the MIT License
 * Website: https://artahadhahd.github.io
 */
#include <string>
#include <unordered_map>
#include <vector>

enum class Tokens {
  Token,
  NewLine,
  Number,   // Hex, Binary, Octal, Decimal, doesn't matter.
  InAdd,    // +=
  InSub,    // -=
  InMul,    // *=
  InDiv,    // /=
  InMod,    // %=
  Bool,     // bool
  Char,     // char
  Int,      // int
  Float,    // float
  Double,   // double
  String,   // string
  Let,      // let
  Assign,   // =
  Add,      // +
  Sub,      // -
  Mul,      // *
  Div,      // /
  Mod,      // %
  If,       // if
  Then,     // then
  Elif,     // elseif
  Else,     // else
  Eq,       // ==
  Neq,      // !=
  LogicNot, // not
  LogicAnd, // and
  LogicOr,  // or
  Lt,       // Less than
  Gt,       // Greater than
  Le,       // Less than or equal
  Ge,       // Greater than or equal
  For,
  While,
  LParen,
  RParen,
  LBracket,
  RBracket,
  LBrace,   // {
  RBrace,   // }
  Function, // fun
  Lambda,   // lambda, or λ
};

struct Token {
  Tokens tokentype;
  std::string token;
  std::size_t line_number, position;
};

auto lexer(std::string_view s) -> std::vector<Token>;

// This variable has the name "keywords" but it also returns for operators and
// data types. If nothing is found, it returns Tokens::Token implicitly

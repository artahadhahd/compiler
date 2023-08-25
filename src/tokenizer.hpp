#pragma once
// #ifdef __GNUC__
// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
// #pragma GCC diagnostic pop
#ifdef __clang__ // I don't care about MSVC
#if __has_warning("-Wimplicit-fallthrough")
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#endif
#endif
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
  BadToken,
  EOL, // End of line, not end of life!
  Comma,
  Dot,
  // 0xff is converted into 255 automatically, 0Xff or 0xFF is not valid
  // Decimals can also contain ' character
  Decimal,
  Binary,
  Octal,
  Hex,
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
  Contains,
  While,
  LParen,
  RParen,
  LBracket,
  RBracket,
  LBrace,   // {
  RBrace,   // }
  Function, // fun
  Lambda,   // lambda, or λ
  Return,
};

struct Token {
  Tokens tokentype;
  std::string token;
  std::size_t line_number, position;
};

auto lexer(std::string_view s) -> std::vector<Token>;
bool is_number(Tokens);

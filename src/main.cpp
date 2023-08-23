#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
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
  LParen,
  RParen,
  AssignPlus,
  Regular,
  Lt,
};

struct Token {
  Tokens type;
  std::string actual;
  std::size_t line_number, position;
};

// TODO: map all operators to enum members
std::unordered_map<std::string_view, Tokens> symbol2tok{
    {"+", Tokens::Plus},        {"=", Tokens::Equals},    {"-", Tokens::Minus},
    {"!", Tokens::LogicNot},    {"(", Tokens::LParen},    {")", Tokens::RParen},
    {"+=", Tokens::AssignPlus}, {"!=", Tokens::LogicNot}, {"<", Tokens::Lt}};

auto lexer(std::string_view s) -> std::vector<Token> {
  std::vector<Token> output;
  std::string buf{};
  std::string tmp;
  std::size_t line_number = 1;
  std::size_t charpos = 1;
  bool parsing_operator = false;
  for (auto const c : s) {
    switch (c) {
      case ' ':
      case '\t':
      case '\r':
      case '\v':
        if (buf.length() != 0) {
          if (parsing_operator) {
            parsing_operator = false;
            output.push_back(Token{symbol2tok[buf], buf, line_number, charpos});
            buf = "";
            break;
          }
          output.push_back(Token{Tokens::Regular, buf, line_number,
                                 charpos});  // push buffer to output
          buf = "";
        }
        charpos++;
        break;
      case '\n':
        output.push_back(Token{Tokens::NewLine, buf, line_number, charpos});
        charpos = 0;
        line_number++;
        break;
      case '(':
      case ')':
        if (buf.length() != 0) {
          output.push_back(Token{Tokens::Regular, buf, line_number, charpos});
          buf = "";
        }
        tmp = std::string(1, c);
        output.push_back(Token{symbol2tok[tmp], tmp, line_number, charpos});
        charpos++;
        break;
      case '+':
      case '|':
      case '^':
      case '&':
      case '*':
      case '-':
      case '=':
      case ':':
      case '%':
      case '!':
      case '>':
      case '<':
        if (buf.length() != 0 and !parsing_operator) {
          output.push_back(Token{Tokens::Regular, buf, line_number, charpos});
          buf = "";
        }
        if (!parsing_operator) {
          parsing_operator = true;
        }
        buf.push_back(c);
        charpos++;
        break;
      default:
        charpos++;
        if (parsing_operator) {
          parsing_operator = false;
          output.push_back(Token{symbol2tok[buf], buf, line_number, charpos});
          buf = "";
        }
        buf.push_back(c);
        break;
    }
  }
  return output;
}

int main() {
  std::string test1 = " a**12 ";
  std::string test2 = "float t=j";  // bug for some reason? hmm?
  for (auto a : lexer(test1)) {
    std::cout << a.actual << '\n';
  }
}

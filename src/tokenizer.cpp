#include "tokenizer.hpp"

// To change the keywords and operators you can modify this part of the code.
// You can remove the unnecessary ones, not all of them have to be in this map.
static std::unordered_map<std::string_view, Tokens> keywords{
    {"let", Tokens::Let},       {"if", Tokens::If},
    {"then", Tokens::Then},     {"else", Tokens::Else},
    {"elseif", Tokens::Elif},   {"for", Tokens::For},
    {"while", Tokens::While},   {"fun", Tokens::Function},
    {">", Tokens::Gt},          {"<", Tokens::Lt},
    {"==", Tokens::Eq},         {"!=", Tokens::Neq},
    {"<=", Tokens::Le},         {">=", Tokens::Ge},
    {"(", Tokens::LParen},      {")", Tokens::RParen},
    {"*", Tokens::Mul},         {"%", Tokens::Mod},
    {"/", Tokens::Div},         {"+", Tokens::Add},
    {"-", Tokens::Sub},         {"=", Tokens::Assign},
    {"+=", Tokens::InAdd},      {"-=", Tokens::InSub},
    {"*=", Tokens::InMul},      {"/=", Tokens::InDiv},
    {"%=", Tokens::InMod},      {"bool", Tokens::Bool},
    {"char", Tokens::Char},     {"int", Tokens::Int},
    {"single", Tokens::Float},  {"double", Tokens::Double},
    {"string", Tokens::String}, {"not", Tokens::LogicNot},
    {"and", Tokens::LogicAnd},  {"or", Tokens::LogicOr},
    {"[", Tokens::LBracket},    {"]", Tokens::RBracket},
    {"{", Tokens::LBrace},      {"}", Tokens::RBrace},
    {"λ", Tokens::Lambda},      {"lambda", Tokens::Lambda},
    {"return", Tokens::Return}};

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
          output.push_back(Token{keywords[buf], buf, line_number, charpos});
          buf = "";
          break;
        }
        output.push_back(Token{keywords[buf], buf, line_number, charpos});
        buf = "";
      }
      charpos++;
      break;
    case '\n':
      line_number++;
      output.push_back(Token{keywords[buf], buf, line_number, charpos});
      charpos = 0;
      break;
    case '(':
    case ')':
    case '{':
    case '}':
    case '[':
    case ']':
    case '"':
    case '?':
      if (buf.length() != 0) {
        output.push_back(Token{keywords[buf], buf, line_number, charpos});
        buf = "";
      }
      tmp = std::string(1, c);
      output.push_back(Token{keywords[tmp], tmp, line_number, charpos});
      charpos++;
      break;
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
      /* basic arithmetic */
    case '|':
    case '^':
    case '&':
    case '~':
      /* basic bitwise operations */
    case '=':
    case '!':
    case '>':
    case '<':
      /* comparison */
    case ':':
    case '$':
      /* misc */
      if (buf.length() != 0 and !parsing_operator) {
        output.push_back(Token{keywords[buf], buf, line_number, charpos});
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
        output.push_back(Token{keywords[buf], buf, line_number, charpos});
        buf = "";
      }
      buf.push_back(c);
      break;
    }
  }
  if (buf.length() != 0) {
    output.push_back(Token{keywords[buf], buf, line_number, charpos});
  }
  return output;
}

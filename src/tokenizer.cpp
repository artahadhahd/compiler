#include "tokenizer.hpp"

bool is_number(Tokens t) {
  return (t == Tokens::Octal || t == Tokens::Hex || t == Tokens::Binary ||
          t == Tokens::Decimal);
}

bool is_bad_token(Tokens t) {
  return (t == Tokens::BadToken || t == Tokens::InvalidFloatLiteral ||
          t == Tokens::InvalidHex || t == Tokens::InvalidOctal ||
          t == Tokens::InvalidBinary || t == Tokens::InValidDecimal ||
          t == Tokens::IncompleteStringLiteral);
}

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
    {"return", Tokens::Return}, {"in", Tokens::Contains},
    {",", Tokens::Comma},       {".", Tokens::Dot},
};

static Tokens analyze_buffer(std::string_view buf) {
  auto out = keywords[buf];
  unsigned char dot_count = 0;
  if (out != Tokens::Token) {
    return out;
  }
  if (buf.length() == 0) {
    return Tokens::Token;
  }
  if (buf.at(0) == '"') {
    if (buf.at(buf.size() - 1) != '"') {
      return Tokens::IncompleteStringLiteral;
    }
    return Tokens::StringLiteral;
  }
  if (!std::isdigit(buf.at(0))) {
    return Tokens::Token;
  }
  if (buf.at(0) == '0') {
    if (buf.length() == 1) {
      return Tokens::Decimal;
    }
    if (buf.length() < 3) {
      return Tokens::BadToken;
    }
    switch (buf.at(1)) {
    case '.': // floating point literal?
      for (unsigned int i = 2; i < buf.size(); i++) {
        if (!std::isdigit(buf.at(i))) {
          return Tokens::InvalidFloatLiteral;
        }
      }
      return Tokens::FloatLiteral;
    case 'b': // binary?
      for (unsigned int i = 2; i < buf.size(); i++) {
        if (buf.at(i) != '0' && buf.at(i) != '1') {
          return Tokens::InvalidBinary;
        }
      }
      return Tokens::Binary;
    case 'o': // octal?
      for (unsigned int i = 2; i < buf.size(); i++) {
        if (!(buf.at(i) >= '0' && buf.at(i) <= '7')) {
          return Tokens::InvalidOctal;
        }
      }
      return Tokens::Octal;
    case 'x': // hex?
      for (unsigned int i = 2; i < buf.size(); i++) {
        if (!(std::isdigit(buf.at(i)) ||
              (buf.at(i) >= 'a' && buf.at(i) <= 'f'))) {
          return Tokens::InvalidHex;
        }
      }
      return Tokens::Hex;
    default: // none of the above
      return Tokens::BadToken;
    }
  } else { // decimal
    for (auto const c : buf) {
      if (c == '.') {
        dot_count++;
        continue;
      }
      if (c == '\'') { // something like 1'000 is allowed
        continue;
      }
      if (!(c >= '0' && c <= '9')) {
        return Tokens::BadToken;
      }
    }
  }
  if (dot_count == 0) {
    return Tokens::Decimal;
  }
  if (dot_count == 1) {
    return Tokens::FloatLiteral;
  }
  return Tokens::InvalidFloatLiteral;
}

static inline void push_to_output(std::vector<Token> &output, std::string &buf,
                                  std::size_t line_number,
                                  std::size_t charpos) {
  output.push_back(Token{analyze_buffer(buf), buf, line_number, charpos});
  buf = "";
}

auto lexer(std::string_view s) -> std::vector<Token> {
  std::vector<Token> output;
  std::string buf{};
  std::string tmp;
  std::size_t line_number = 1;
  std::size_t charpos = 1;
  bool parsing_operator = false;
  bool parsing_string = false;
  for (auto const c : s) {
    switch (c) {
    case '"': // String literals should be treated differently.
      if (!parsing_string) {
        buf.push_back(c);
        parsing_string = true;
      } else {
        parsing_string = false;
        buf.push_back(c);
        push_to_output(output, buf, line_number, charpos);
      }
      charpos++;
      break;
    case ' ':
    case '\t':
    case '\r':
    case '\v':
      if (parsing_string) {
        charpos++;
        buf.push_back(c);
        break;
      }
      if (buf.length() != 0) {
        if (parsing_operator) {
          parsing_operator = false;
          push_to_output(output, buf, line_number, charpos);
          break;
        }
        push_to_output(output, buf, line_number, charpos);
      }
      charpos++;
      break;
    case '\n':
      // ignore implicit falthrough warning. in C++20 this can be replaced with
      // the [[fallthrough]] attribute
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#else
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#endif
      line_number++;
#ifdef __GNUC__
#pragma GCC diagnostic pop
#else
#pragma clang diagnostic pop
#endif
    case ';':
      output.push_back(Token{Tokens::EOL, buf, line_number, charpos});
      charpos = 0;
      break;
    case ',':
    case '(':
    case ')':
    case '{':
    case '}':
    case '[':
    case ']':
    case '?':
      if (parsing_string) {
        buf.push_back(c);
        break;
      }
      if (buf.length() != 0) {
        push_to_output(output, buf, line_number, charpos);
      }
      tmp = std::string(1, c);
      output.push_back(Token{analyze_buffer(buf), tmp, line_number, charpos});
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
      if (parsing_string) {
        buf.push_back(c);
        break;
      }
      if (buf.length() != 0 and !parsing_operator) {
        push_to_output(output, buf, line_number, charpos);
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
        push_to_output(output, buf, line_number, charpos);
      }
      buf.push_back(c);
      break;
    }
  }
  if (buf.length() != 0) {
    output.push_back(Token{analyze_buffer(buf), buf, line_number, charpos});
  }
  return output;
}

#include "tokenizer.hpp"
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
          output.push_back(Token{buf, line_number, charpos});
          buf = "";
          break;
        }
        output.push_back(
            Token{buf, line_number, charpos}); // push buffer to output
        buf = "";
      }
      charpos++;
      break;
    case '\n':
      line_number++;
      output.push_back(Token{buf, line_number, charpos});
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
        output.push_back(Token{buf, line_number, charpos});
        buf = "";
      }
      tmp = std::string(1, c);
      output.push_back(Token{tmp, line_number, charpos});
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
        output.push_back(Token{buf, line_number, charpos});
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
        output.push_back(Token{buf, line_number, charpos});
        buf = "";
      }
      buf.push_back(c);
      break;
    }
  }
  if (buf.length() != 0) {
    output.push_back(Token{buf, line_number, charpos});
  }
  return output;
}

#ifndef CMILAN_SCANNER_H
#define CMILAN_SCANNER_H

#include <fstream>
#include <string>
#include <map>

using std::istream;
using std::string;
using std::map;

enum Token
{
  T_EOF,
  T_ILLEGAL,
  T_IDENTIFIER,
  T_NUMBER,
  T_BEGIN,
  T_END,
  T_IF,
  T_THEN,
  T_ELSE,
  T_FI,
  T_WHILE,
  T_DO,
  T_OD,
  T_WRITE,
  T_READ,
  T_ASSIGN,
  T_ADDOP,
  T_MULOP,
  T_CMP,
  T_LPAREN,
  T_RPAREN,
  T_SEMICOLON,
  T_INCREMENT,
  T_DECREMENT,
  T_LAND,
  T_LOR,
  T_BAND,
  T_BOR,
  T_NOT,
  T_TRUE,
  T_FALSE,
  T_BREAK,
  T_CONTINUE
};

const char * tokenToString(Token t);

enum Cmp
{
  C_EQ,
  C_NE,
  C_LT,
  C_LE,
  C_GT,
  C_GE
};

enum Arithmetic
{
  A_PLUS,
  A_MINUS,
  A_MULTIPLY,
  A_DIVIDE
};

class Scanner
{
public:
  explicit Scanner(const string& fileName, istream& input);

  virtual ~Scanner() {}

  const string& getFileName() const;
  int getLineNumber() const;
  Token token() const;
  int getIntValue() const;
  string getStringValue() const;
  Cmp getCmpValue() const;
  Arithmetic getArithmeticValue() const;

  void nextToken();

private:
  void skipSpace();
  void nextChar();
  bool isIdentifierStart(char c);
  bool isIdentifierBody(char c);

  const string fileName_;
  int lineNumber_;
  Token token_;
  int intValue_;
  string stringValue_;
  Cmp cmpValue_;
  Arithmetic arithmeticValue_;
  map<string, Token> keywords_;
  istream& input_;
  char ch_;
};

#endif

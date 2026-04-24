#include "scanner.h"
#include <algorithm>
#include <cctype>

using std::transform;
using std::isdigit;

static const char * tokenNames_[] = {
  "end of file",
  "illegal token",
  "identifier",
  "number",
  "'BEGIN'",
  "'END'",
  "'IF'",
  "'THEN'",
  "'ELSE'",
  "'FI'",
  "'WHILE'",
  "'DO'",
  "'OD'",
  "'WRITE'",
  "'READ'",
  "':='",
  "'+' or '-'",
  "'*' or '/'",
  "comparison operator",
  "'('",
  "')'",
  "';'",
  "'++'",
  "'--'",
  "'&&'",
  "'||'",
  "'&'",
  "'|'",
  "'!'",
  "'true'",
  "'false'",
  "'break'",
  "'continue'"
};

Scanner::Scanner(const string& fileName, istream& input)
  : fileName_(fileName), lineNumber_(1), input_(input)
{
  keywords_["begin"] = T_BEGIN;
  keywords_["end"] = T_END;
  keywords_["if"] = T_IF;
  keywords_["then"] = T_THEN;
  keywords_["else"] = T_ELSE;
  keywords_["fi"] = T_FI;
  keywords_["while"] = T_WHILE;
  keywords_["do"] = T_DO;
  keywords_["od"] = T_OD;
  keywords_["write"] = T_WRITE;
  keywords_["read"] = T_READ;
  keywords_["true"] = T_TRUE;
  keywords_["false"] = T_FALSE;
  keywords_["break"] = T_BREAK;
  keywords_["continue"] = T_CONTINUE;
  nextChar();
}

const string& Scanner::getFileName() const
{
  return fileName_;
}

int Scanner::getLineNumber() const
{
  return lineNumber_;
}

Token Scanner::token() const
{
  return token_;
}

int Scanner::getIntValue() const
{
  return intValue_;
}

string Scanner::getStringValue() const
{
  return stringValue_;
}

Cmp Scanner::getCmpValue() const
{
  return cmpValue_;
}

Arithmetic Scanner::getArithmeticValue() const
{
  return arithmeticValue_;
}

void Scanner::nextToken()
{
  skipSpace();

  if (ch_ == '/')
  {
    nextChar();
    if (ch_ == '*')
    {
      nextChar();
      bool inside = true;
      while (inside)
      {
        while (ch_ != '*' && !input_.eof())
        {
          nextChar();
        }
        if (input_.eof())
        {
          token_ = T_EOF;
          return;
        }
        nextChar();
        if (ch_ == '/')
        {
          inside = false;
          nextChar();
        }
      }
    }
    else
    {
      token_ = T_MULOP;
      arithmeticValue_ = A_DIVIDE;
      return;
    }
    skipSpace();
  }

  if (input_.eof())
  {
    token_ = T_EOF;
    return;
  }

  if (isdigit(ch_))
  {
    int value = 0;
    while (isdigit(ch_))
    {
      value = value * 10 + (ch_ - '0');
      nextChar();
    }
    token_ = T_NUMBER;
    intValue_ = value;
    return;
  }

  if (isIdentifierStart(ch_))
  {
    string buffer;
    while (isIdentifierBody(ch_))
    {
      buffer += ch_;
      nextChar();
    }
    transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
    map<string, Token>::iterator kwd = keywords_.find(buffer);
    if (kwd == keywords_.end())
    {
      token_ = T_IDENTIFIER;
      stringValue_ = buffer;
    }
    else
    {
      token_ = kwd->second;
    }
    return;
  }

  switch (ch_)
  {
    case '(':
      token_ = T_LPAREN;
      nextChar();
      break;
    case ')':
      token_ = T_RPAREN;
      nextChar();
      break;
    case ';':
      token_ = T_SEMICOLON;
      nextChar();
      break;
    case ':':
      nextChar();
      if (ch_ == '=')
      {
        token_ = T_ASSIGN;
        nextChar();
      }
      else
      {
        token_ = T_ILLEGAL;
      }
      break;
    case '+':
      nextChar();
      if (ch_ == '+')
      {
        token_ = T_INCREMENT;
        nextChar();
      }
      else
      {
        token_ = T_ADDOP;
        arithmeticValue_ = A_PLUS;
      }
      break;
    case '-':
      nextChar();
      if (ch_ == '-')
      {
        token_ = T_DECREMENT;
        nextChar();
      }
      else
      {
        token_ = T_ADDOP;
        arithmeticValue_ = A_MINUS;
      }
      break;
    case '*':
      token_ = T_MULOP;
      arithmeticValue_ = A_MULTIPLY;
      nextChar();
      break;
    case '<':
      token_ = T_CMP;
      nextChar();
      if (ch_ == '=')
      {
        cmpValue_ = C_LE;
        nextChar();
      }
      else
      {
        cmpValue_ = C_LT;
      }
      break;
    case '>':
      token_ = T_CMP;
      nextChar();
      if (ch_ == '=')
      {
        cmpValue_ = C_GE;
        nextChar();
      }
      else
      {
        cmpValue_ = C_GT;
      }
      break;
    case '!':
      nextChar();
      if (ch_ == '=')
      {
        token_ = T_CMP;
        cmpValue_ = C_NE;
        nextChar();
      }
      else
      {
        token_ = T_NOT;
      }
      break;
    case '=':
      token_ = T_CMP;
      cmpValue_ = C_EQ;
      nextChar();
      break;
    case '&':
      nextChar();
      if (ch_ == '&')
      {
        token_ = T_LAND;
        nextChar();
      }
      else
      {
        token_ = T_BAND;
      }
      break;
    case '|':
      nextChar();
      if (ch_ == '|')
      {
        token_ = T_LOR;
        nextChar();
      }
      else
      {
        token_ = T_BOR;
      }
      break;
    default:
      token_ = T_ILLEGAL;
      nextChar();
      break;
  }
}

void Scanner::skipSpace()
{
  while (isspace(ch_))
  {
    if (ch_ == '\n')
    {
      ++lineNumber_;
    }
    nextChar();
  }
}

void Scanner::nextChar()
{
  ch_ = input_.get();
}

bool Scanner::isIdentifierStart(char c)
{
  return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

bool Scanner::isIdentifierBody(char c)
{
  return isIdentifierStart(c) || (c >= '0' && c <= '9');
}

const char * tokenToString(Token t)
{
  return tokenNames_[t];
}

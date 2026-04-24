#ifndef CMILAN_PARSER_H
#define CMILAN_PARSER_H

#include "scanner.h"
#include "codegen.h"
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

using std::ostream;
using std::ostringstream;
using std::string;
using std::map;
using std::vector;

class Parser
{
public:
  Parser(const string& fileName, istream& input);
  ~Parser();
  void parse();

private:
  typedef map<string, int> VarTable;

  void program();
  void statementList();
  void statement();
  void expression();
  void term();
  void factor();
  void relation();
  void condition();
  void boolExpr();
  void boolTerm();
  void boolFactor();
  void boolPrimary();
  void boolSecondary();

  bool see(Token t);
  bool match(Token t);
  void next();
  void reportError(const string& message);
  void mustBe(Token t);
  void recover(Token t);
  int findOrAddVariable(const string& var);

  Scanner* scanner_;
  CodeGen* codegen_;
  ostream& output_;
  bool error_;
  bool recovered_;
  VarTable variables_;
  int lastVar_;

  vector<int> breakAddresses_;
  vector<int> continueAddresses_;
  int loopConditionAddress_;
  int loopEndAddress_;
  bool inLoop_;
};

#endif

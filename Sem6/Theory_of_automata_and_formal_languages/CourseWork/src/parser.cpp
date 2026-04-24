#include "parser.h"

Parser::Parser(const string& fileName, istream& input)
  : output_(std::cout), error_(false), recovered_(true), lastVar_(0),
    loopConditionAddress_(0), loopEndAddress_(0), inLoop_(false)
{
  scanner_ = new Scanner(fileName, input);
  codegen_ = new CodeGen(output_);
  next();
}

Parser::~Parser()
{
  delete codegen_;
  delete scanner_;
}

void Parser::parse()
{
  program();
  if (!error_)
  {
    codegen_->flush();
  }
}

void Parser::program()
{
  mustBe(T_BEGIN);
  statementList();
  mustBe(T_END);
  codegen_->emit(STOP);
}

void Parser::statementList()
{
  if (see(T_END) || see(T_OD) || see(T_ELSE) || see(T_FI))
  {
    return;
  }
  else
  {
    bool more = true;
    while (more)
    {
      statement();
      more = match(T_SEMICOLON);
    }
  }
}

void Parser::statement()
{
  if (see(T_IDENTIFIER))
  {
    string varName = scanner_->getStringValue();
    int varAddress = findOrAddVariable(varName);
    next();
    mustBe(T_ASSIGN);
    expression();
    codegen_->emit(STORE, varAddress);
  }
  else if (match(T_IF))
  {
    condition();
    int jumpNoAddress = codegen_->reserve();
    mustBe(T_THEN);
    statementList();
    if (match(T_ELSE))
    {
      int jumpAddress = codegen_->reserve();
      codegen_->emitAt(jumpNoAddress, JUMP_NO, codegen_->getCurrentAddress());
      statementList();
      codegen_->emitAt(jumpAddress, JUMP, codegen_->getCurrentAddress());
    }
    else
    {
      codegen_->emitAt(jumpNoAddress, JUMP_NO, codegen_->getCurrentAddress());
    }
    mustBe(T_FI);
  }
  else if (match(T_WHILE))
  {
    bool oldInLoop = inLoop_;
    int oldConditionAddr = loopConditionAddress_;
    int oldEndAddr = loopEndAddress_;
    vector<int> oldBreakAddrs = breakAddresses_;
    vector<int> oldContinueAddrs = continueAddresses_;

    inLoop_ = true;
    loopConditionAddress_ = codegen_->getCurrentAddress();
    condition();
    int jumpNoAddress = codegen_->reserve();
    mustBe(T_DO);
    statementList();
    mustBe(T_OD);
    codegen_->emit(JUMP, loopConditionAddress_);
    loopEndAddress_ = codegen_->getCurrentAddress();
    codegen_->emitAt(jumpNoAddress, JUMP_NO, loopEndAddress_);

    for (size_t i = 0; i < breakAddresses_.size(); ++i)
    {
      codegen_->emitAt(breakAddresses_[i], JUMP, loopEndAddress_);
    }
    for (size_t i = 0; i < continueAddresses_.size(); ++i)
    {
      codegen_->emitAt(continueAddresses_[i], JUMP, loopConditionAddress_);
    }

    inLoop_ = oldInLoop;
    loopConditionAddress_ = oldConditionAddr;
    loopEndAddress_ = oldEndAddr;
    breakAddresses_ = oldBreakAddrs;
    continueAddresses_ = oldContinueAddrs;
  }
  else if (match(T_WRITE))
  {
    mustBe(T_LPAREN);
    expression();
    mustBe(T_RPAREN);
    codegen_->emit(PRINT);
  }
  else if (match(T_BREAK))
  {
    if (!inLoop_)
    {
      reportError("break outside loop");
    }
    else
    {
      int addr = codegen_->reserve();
      breakAddresses_.push_back(addr);
    }
  }
  else if (match(T_CONTINUE))
  {
    if (!inLoop_)
    {
      reportError("continue outside loop");
    }
    else
    {
      int addr = codegen_->reserve();
      continueAddresses_.push_back(addr);
    }
  }
  else
  {
    reportError("statement expected");
  }
}

void Parser::expression()
{
  term();
  while (see(T_ADDOP))
  {
    Arithmetic op = scanner_->getArithmeticValue();
    next();
    term();
    if (op == A_PLUS)
    {
      codegen_->emit(ADD);
    }
    else
    {
      codegen_->emit(SUB);
    }
  }
}

void Parser::term()
{
  factor();
  while (see(T_MULOP))
  {
    Arithmetic op = scanner_->getArithmeticValue();
    next();
    factor();
    if (op == A_MULTIPLY)
    {
      codegen_->emit(MULT);
    }
    else
    {
      codegen_->emit(DIV);
    }
  }
}

void Parser::factor()
{
  if (see(T_NUMBER))
  {
    int value = scanner_->getIntValue();
    next();
    codegen_->emit(PUSH, value);
  }
  else if (match(T_TRUE))
  {
    codegen_->emit(PUSH, 1);
  }
  else if (match(T_FALSE))
  {
    codegen_->emit(PUSH, 0);
  }
  else if (see(T_IDENTIFIER) && scanner_->token() == T_IDENTIFIER)
  {
    string varName = scanner_->getStringValue();
    int varAddress = findOrAddVariable(varName);
    next();
    if (match(T_INCREMENT))
    {
      codegen_->emit(LOAD, varAddress);
      codegen_->emit(PUSH, 1);
      codegen_->emit(ADD);
      codegen_->emit(STORE, varAddress);
      codegen_->emit(PUSH, 1);
      codegen_->emit(SUB);
    }
    else if (match(T_DECREMENT))
    {
      codegen_->emit(LOAD, varAddress);
      codegen_->emit(PUSH, 1);
      codegen_->emit(SUB);
      codegen_->emit(STORE, varAddress);
      codegen_->emit(PUSH, 1);
      codegen_->emit(ADD);
    }
    else
    {
      codegen_->emit(LOAD, varAddress);
    }
  }
  else if (match(T_LPAREN))
  {
    expression();
    mustBe(T_RPAREN);
  }
  else if (match(T_READ))
  {
    codegen_->emit(INPUT);
  }
  else if (match(T_INCREMENT))
  {
    if (!match(T_IDENTIFIER))
    {
      reportError("identifier expected after '++'");
    }
    else
    {
      string varName = scanner_->getStringValue();
      int varAddress = findOrAddVariable(varName);
      codegen_->emit(LOAD, varAddress);
      codegen_->emit(PUSH, 1);
      codegen_->emit(ADD);
      codegen_->emit(DUP);
      codegen_->emit(STORE, varAddress);
    }
  }
  else if (match(T_DECREMENT))
  {
    if (!match(T_IDENTIFIER))
    {
      reportError("identifier expected after '--'");
    }
    else
    {
      string varName = scanner_->getStringValue();
      int varAddress = findOrAddVariable(varName);
      codegen_->emit(LOAD, varAddress);
      codegen_->emit(PUSH, 1);
      codegen_->emit(SUB);
      codegen_->emit(DUP);
      codegen_->emit(STORE, varAddress);
    }
  }
  else
  {
    reportError("factor expected");
  }
}

void Parser::relation()
{
  expression();
  if (see(T_CMP))
  {
    Cmp cmp = scanner_->getCmpValue();
    next();
    expression();
    switch (cmp)
    {
      case C_EQ: codegen_->emit(COMPARE, 0); break;
      case C_NE: codegen_->emit(COMPARE, 1); break;
      case C_LT: codegen_->emit(COMPARE, 2); break;
      case C_GT: codegen_->emit(COMPARE, 3); break;
      case C_LE: codegen_->emit(COMPARE, 4); break;
      case C_GE: codegen_->emit(COMPARE, 5); break;
    }
  }
  else
  {
    reportError("comparison operator expected");
  }
}

void Parser::condition()
{
  boolExpr();
}

void Parser::boolExpr()
{
  boolTerm();
  while (match(T_LOR))
  {
    boolTerm();
    codegen_->emit(ADD);
    codegen_->emit(PUSH, 0);
    codegen_->emit(COMPARE, 1);
  }
}

void Parser::boolTerm()
{
  boolFactor();
  while (match(T_LAND))
  {
    boolFactor();
    codegen_->emit(MULT);
  }
}

void Parser::boolFactor()
{
  boolPrimary();
  while (match(T_BOR))
  {
    boolPrimary();
    codegen_->emit(ADD);
    codegen_->emit(PUSH, 0);
    codegen_->emit(COMPARE, 1);
  }
}

void Parser::boolPrimary()
{
  boolSecondary();
  while (match(T_BAND))
  {
    boolSecondary();
    codegen_->emit(MULT);
  }
}

void Parser::boolSecondary()
{
  if (match(T_NOT))
  {
    boolSecondary();
    codegen_->emit(PUSH, 1);
    codegen_->emit(SUB);
  }
  else if (match(T_TRUE))
  {
    codegen_->emit(PUSH, 1);
  }
  else if (match(T_FALSE))
  {
    codegen_->emit(PUSH, 0);
  }
  else if (see(T_LPAREN))
  {
    match(T_LPAREN);
    boolExpr();
    mustBe(T_RPAREN);
  }
  else if (see(T_IDENTIFIER))
  {
    string varName = scanner_->getStringValue();
    int varAddress = findOrAddVariable(varName);
    next();
    codegen_->emit(LOAD, varAddress);
    if (see(T_CMP))
    {
      Cmp cmp = scanner_->getCmpValue();
      next();
      expression();
      switch (cmp)
      {
        case C_EQ: codegen_->emit(COMPARE, 0); break;
        case C_NE: codegen_->emit(COMPARE, 1); break;
        case C_LT: codegen_->emit(COMPARE, 2); break;
        case C_GT: codegen_->emit(COMPARE, 3); break;
        case C_LE: codegen_->emit(COMPARE, 4); break;
        case C_GE: codegen_->emit(COMPARE, 5); break;
      }
    }
  }
  else
  {
    relation();
  }
}

bool Parser::see(Token t)
{
  return scanner_->token() == t;
}

bool Parser::match(Token t)
{
  if (scanner_->token() == t)
  {
    scanner_->nextToken();
    return true;
  }
  else
  {
    return false;
  }
}

void Parser::next()
{
  scanner_->nextToken();
}

void Parser::reportError(const string& message)
{
  std::cerr << "Line " << scanner_->getLineNumber() << ": " << message << std::endl;
  error_ = true;
}

void Parser::mustBe(Token t)
{
  if (!match(t))
  {
    error_ = true;
    ostringstream msg;
    msg << tokenToString(scanner_->token()) << " found while "
        << tokenToString(t) << " expected";
    reportError(msg.str());
    recover(t);
  }
}

void Parser::recover(Token t)
{
  while (!see(t) && !see(T_EOF))
  {
    next();
  }
  if (see(t))
  {
    next();
  }
}

int Parser::findOrAddVariable(const string& var)
{
  VarTable::iterator it = variables_.find(var);
  if (it == variables_.end())
  {
    variables_[var] = lastVar_;
    return lastVar_++;
  }
  else
  {
    return it->second;
  }
}

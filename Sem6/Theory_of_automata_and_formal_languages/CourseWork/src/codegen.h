#ifndef CMILAN_CODEGEN_H
#define CMILAN_CODEGEN_H

#include <vector>
#include <iostream>

using std::vector;
using std::ostream;

enum Instruction
{
  NOP,
  STOP,
  LOAD,
  STORE,
  BLOAD,
  BSTORE,
  PUSH,
  POP,
  DUP,
  ADD,
  SUB,
  MULT,
  DIV,
  INVERT,
  COMPARE,
  JUMP,
  JUMP_YES,
  JUMP_NO,
  INPUT,
  PRINT
};

class Command
{
public:
  Command(Instruction instruction);
  Command(Instruction instruction, int arg);
  void print(int address, ostream& os);

private:
  Instruction instruction_;
  int arg_;
};

class CodeGen
{
public:
  explicit CodeGen(ostream& output);

  void emit(Instruction instruction);
  void emit(Instruction instruction, int arg);
  void emitAt(int address, Instruction instruction);
  void emitAt(int address, Instruction instruction, int arg);
  int getCurrentAddress();
  int reserve();
  void flush();

private:
  ostream& output_;
  vector<Command> commandBuffer_;
};

#endif

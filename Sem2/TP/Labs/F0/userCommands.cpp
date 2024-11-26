#include "userCommands.hpp"

#include <queue>
#include <stack>
#include <stdexcept>
#include <fstream>
#include <iostream>

#include "token.hpp"
#include "postfix.hpp"

namespace rebdev
{
  class userOper
  {
    public:
      double operator()(double x, double y = 0)
      {
        return postFixToResult(queue, x, y);
      }
      std::queue< rebdev::token > queue;
  };
}
void bracketsProtect(std::string str, std::string commName)
{
  std::string err = "forget ";
  if (str.find('{') == std::string::npos)
  {
    err += '{';
  }
  else if (str.find('}') == std::string::npos)
  {
    err += '}';
  }
  else
  {
    return;
  }
  err += " in ";
  err += commName;
  throw std::logic_error(err);
}
void addReplaceBase(std::string str, rebdev::unary & unaryMap, rebdev::binary & binaryMap, rebdev::userMath & uM, bool toAdd)
{
  std::string name;
  name.assign(str, 0, str.find(' '));
  if ((uM.find(name) != uM.end()) && toAdd)
  {
    std::string err = "operation ";
    err += name;
    err += " is exist, if you want to change operation logic please use command replace!";
    throw std::logic_error(err);
  }
  else if ((uM.find(name) == uM.end()) && !toAdd)
  {
    std::string err = "you try to relpace operation, which isn't exist in base! Operation name: ";
    err += name;
    err += "!";
    throw std::logic_error(err);
  }
  rebdev::userOper uo;
  size_t index = 0, lastIndex = str.find(' ', 0) + 1;
  std::stack< rebdev::token > mathStack;
  while (index < str.size())
  {
    index = str.find(' ', lastIndex);
    if (index == std::string::npos)
    {
      index = str.size();
    }
    std::string strPart;
    strPart.assign(str, lastIndex, (index - lastIndex));
    try
    {
      postFixBase(strPart, uo.queue, unaryMap, binaryMap, mathStack);
    }
    catch(const std::logic_error & e)
    {
      if ((strPart == "x") || (strPart == "y"))
      {
        uo.queue.push(rebdev::token{strPart[0]});
      }
      else if ((strPart == "{") || (strPart == "}")){}
      else
      {
        throw std::logic_error("bad mathematical expression!");
      }
    }
    lastIndex = index + 1;
  }
  while (!mathStack.empty())
  {
    uo.queue.push(mathStack.top());
    mathStack.pop();
  }
  if (str.find(" y ") != std::string::npos)
  {
    binaryMap[name] = uo;
  }
  else
  {
    unaryMap[name] = uo;
  }
  uM[name] = str;
}
void rebdev::add(std::string str, unary & unaryMap, binary & binaryMap, userMath & uM)
{
  bracketsProtect(str, "add");
  std::string str2;
  str2.assign(str, (str.find("add ") + 4));
  addReplaceBase(str2, unaryMap, binaryMap, uM, true);
}
void rebdev::replace(std::string str, unary & unaryMap, binary & binaryMap, userMath & uM)
{
  bracketsProtect(str, "replace");
  std::string str2;
  str2.assign(str, (str.find("replace ") + 8));
  addReplaceBase(str2, unaryMap, binaryMap, uM, false);
}
std::string getFileName(std::string str)
{
  std::string name;
  name.assign(str, (str.find("{ ") + 2), (str.find(" }") - (str.find("{ ") + 2)));
  return name;
}
void rebdev::importFile(std::string str, unary & unaryMap, binary & binaryMap, userMath & uM)
{
  bracketsProtect(str, "import");
  std::string fileName = getFileName(str);
  std::fstream inFile(fileName);
  if (!inFile.is_open())
  {
    std::string err = "try to import operations from ";
    err += fileName;
    err += ", which doesn't exist in this catalog!";
    throw std::logic_error(err);
  }
  while (!inFile.eof())
  {
    std::string inStr;
    std::getline(inFile, inStr);
    if (inStr.empty())
    {
      continue;
    }
    try
    {
      add(inStr, unaryMap, binaryMap, uM);
    }
    catch (const std::exception & e)
    {
      std::cerr << "uncorect operation [" << inStr << "] in file " << fileName << '\n';
    }
  }
}
void rebdev::exportFile(std::string str, userMath & uM)
{
  bracketsProtect(str, "export");
  std::ofstream outFile(getFileName(str));
  auto begin = uM.begin();
  while (begin != uM.end())
  {
    outFile << ((*begin).second) << '\n';
    ++begin;
  }
}

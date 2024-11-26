#include "postfix.hpp"

#include <stdexcept>

void rebdev::postFixBase(std::string & str, tokQueue & queue, unary & un, binary & bin, std::stack < token > & mathSt)
{
  try
  {
    token tok(&bin.at(str), str);
    if (!mathSt.empty())
    {
      token top = mathSt.top();
      mathSt.pop();
      if (top.priority() < tok.priority())
      {
        mathSt.push(top);
      }
      else
      {
        while ((top.priority() >= tok.priority()) && (top.priority() > 2))
        {
          queue.push(top);
          if (mathSt.empty())
          {
            break;
          }
          top = mathSt.top();
          mathSt.pop();
        }
      }
    }
    mathSt.push(tok);
  }
  catch (const std::out_of_range & e)
  {
    try
    {
      token tok(&un.at(str));
      mathSt.push(tok);
    }
    catch (const std::out_of_range & e)
    {
      if (str[0] == ')')
      {
        token top = mathSt.top();
        mathSt.pop();
        while (!top.leftBracket())
        {
          queue.push(top);
          top = mathSt.top();
          mathSt.pop();
        }
      }
      else if (str[0] == '(')
      {
        mathSt.push(token{true});
      }
      else
      {
        try
        {
          size_t symNum = 0;
          double num = std::stod(str, &symNum);
          if (str.size() != symNum)
          {
            throw std::logic_error("unknown type of operation!");
          }
          queue.push(token{num});
        }
        catch (const std::invalid_argument & e)
        {
          throw std::logic_error("bad mathematical expression!");
        }
        catch (const std::out_of_range & e)
        {
          throw std::logic_error("bad mathematical expression!");
        }
      }
    }
  }
}
void rebdev::makePostFix(std::string & str, tokQueue & queue, unary & un, binary & bin, const user & usr, userMath & uM)
{
  size_t lastIndex = 0, index = 0;
  std::stack< token > mathStack;
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
      usr.at(strPart)(str, uM);
      index = str.find('}') + 1;
    }
    catch(const std::out_of_range & e)
    {
      postFixBase(strPart, queue, un, bin, mathStack);
    }
    lastIndex = index + 1;
  }
  while (!mathStack.empty())
  {
    queue.push(mathStack.top());
    mathStack.pop();
  }
}
double rebdev::postFixToResult(tokQueue & q, double x, double y)
{
  tokQueue queue(q);
  std::stack< token > resultStack;
  while (!queue.empty())
  {
    token top = queue.front();
    queue.pop();
    if (top.priority() == 0)
    {
      resultStack.push(top);
    }
    else if (top.priority() == 5)
    {
      resultStack.push(token{x});
    }
    else if (top.priority() == 6)
    {
      resultStack.push(token{ y });
    }
    else
    {
      if (resultStack.empty())
      {
        throw std::logic_error("Uncorrect mathematical expression!");
      }
      token first = resultStack.top();
      resultStack.pop();
      if (top.priority() == 4)
      {
        resultStack.push(top(first));
      }
      else
      {
        if (resultStack.empty())
        {
          throw std::logic_error("Uncorrect mathematical expression!");
        }
        token second = resultStack.top();
        resultStack.pop();
        resultStack.push(top(second, first));
      }
    }
  }
  return (resultStack.top()).getNum();
}

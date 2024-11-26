#include "postfix.hpp"

#include <stdexcept>

#include "stack.hpp"

void rebdev::makePostFix(std::string & str, postFixQueue & queue)
{
  size_t lastIndex = 0, index = 0;
  Stack< token > mathStack;
  while (index < str.size())
  {
    index = str.find(' ', lastIndex);
    if (index == std::string::npos)
    {
      index = str.size();
    }
    std::string strPart;
    strPart.assign(str, lastIndex, (index - lastIndex));
    if (((strPart.size()) == 1) && (!isdigit(strPart[0])))
    {
      if (strPart[0] == ')')
      {
        auto top = *(mathStack.drop());
        while (!top.leftBracket())
        {
          queue.push(top);
          top = *(mathStack.drop());
        }
      }
      else if (strPart[0] == '(')
      {
        mathStack.push(token{strPart[0]});
      }
      else
      {
        token operTok(strPart[0]);
        if (!mathStack.empty())
        {
          auto top = *(mathStack.drop());
          if (top.priority() < operTok.priority())
          {
            mathStack.push(top);
          }
          else
          {
            while (top.priority() >= operTok.priority())
            {
              queue.push(top);
              if (mathStack.empty())
              {
                break;
              }
              top = *(mathStack.drop());
            }
          }
        }
        mathStack.push(operTok);
      }
    }
    else if (strPart.size() >= 1)
    {
      queue.push(token{std::stoll(strPart)});
    }

    lastIndex = index + 1;
  }
  while (!mathStack.empty())
  {
    queue.push(*(mathStack.drop()));
  }
}
long long rebdev::postFixToResult(postFixQueue & queue)
{
  Stack< token > resultStack;
  while (!queue.empty())
  {
    auto top = *(queue.drop());
    if (!top.isNum())
    {
      if (resultStack.size() < 2)
      {
        throw std::logic_error("Uncorrect mathematical expression!");
      }
      auto second = *(resultStack.drop());
      auto first = *(resultStack.drop());
      resultStack.push(top(first, second));
    }
    else
    {
      resultStack.push(top);
    }
  }
  return (*(resultStack.drop())).getNum();
}

#ifndef POSTFIX_HPP
#define POSTFIX_HPP

#include <string>

#include "queue.hpp"
#include "token.hpp"

namespace rebdev
{
  using postFixQueue = rebdev::Queue< token >;
  void makePostFix(std::string & str, postFixQueue & queue);
  long long postFixToResult(postFixQueue & queue);
}

#endif

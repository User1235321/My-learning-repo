#ifndef POSTFIX_HPP
#define POSTFIX_HPP

#include <string>
#include <queue>
#include <map>
#include <string>
#include <functional>
#include <stack>

#include "token.hpp"

namespace rebdev
{
  using tokQueue = std::queue< token >;
  using unary = std::map < std::string, std::function< double(const double &) > >;
  using binary = std::map < std::string, std::function< double(const double &, const double &) > >;
  using userMath = std::map< std::string, std::string >;
  using user = std::map < std::string, std::function< void(std::string, userMath &) > >;
  void postFixBase(std::string & strPart, tokQueue & queue, unary & un, binary & bin, std::stack < token > & mathSt);
  void makePostFix(std::string & str, tokQueue & queue, unary & un, binary & bin, const user & usr, userMath & uM);
  double postFixToResult(tokQueue & q, double x = 0, double y = 0);
}

#endif

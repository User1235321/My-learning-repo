#include "token.hpp"

rebdev::token::data::data(double num):
  num_(num)
{}
rebdev::token::data::data(bool leftBracket):
  leftBracket_(leftBracket)
{}
rebdev::token::data::data(std::function< double(const double &) > * unary):
  unary_(unary)
{}
rebdev::token::data::data(std::function< double(const double &, const double &) > * binary):
  binary_(binary)
{}
rebdev::token::token(double num):
  data_(num),
  priority_(0)
{}
rebdev::token::token(bool leftBracket):
  data_(leftBracket),
  priority_(1)
{}
rebdev::token::token(std::function< double(const double &) > * unary):
  data_(unary),
  priority_(4)
{}
rebdev::token::token(std::function< double(const double &, const double &) > * binary, std::string str):
  data_(binary),
  priority_(3)
{
  if ((str == "-") || (str == "+"))
  {
    priority_ = 2;
  }
}
rebdev::token::token(char var)
{
  priority_ = 5;
  if (var == 'y')
  {
    priority_ = 6;
  }
}
rebdev::token rebdev::token::operator()(token f)
{
  token newTok((*data_.unary_)(f.data_.num_));
  return newTok;
}
rebdev::token rebdev::token::operator()(token f, token s)
{
  token newTok((*data_.binary_)(f.data_.num_, s.data_.num_));
  return newTok;
}
double rebdev::token::getNum() const noexcept
{
  return data_.num_;
}
unsigned int rebdev::token::priority() const noexcept
{
  return priority_;
}

bool rebdev::token::leftBracket() const noexcept
{
  return (priority_ == 1);
}

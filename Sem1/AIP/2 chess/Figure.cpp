#include "Figure.hpp"

bool figure::operator()() const
{
  return (column >= 'A')
  && (column <= 'H')
  && (line >= 1)
  && (line <= 8);
}
bool figure::operator()(figure secondFigure) const
{
  return ((line-secondFigure.line) == 1)
  && ((((column-secondFigure.column) == 1))
  || (((column-secondFigure.column) == -1)));
}

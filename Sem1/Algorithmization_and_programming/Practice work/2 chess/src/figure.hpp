#ifndef FIGURE_HPP
#define FIGURE_HPP
class figure
{
  public: 
  int line;
  char column;
  
  bool operator()() const;
  bool operator()(figure secondFigure) const;
};
#endif

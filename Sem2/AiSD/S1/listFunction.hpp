#ifndef LISTFUNCTION_HPP
#define LISTFUNCTION_HPP

#include <iostream>
#include <utility>
#include <string>

#include <list.hpp>

namespace rebdev
{
  using numList = rebdev::BiList< size_t >;
  using nameAndNum = std::pair< std::string, numList >;
  using pairList = rebdev::BiList< nameAndNum >;
  using nameList = rebdev::BiList< std::string >;
  using listOfNumList = rebdev::BiList< numList >;
  using nameAndListOfNumList = std::pair< nameList, listOfNumList >;

  void inputList(std::istream & in, pairList & pList);
  void inputNumList(std::istream & in, numList & nList);
  nameList createNameList(pairList & pList);
  listOfNumList createListOfNumList(pairList & pList);
  void outputList(std::ostream & out, nameAndListOfNumList & pair);

  bool plus(size_t & f, size_t s);
}

#endif

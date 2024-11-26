#include "listFunction.hpp"

#include <limits>

void rebdev::inputList(std::istream & in, pairList & pList)
{
  std::string listName;
  while (in >> listName)
  {
    numList inNumList;
    inputNumList(in, inNumList);
    pList.push_back(nameAndNum(listName, inNumList));
    listName.clear();
  }
}

void rebdev::inputNumList(std::istream & in, numList & nList)
{
  size_t num = 0;
  while (in >> num)
  {
    nList.push_back(num);
  }
  in.clear();
}

rebdev::nameList rebdev::createNameList(pairList & pList)
{
  using pairCIter = pairList::const_iterator;
  pairCIter pairNow = pList.cbegin();
  nameList newNameList;

  while (pairNow != pList.cend())
  {
    newNameList.push_back((*pairNow).first);
    ++pairNow;
  }
  return newNameList;
}

rebdev::listOfNumList rebdev::createListOfNumList(pairList & pList)
{
  using pairIter = pairList::iterator;
  pairIter pairNow = pList.begin();

  size_t maxLength = 0;
  while (pairNow != pList.end())
  {
    maxLength = std::max(maxLength, ((*pairNow).second.size()));
    ++pairNow;
  }

  listOfNumList newListOfNumList;
  for (size_t i = 0; i < maxLength; ++i)
  {
    pairNow = pList.begin();
    numList localList;
    while (pairNow != pList.end())
    {
      if ((*pairNow).second.size() > 0)
      {
        localList.push_back((*pairNow).second.front());
        (*pairNow).second.pop_front();
      }
      ++pairNow;
    }
    newListOfNumList.push_back(localList);
  }
  return newListOfNumList;
}

void rebdev::outputList(std::ostream & out, nameAndListOfNumList & pair)
{
  if (pair.second.empty())
  {
    if (!pair.first.front().empty())
    {
      out << pair.first.front() << '\n';
    }
    out << "0\n";
    return;
  }

  using nameListCIter = nameList::const_iterator;
  nameListCIter nameNow = pair.first.cbegin();
  out << *nameNow;
  while (++nameNow != pair.first.cend())
  {
    out << ' ' << *nameNow;
  }
  out << '\n';

  using ListOfNumListCIter = listOfNumList::const_iterator;
  ListOfNumListCIter listOfNumNow = pair.second.cbegin();

  numList sumList;
  bool sumIsGood = true;
  while (listOfNumNow != pair.second.cend())
  {
    using numListCIter = numList::const_iterator;
    numListCIter numNow = (*listOfNumNow).cbegin();
    size_t sum = *numNow;

    out << *numNow;
    while (++numNow != (*listOfNumNow).cend())
    {
      out << ' ' << *numNow;
      sumIsGood &= plus(sum, *numNow);
    }
    out << '\n';

    ++listOfNumNow;
    sumList.push_back(sum);
  }

  if (sumIsGood)
  {
    out << sumList.front();
    sumList.pop_front();

    while (!sumList.empty())
    {
      out << ' ' << sumList.front();
      sumList.pop_front();
    }
    out << '\n';
  }
  else
  {
    throw std::logic_error("Overlow as a result of sum");
  }
}

bool rebdev::plus(size_t & f, size_t s)
{
  if (f > (std::numeric_limits< size_t >::max() - s))
  {
    return false;
  }

  f += s;
  return true;
}

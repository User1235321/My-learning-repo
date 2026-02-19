#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <utility>

struct NFA
{
  int numStates;
  int alphabetSize;
  int initialState;
  std::set< int > finalStates;
  std::vector< std::vector< std::pair< int, int > > > transitions;
};

int main()
{
  std::cout << "Введите количество состояний НКА: ";
  int numStates;
  std::cin >> numStates;

  std::cout << "Введите размер входного алфавита: ";
  int alphaSize;
  std::cin >> alphaSize;

  std::cout << "Введите начальное состояние: ";
  int initState;
  std::cin >> initState;

  std::cout << "Введите количество финальных состояний: ";
  int numFinal;
  std::cin >> numFinal;
  std::set< int > finalStates;
  for (int i = 0; i < numFinal; ++i)
  {
    int f;
    std::cin >> f;
    finalStates.insert(f);
  }

  std::vector< std::vector< std::pair< int, int > > > transitions(alphaSize);
  for (int sym = 0; sym < alphaSize; ++sym)
  {
    std::cout << "Для символа " << sym << " введите количество переходов: ";
    int numTrans;
    std::cin >> numTrans;
    std::cout << "Введите пары (from to):\n";
    for (int j = 0; j < numTrans; ++j)
    {
      int from, to;
      std::cin >> from >> to;
      transitions[sym].push_back(std::make_pair(from, to));
    }
  }

  std::map< std::set< int >, int > stateToIndex;
  std::vector< std::set< int > > indexToState;
  std::queue< int > q;

  std::set< int > startSet;
  startSet.insert(initState);
  stateToIndex[startSet] = 0;
  indexToState.push_back(startSet);
  q.push(0);

  while (!q.empty())
  {
    int curIdx = q.front();
    q.pop();
    std::set< int > curSet = indexToState[curIdx];

    for (int sym = 0; sym < alphaSize; ++sym)
    {
      std::set< int > nextSet;
      for (int s : curSet)
      {
        for (std::size_t k = 0; k < transitions[sym].size(); ++k)
        {
          if (transitions[sym][k].first == s)
          {
            nextSet.insert(transitions[sym][k].second);
          }
        }
      }
      if (!nextSet.empty())
      {
        std::map< std::set< int >, int >::iterator it = stateToIndex.find(nextSet);
        if (it == stateToIndex.end())
        {
          int newIdx = indexToState.size();
          stateToIndex[nextSet] = newIdx;
          indexToState.push_back(nextSet);
          q.push(newIdx);
        }
      }
    }
  }

  int dfaStatesCount = indexToState.size();
  std::vector< std::vector< int > > dfaTrans(dfaStatesCount,
                                              std::vector< int >(alphaSize, -1));

  for (int i = 0; i < dfaStatesCount; ++i)
  {
    std::set< int > curSet = indexToState[i];
    for (int sym = 0; sym < alphaSize; ++sym)
    {
      std::set< int > nextSet;
      for (int s : curSet)
      {
        for (std::size_t k = 0; k < transitions[sym].size(); ++k)
        {
          if (transitions[sym][k].first == s)
          {
            nextSet.insert(transitions[sym][k].second);
          }
        }
      }
      if (!nextSet.empty())
      {
        dfaTrans[i][sym] = stateToIndex[nextSet];
      }
    }
  }

  std::vector< bool > dfaFinal(dfaStatesCount, false);
  for (int i = 0; i < dfaStatesCount; ++i)
  {
    for (int s : indexToState[i])
    {
      if (finalStates.count(s))
      {
        dfaFinal[i] = true;
        break;
      }
    }
  }

  std::vector< char > stateLetters;
  for (int i = 0; i < dfaStatesCount; ++i)
  {
    stateLetters.push_back('A' + i);
  }

  std::cout << "\nПостроенный ДКА:\n";
  std::cout << "Количество состояний: " << dfaStatesCount << "\n";
  std::cout << "Начальное состояние: " << stateLetters[0] << " (множество {";
  bool first = true;
  for (int s : startSet)
  {
    if (!first) std::cout << ",";
    std::cout << s;
    first = false;
  }
  std::cout << "})\n";

  std::cout << "Состояния ДКА и соответствующие множества НКА:\n";
  for (int i = 0; i < dfaStatesCount; ++i)
  {
    std::cout << stateLetters[i] << ": {";
    first = true;
    for (int s : indexToState[i])
    {
      if (!first) std::cout << ",";
      std::cout << s;
      first = false;
    }
    std::cout << "}";
    if (dfaFinal[i]) std::cout << " (финальное)";
    std::cout << "\n";
  }

  std::cout << "\nТаблица переходов ДКА:\n";
  std::cout << "Состояние";
  for (int sym = 0; sym < alphaSize; ++sym)
  {
    std::cout << "\t" << sym;
  }
  std::cout << "\tВыход\n";
  for (int i = 0; i < dfaStatesCount; ++i)
  {
    std::cout << stateLetters[i];
    for (int sym = 0; sym < alphaSize; ++sym)
    {
      int next = dfaTrans[i][sym];
      if (next != -1)
      {
        std::cout << "\t" << stateLetters[next];
      }
      else
      {
        std::cout << "\t-";
      }
    }
    std::cout << "\t" << (dfaFinal[i] ? 1 : 0) << "\n";
  }

  return 0;
}

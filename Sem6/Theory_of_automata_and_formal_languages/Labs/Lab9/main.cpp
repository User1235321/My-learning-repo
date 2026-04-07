#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <vector>

struct Action
{
  int state;
  int rule;
  bool shift;
  bool accept;
};

class SLRParser
{
public:
  SLRParser()
  {
    for (int i = 0; i < 10; ++i)
    {
      for (char c : std::string("#ac"))
      {
        action[i][c] = {-1, -1, false, false};
      }
    }
    action[0]['#'] = {1, -1, true, false};
    action[1]['a'] = {3, -1, true, false};
    action[1]['c'] = {-1, 2, false, false};
    action[1]['#'] = {-1, 2, false, false};
    action[2]['#'] = {4, -1, true, false};
    action[3]['a'] = {3, -1, true, false};
    action[3]['c'] = {-1, 2, false, false};
    action[3]['#'] = {-1, 2, false, false};
    action[4]['#'] = {-1, -1, false, true};
    action[5]['c'] = {6, -1, true, false};
    action[6]['c'] = {-1, 1, false, false};
    action[6]['#'] = {-1, 1, false, false};

    gotoTable[1]['S'] = 2;
    gotoTable[3]['S'] = 5;
  }

  bool parse(const std::string& input)
  {
    std::string w = "#" + input + "#";
    std::stack< int > st;
    st.push(0);
    size_t ip = 0;
    int step = 0;

    std::cout << "\nПротокол разбора SLR(1) для цепочки \"" << w << "\":\n";
    std::cout << "Шаг | Стек (вершина справа) | Остаток входа | Действие\n";

    while (true)
    {
      ++step;
      int state = st.top();

      if (ip >= w.size())
      {
        if (action[state]['#'].accept)
        {
          std::cout << step << "   | " << stackToString(st) << " | конец | допуск\n";
          return true;
        }
        else
        {
          std::cout << step << "   | " << stackToString(st) << " | конец | ошибка\n";
          return false;
        }
      }

      char sym = w[ip];
      Action act = action[state][sym];

      if (act.accept)
      {
        std::cout << step << "   | " << stackToString(st) << " | " << w.substr(ip) << " | допуск\n";
        return true;
      }
      if (act.shift)
      {
        std::cout << step << "   | " << stackToString(st) << " | " << w.substr(ip) << " | сдвиг " << sym << "\n";
        st.push(act.state);
        ++ip;
      }
      else if (act.rule == 1)
      {
        std::cout << step << "   | " << stackToString(st) << " | " << w.substr(ip) << " | свёртка по правилу 1: S → a S c\n";
        st.pop(); st.pop(); st.pop();
        int topState = st.top();
        st.push(gotoTable[topState]['S']);
      }
      else if (act.rule == 2)
      {
        std::cout << step << "   | " << stackToString(st) << " | " << w.substr(ip) << " | свёртка по правилу 2: S → ε\n";
        int topState = st.top();
        st.push(gotoTable[topState]['S']);
      }
      else
      {
        std::cout << step << "   | " << stackToString(st) << " | " << w.substr(ip) << " | ошибка\n";
        return false;
      }
    }
  }

private:
  std::map< int, std::map< char, Action > > action;
  std::map< int, std::map< char, int > > gotoTable;

  std::string stackToString(std::stack< int > st)
  {
    std::string res;
    std::vector< int > vec;
    while (!st.empty())
    {
      vec.push_back(st.top());
      st.pop();
    }
    for (int i = vec.size() - 1; i >= 0; --i)
    {
      res += std::to_string(vec[i]) + " ";
    }
    return res;
  }
};

int main()
{
  std::cout << "Введите цепочку из a и c: ";
  std::string word;
  std::getline(std::cin, word);
  SLRParser parser;
  if (parser.parse(word))
  {
    std::cout << "\nСлово \"" << word << "\" принадлежит языку.\n";
  }
  else
  {
    std::cout << "\nСлово \"" << word << "\" НЕ принадлежит языку.\n";
  }
  return 0;
}

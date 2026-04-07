#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stack>

struct Rule
{
  std::string left;
  std::string right;
};

class PrecedenceParser
{
public:
  PrecedenceParser()
  {
    rules.push_back({"S'", "#S#"});
    rules.push_back({"S", "a"});
    rules.push_back({"S", "aT"});
    rules.push_back({"S", "[S]"});
    rules.push_back({"T", "b"});
    rules.push_back({"T", "bT"});

    std::vector< std::string > order = {"#", "a", "b", "[", "]", "S", "T"};
    for (std::string s : order)
    {
      symbols.push_back(s);
    }
  }

  void buildRelations()
  {
    for (const Rule& r : rules)
    {
      for (size_t i = 0; i + 1 < r.right.size(); ++i)
      {
        std::string leftSym = std::string(1, r.right[i]);
        std::string rightSym = std::string(1, r.right[i + 1]);
        eq[leftSym][rightSym] = true;
      }
    }

    std::map< std::string, std::set< std::string > > first;
    bool changed = true;
    while (changed)
    {
      changed = false;
      for (const Rule& r : rules)
      {
        std::string A = r.left;
        if (r.right.empty()) continue;
        std::string firstSym = std::string(1, r.right[0]);
        if (isTerminal(firstSym))
        {
          if (first[A].count(firstSym) == 0)
          {
            first[A].insert(firstSym);
            changed = true;
          }
        }
        else
        {
          for (std::string s : first[firstSym])
          {
            if (first[A].count(s) == 0)
            {
              first[A].insert(s);
              changed = true;
            }
          }
        }
      }
    }

    for (const Rule& r : rules)
    {
      for (size_t i = 0; i + 1 < r.right.size(); ++i)
      {
        std::string leftSym = std::string(1, r.right[i]);
        std::string rightSym = std::string(1, r.right[i + 1]);
        if (isNonTerminal(rightSym))
        {
          for (std::string t : first[rightSym])
          {
            lessRel[leftSym][t] = true;
          }
        }
      }
    }

    std::map< std::string, std::set< std::string > > last;
    changed = true;
    while (changed)
    {
      changed = false;
      for (const Rule& r : rules)
      {
        std::string A = r.left;
        if (r.right.empty()) continue;
        std::string lastSym = std::string(1, r.right[r.right.size() - 1]);
        if (isTerminal(lastSym))
        {
          if (last[A].count(lastSym) == 0)
          {
            last[A].insert(lastSym);
            changed = true;
          }
        }
        else
        {
          for (std::string s : last[lastSym])
          {
            if (last[A].count(s) == 0)
            {
              last[A].insert(s);
              changed = true;
            }
          }
          if (last[A].count(lastSym) == 0)
          {
            last[A].insert(lastSym);
            changed = true;
          }
        }
      }
    }

    for (const Rule& r : rules)
    {
      for (size_t i = 0; i + 1 < r.right.size(); ++i)
      {
        std::string leftSym = std::string(1, r.right[i]);
        std::string rightSym = std::string(1, r.right[i + 1]);
        if (isNonTerminal(leftSym))
        {
          for (std::string t : last[leftSym])
          {
            std::set< std::string > rightFirst;
            if (isTerminal(rightSym))
            {
              rightFirst.insert(rightSym);
            }
            else
            {
              rightFirst = first[rightSym];
            }
            for (std::string rfirst : rightFirst)
            {
              greaterRel[t][rfirst] = true;
            }
          }
        }
      }
    }
  }

  void printMatrix()
  {
    std::cout << "\nМатрица отношений предшествования:\n";
    std::cout << "     | # | a | b | [ | ] | S | T\n";
    std::cout << "---------------------------------\n";
    for (std::string row : symbols)
    {
      std::cout << "  " << row << " |";
      for (std::string col : symbols)
      {
        if (eq[row][col])
        {
          std::cout << " = |";
        }
        else if (lessRel[row][col])
        {
          std::cout << " < |";
        }
        else if (greaterRel[row][col])
        {
          std::cout << " > |";
        }
        else
        {
          std::cout << "   |";
        }
      }
      std::cout << "\n";
    }
  }

  bool parse(const std::string& input)
  {
    std::string w = input + "#";
    std::stack< std::string > st;
    st.push("#");
    size_t ip = 0;
    int step = 1;
    std::cout << "\nПротокол разбора слова \"" << input << "\":\n";
    std::cout << "Шаг | Стек (вершина справа) | Остаток входа | Отношение | Операция\n";
    std::cout << "----|-----------------------|----------------|-----------|---------\n";

    while (true)
    {
      if (ip >= w.size())
      {
        if (st.size() == 1 && st.top() == "S'")
        {
          std::cout << step << "   | " << st.top() << " |      |       | успех\n";
          return true;
        }
        if (st.size() == 3)
        {
          std::string top = st.top(); st.pop();
          std::string mid = st.top(); st.pop();
          std::string bot = st.top(); st.pop();
          if (bot == "#" && mid == "S" && top == "#")
          {
            std::cout << step << "   | #S# |      |       | свёртка #S# -> S'\n";
            st.push("S'");
            ++step;
            continue;
          }
          else
          {
            st.push(bot); st.push(mid); st.push(top);
          }
        }
        std::cout << "Ошибка: стек не пуст, вход закончен\n";
        return false;
      }

      std::string top = st.top();
      std::string nextSym = std::string(1, w[ip]);

      std::string rel;
      if (eq[top][nextSym])
      {
        rel = "=";
      }
      else if (lessRel[top][nextSym])
      {
        rel = "<";
      }
      else if (greaterRel[top][nextSym])
      {
        rel = ">";
      }
      else
      {
        std::cout << "Ошибка: нет отношения между " << top << " и " << nextSym << "\n";
        return false;
      }

      std::string stackStr;
      std::stack< std::string > tmp = st;
      std::vector< std::string > vec;
      while (!tmp.empty())
      {
        vec.push_back(tmp.top());
        tmp.pop();
      }
      for (int i = vec.size() - 1; i >= 0; --i)
      {
        stackStr += vec[i];
      }
      std::string rest = w.substr(ip);
      std::cout << step++ << "   | " << stackStr << " | " << rest << " | " << rel << "     | ";

      if (rel == "<" || rel == "=")
      {
        std::cout << "перенос " << nextSym << "\n";
        st.push(nextSym);
        ++ip;
      }
      else
      {
        std::string basis;
        while (true)
        {
          std::string top2 = st.top();
          st.pop();
          basis = top2 + basis;
          if (st.empty())
          {
            break;
          }
          std::string newTop = st.top();
          std::string leftRel;
          if (eq[newTop][top2])
          {
            leftRel = "=";
          }
          else if (lessRel[newTop][top2])
          {
            leftRel = "<";
          }
          else
          {
            leftRel = "?";
          }
          if (leftRel == "<")
          {
            break;
          }
        }

        bool found = false;
        std::string lhs;
        for (const Rule& r : rules)
        {
          if (r.right == basis)
          {
            lhs = r.left;
            found = true;
            break;
          }
        }
        if (!found)
        {
          std::cout << "ошибка: нет правила для " << basis << "\n";
          return false;
        }

        std::cout << "свёртка " << basis << " -> " << lhs << "\n";
        st.push(lhs);
      }
    }
  }

private:
  bool isTerminal(const std::string& s)
  {
    return (s == "a" || s == "b" || s == "[" || s == "]" || s == "#");
  }

  bool isNonTerminal(const std::string& s)
  {
    return (s == "S" || s == "T" || s == "S'");
  }

  std::vector< Rule > rules;
  std::vector< std::string > symbols;
  std::map< std::string, std::map< std::string, bool > > eq;
  std::map< std::string, std::map< std::string, bool > > lessRel;
  std::map< std::string, std::map< std::string, bool > > greaterRel;
};

int main()
{
  PrecedenceParser parser;
  parser.buildRelations();
  parser.printMatrix();

  std::cout << "\nВведите цепочку (терминалы a, b, [, ]): ";
  std::string word;
  std::getline(std::cin, word);

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

#include <iostream>
#include <vector>
#include <queue>

struct Automaton
{
  int statesCount;
  int inputAlphabetSize;
  int outputAlphabetSize;
  int initialState;
  std::vector< std::vector< int > > transition;
  std::vector< std::vector< int > > output;
};

bool areEquivalent(const Automaton & a1, const Automaton & a2)
{
  if (a1.inputAlphabetSize != a2.inputAlphabetSize)
  {
    std::cout << "Входные алфавиты не совпадают!\n";
    return false;
  }
  if (a1.outputAlphabetSize != a2.outputAlphabetSize)
  {
    std::cout << "Выходные алфавиты не совпадают!\n";
    return false;
  }

  int n1 = a1.statesCount;
  int n2 = a2.statesCount;
  int m = a1.inputAlphabetSize;

  std::vector< std::vector< bool > > visited(n1, std::vector< bool >(n2, false));
  std::queue< std::pair< int, int > > q;

  q.push(std::make_pair(a1.initialState, a2.initialState));
  visited[a1.initialState][a2.initialState] = true;

  int step = 0;
  while (!q.empty())
  {
    auto current = q.front();
    int s1 = current.first;
    int s2 = current.second;
    q.pop();
    ++step;

    std::cout << "Шаг " << step << ": состояние (" << s1 << ", " << s2 << ")\n";

    for (int a = 0; a < m; ++a)
    {
      int out1 = a1.output[s1][a];
      int out2 = a2.output[s2][a];

      std::cout << "  Вход " << a << ": выходы " << out1 << " и " << out2;

      if (out1 != out2)
      {
        std::cout << " -> РАЗЛИЧАЮТСЯ!\n";
        std::cout << "Автоматы не эквивалентны.\n";
        return false;
      }
      std::cout << " -> совпадают.\n";

      int next1 = a1.transition[s1][a];
      int next2 = a2.transition[s2][a];

      if (!visited[next1][next2])
      {
        visited[next1][next2] = true;
        q.push(std::make_pair(next1, next2));
        std::cout << "    Добавлено состояние (" << next1 << ", " << next2 << ")\n";
      }
    }
  }

  std::cout << "Все достижимые состояния обработаны, различий не найдено.\n";
  return true;
}

int main()
{
  Automaton a1, a2;

  std::cout << "=== Ввод автомата A ===\n";
  std::cout << "Количество состояний: ";
  std::cin >> a1.statesCount;
  std::cout << "Размер входного алфавита: ";
  std::cin >> a1.inputAlphabetSize;
  std::cout << "Размер выходного алфавита: ";
  std::cin >> a1.outputAlphabetSize;
  std::cout << "Начальное состояние: ";
  std::cin >> a1.initialState;

  a1.transition.resize(a1.statesCount, std::vector< int >(a1.inputAlphabetSize));
  a1.output.resize(a1.statesCount, std::vector< int >(a1.inputAlphabetSize));

  std::cout << "Таблица переходов (по строкам состояния, для каждого входа 0, 1, ...):\n";
  for (int i = 0; i < a1.statesCount; ++i)
  {
    for (int j = 0; j < a1.inputAlphabetSize; ++j)
    {
      std::cin >> a1.transition[i][j];
    }
  }

  std::cout << "Таблица выходов (по строкам состояния, для каждого входа 0, 1, ...):\n";
  for (int i = 0; i < a1.statesCount; ++i)
  {
    for (int j = 0; j < a1.inputAlphabetSize; ++j)
    {
      std::cin >> a1.output[i][j];
    }
  }

  std::cout << "\n=== Ввод автомата B ===\n";
  std::cout << "Количество состояний: ";
  std::cin >> a2.statesCount;
  std::cout << "Размер входного алфавита: ";
  std::cin >> a2.inputAlphabetSize;
  std::cout << "Размер выходного алфавита: ";
  std::cin >> a2.outputAlphabetSize;
  std::cout << "Начальное состояние: ";
  std::cin >> a2.initialState;

  a2.transition.resize(a2.statesCount, std::vector< int >(a2.inputAlphabetSize));
  a2.output.resize(a2.statesCount, std::vector< int >(a2.inputAlphabetSize));

  std::cout << "Таблица переходов:\n";
  for (int i = 0; i < a2.statesCount; ++i)
  {
    for (int j = 0; j < a2.inputAlphabetSize; ++j)
    {
      std::cin >> a2.transition[i][j];
    }
  }

  std::cout << "Таблица выходов:\n";
  for (int i = 0; i < a2.statesCount; ++i)
  {
    for (int j = 0; j < a2.inputAlphabetSize; ++j)
    {
      std::cin >> a2.output[i][j];
    }
  }

  std::cout << "\n=== Проверка эквивалентности ===\n";
  if (areEquivalent(a1, a2))
  {
    std::cout << "Результат: автоматы ЭКВИВАЛЕНТНЫ.\n";
  }
  else
  {
    std::cout << "Результат: автоматы НЕ ЭКВИВАЛЕНТНЫ.\n";
  }

  return 0;
}

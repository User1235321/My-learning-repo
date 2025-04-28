#include <iostream>

int main()
{
  const int arrSize = 8;
  int arr[arrSize];
  std::cout << "\033[1;32mGenerate " << arrSize << " random numbers:\033[0m\n";
  for (int i = 0; i < arrSize; ++i)
  {
    arr[i] = (std::rand() % 10);
    std::cout << arr[i] << ' ';
  };
  std::cout << "\n\n";

  for (int i = 0; i < (arrSize - 1); ++i)
  {
    std::cout << (i + 1) << ".\t";
    for (int x : arr) std::cout << x << ' ';
    std::cout << '\n';
    int j = i;
    while ((arr[j] > arr[j + 1]) && (j >= 0))
    {
      int c = arr[j];
      arr[j] = arr[j + 1];
      arr[j + 1] = c;
      std::cout << (i + 1) << '.' << (i - j + 1) << ".\t";
      for (int x : arr) std::cout << x << ' ';
      std::cout << '\n';
      --j;
    }
  }

  std::cout << "\n\033[1;32mResult of sorting:\033[0m\n";
  for (int x : arr) std::cout << x << ' ';
  std::cout << '\n';
  return 0;
}

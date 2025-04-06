#include <omp.h>
#include <iostream>

void func()
{
  for(int i= 0; i < 500000; i++)
  {
    std::rand();
  }
}
int main()
{
  size_t threadsNumber = 2;
  omp_set_num_threads(threadsNumber);
  #pragma omp parallel for
  for (int i = 0; i < 100; i++)
  {
    std::cout << 0;
    func();
  }
  #pragma omp parallel
  {
    #pragma omp sections nowait
    {
      #pragma omp section
      for (int i = 0; i < 10; i++)
      {
        std::cout << 1;
        func();
      }
      #pragma omp section
      for (int i = 0; i < 20; i++)
      {
        std::cout << 2;
        func();
      }
    }
    std::cout << '\n' << omp_get_num_threads() << '\n';
  }
  std::cout << '\n' << omp_get_num_threads() << '\n';
  #pragma omp barrier
  for (int i = 0; i < 10; i++)
  {
    std::cout << 3;
    func();
  }

  std::cout << "\nThird:\n";
  omp_set_num_threads(threadsNumber);
  int i = 0;
  #pragma omp parallel
  {
    #pragma omp atomic
      ++i;
    #pragma omp atomic
      ++i;
    #pragma omp critical
    {
      std::cout << i << '\n';
    }
  }

  omp_set_num_threads(threadsNumber);
  omp_lock_t lock;
  int n;
  omp_init_lock(&lock);
  #pragma omp parallel private (n)
  {
    n = omp_get_num_threads();
    while (!omp_test_lock(&lock))
    {
      std::cout << "Waiting\n";
      func();
    }
    std::cout << "Start working\n";
    func();
    std::cout << "End working\n";
    omp_unset_lock(&lock);
  }
  omp_destroy_lock(&lock);
}

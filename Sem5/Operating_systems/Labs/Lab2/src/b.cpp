#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>
#include <mutex>

std::mutex cout_mutex;

void thread_function(int thread_id)
{
  int local_var = 0;
  while (true)
  {
    {
      std::lock_guard<std::mutex> lock(cout_mutex);
      std::cout << "Поток " << thread_id << ": local_var = " << local_var << '\n';
    }
    ++local_var;
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }
}

int main()
{
  const int num_threads = 3;
  std::vector< std::thread > threads;
  std::cout << "Создание " << num_threads << " потоков.\n";
  for (int i = 1; i <= num_threads; ++i)
  {
    threads.emplace_back(thread_function, i);
  }
  for (auto& thread : threads)
  {
    thread.join();
  }
  return 0;
}

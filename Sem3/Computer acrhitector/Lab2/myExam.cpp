#include <iostream>
#include <stdio.h>

int main() {
  int arr1[10];
  int arr2[10];

  // Инициализация первого массива
  for (int i = 0; i < 10; i++) {
    arr1[i] = i * 2; 
  }

  // Ассемблерная вставка
  __asm {
  mov esi, offset (arr1)
  mov edi, offset (arr2)
  mov ecx, 10
  };
  loop_start:
  __asm {
  mov eax, [esi]
  mov [edi], eax
  inc esi
  inc edi
  loop loop_start
  };
  // Вывод результатов
  std::cout << "Первый массив: ";
  for (int i = 0; i < 10; i++) {
    std::cout << arr1[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "Второй массив: ";
  for (int i = 0; i < 10; i++) {
    std::cout << arr2[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}

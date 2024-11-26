#include <iostream>
#include <cstdlib>
#include <csignal>
#include <unistd.h>

// Функция-обработчик сигнала
void signalHandler(int signum) {
    std::cout << "Received signal: " << signum << std::endl;
}

int main() {
    // Создание процесс-потомка
    pid_t pid = fork();

    if (pid == -1) {
        // Обработка ошибки создания процесса
        std::cerr << "Failed to fork" << std::endl;
        return 1;
    }

    if (pid == 0) {
        // Код процесса-потомка

        // Установка обработчика сигнала для потомка
        signal(SIGUSR1, signalHandler);

        // Бесконечный цикл генерации и отправки сигналов родителю
        while (true) {
            // Генерация случайного числа от 1 до 3
            int randomSignal = rand() % 3 + 1;

            // Отправка сигнала родителю
            kill(getppid(), randomSignal);

            std::cout << "Child sent signal: " << randomSignal << std::endl;

            // Задержка для наглядности
            sleep(1);
        }
    } else {
        // Код процесса-родителя

        // Установка обработчика сигнала для родителя
        signal(SIGUSR1, signalHandler);

        // Бесконечный цикл генерации и отправки сигналов потомку
        while (true) {
            // Генерация случайного числа от 1 до 3
            int randomSignal = rand() % 3 + 1;

            // Отправка сигнала потомку
            kill(pid, randomSignal);

            std::cout << "Parent sent signal: " << randomSignal << std::endl;

            // Задержка для наглядности
            sleep(1);
        }
    }

    return 0;
}


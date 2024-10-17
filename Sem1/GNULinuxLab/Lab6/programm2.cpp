#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>

void signalHandler(int signal) {
    // Обработчик сигналов
    std::cout << "Процесс " << getpid() << " получил сигнал: " << signal << std::endl;
}

int main() {
    // Установка обработчика сигналов
    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, nullptr);

    // Создание процесса-потомка
    pid_t pid = fork();

    if (pid == -1) {
        std::cerr << "Ошибка при создании процесса-потомка" << std::endl;
        return 1;
    } else if (pid == 0) {
        // Код для процесса-потомка
        srand(getpid());

        for (int i = 0; i < 5; i++) {
            // Генерация случайного сигнала
            int signal = rand() % 10 + 1;

            // Отправка сигнала процессу-родителю
            kill(getppid(), signal);
            std::cout << "Процесс-потомок " << getpid() << " отправил сигнал " << signal << std::endl;

            // Задержка
            sleep(1);
        }
    } else {
        // Код для процесса-родителя
        srand(getpid());

        for (int i = 0; i < 5; i++) {
            // Генерация случайного сигнала
            int signal = rand() % 10 + 1;

            // Отправка сигнала процессу-потомку
            kill(pid, signal);
            std::cout << "Процесс-родитель " << getpid() << " отправил сигнал " << signal << std::endl;

            // Задержка
            sleep(1);
        }
    }

    return 0;
}

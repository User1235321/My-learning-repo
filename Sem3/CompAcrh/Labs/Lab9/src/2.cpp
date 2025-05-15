#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

int main() {
    int fd = open("/dev/tty", O_RDWR | O_NOCTTY); // Открываем терминал
    if (fd == -1) {
        perror("Ошибка открытия /dev/tty");
        return 1;
    }

    struct termios old_termios, new_termios;
    tcgetattr(fd, &old_termios); // Сохраняем текущие атрибуты терминала
    new_termios = old_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO); // Отключаем канонический режим и эхо
    new_termios.c_cc[VMIN] = 1;           // Минимальное количество символов для чтения (1)
    new_termios.c_cc[VTIME] = 0;          // Таймаут чтения (0)
    tcsetattr(fd, TCSANOW, &new_termios); // Устанавливаем новые атрибуты

    std::cout << "Ввод заблокирован. Подождите 10 секунд..." << std::endl;
    sleep(10); // Ждем 10 секунд

    tcsetattr(fd, TCSANOW, &old_termios); // Восстанавливаем старые атрибуты
    close(fd);
    std::cout << "Ввод разблокирован." << std::endl;
    return 0;
}

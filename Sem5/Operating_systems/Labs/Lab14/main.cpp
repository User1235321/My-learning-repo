#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>

struct Node {
    int data;
    off_t next;
};

struct Header {
    off_t head;
    off_t tail;
};

void initialize_file(int fd) {
    struct Header header = {0, 0};
    lseek(fd, 0, SEEK_SET);
    if (write(fd, &header, sizeof(header)) != sizeof(header)) {
        perror("write header");
        exit(1);
    }
}

void add_element(const char* filename, int value) {
    int fd = open(filename, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    // Проверяем, пустой ли файл
    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == 0) {
        // Файл новый - инициализируем заголовок
        initialize_file(fd);
    }
    lseek(fd, 0, SEEK_SET); // Возвращаемся в начало

    // Установка блокировки
    struct flock lock;
    memset(&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0; // Блокируем весь файл

    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("fcntl lock");
        close(fd);
        exit(1);
    }

    // Читаем заголовок
    struct Header header;
    if (read(fd, &header, sizeof(header)) != sizeof(header)) {
        perror("read header");
        close(fd);
        exit(1);
    }

    printf("До добавления: head=%ld, tail=%ld\n", header.head, header.tail);

    // Создание нового узла
    struct Node newNode;
    newNode.data = value;
    newNode.next = 0;

    // Перемещаемся в конец файла для записи нового узла
    off_t newOffset = lseek(fd, 0, SEEK_END);
    if (write(fd, &newNode, sizeof(newNode)) != sizeof(newNode)) {
        perror("write new node");
        close(fd);
        exit(1);
    }

    if (header.head == 0) {
        // Первый элемент в списке
        header.head = newOffset;
        header.tail = newOffset;
    } else {
        // Обновляем предыдущий tail
        struct Node oldTail;
        lseek(fd, header.tail, SEEK_SET);
        if (read(fd, &oldTail, sizeof(oldTail)) != sizeof(oldTail)) {
            perror("read old tail");
            close(fd);
            exit(1);
        }

        oldTail.next = newOffset;
        lseek(fd, header.tail, SEEK_SET);
        if (write(fd, &oldTail, sizeof(oldTail)) != sizeof(oldTail)) {
            perror("write updated tail");
            close(fd);
            exit(1);
        }
        header.tail = newOffset;
    }

    // Записываем обновленный заголовок
    lseek(fd, 0, SEEK_SET);
    if (write(fd, &header, sizeof(header)) != sizeof(header)) {
        perror("write updated header");
        close(fd);
        exit(1);
    }

    printf("После добавления: head=%ld, tail=%ld, newOffset=%ld\n",
           header.head, header.tail, newOffset);

    // Снятие блокировки
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
}

// Функция для печати списка (для проверки)
void print_list(const char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open for read");
        return;
    }

    struct Header header;
    if (read(fd, &header, sizeof(header)) != sizeof(header)) {
        perror("read header in print");
        close(fd);
        return;
    }

    printf("Содержимое списка в файле %s:\n", filename);
    printf("Заголовок: head=%ld, tail=%ld\n", header.head, header.tail);

    off_t current = header.head;
    int count = 0;
    
    while (current != 0 && count < 10) { // ограничение для избежания бесконечного цикла
        struct Node node;
        lseek(fd, current, SEEK_SET);
        if (read(fd, &node, sizeof(node)) != sizeof(node)) {
            printf("Ошибка чтения узла по смещению %ld\n", current);
            break;
        }
        
        printf("  Узел по смещению %ld: data=%d, next=%ld\n", 
               current, node.data, node.next);
        current = node.next;
        count++;
    }
    
    if (count == 0) {
        printf("  Список пуст\n");
    }
    
    close(fd);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Использование: %s <файл> <значение>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    int value = atoi(argv[2]);

    add_element(filename, value);
    printf("Элемент %d добавлен в файл %s\n", value, filename);
    
    // Печатаем список для проверки
    print_list(filename);

    return 0;
}

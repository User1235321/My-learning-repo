#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define SHM_SIZE 4096

int main() {
    int shm_fd;
    char *shared_memory;
    char filename[256];
    
    // Открываем разделяемую память
    shm_fd = shm_open("/my_shared_memory", O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("shm_open");
        printf("Make sure server is running first!\n");
        exit(1);
    }
    
    // Отображаем в адресное пространство
    shared_memory = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    
    printf("Client started. Enter filename (or 'exit' to quit):\n");
    
    while (1) {
        printf("> ");
        if (fgets(filename, sizeof(filename), stdin) == NULL) {
            break;
        }
        
        // Убираем символ новой строки
        filename[strcspn(filename, "\n")] = '\0';
        
        if (strcmp(filename, "exit") == 0) {
            break;
        }
        
        // Записываем имя файла в разделяемую память
        strncpy(shared_memory, filename, SHM_SIZE - 1);
        
        // Ждем ответ от сервера
        sleep(1);
        
        // Читаем и выводим результат
        printf("Result: %s\n", shared_memory);
    }
    
    // Очистка
    munmap(shared_memory, SHM_SIZE);
    close(shm_fd);
    
    return 0;
}

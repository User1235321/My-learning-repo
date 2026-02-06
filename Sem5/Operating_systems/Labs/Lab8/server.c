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
    
    // Создаем разделяемую память
    shm_fd = shm_open("/my_shared_memory", O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("shm_open");
        exit(1);
    }
    
    // Устанавливаем размер
    ftruncate(shm_fd, SHM_SIZE);
    
    // Отображаем в адресное пространство
    shared_memory = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    
    printf("Server started. Waiting for filename...\n");
    
    while (1) {
        // Ждем, пока клиент запишет имя файла
        if (strlen(shared_memory) > 0) {
            printf("Server received filename: %s\n", shared_memory);
            
            // Пытаемся открыть файл
            FILE *file = fopen(shared_memory, "r");
            if (file) {
                // Читаем содержимое файла
                char buffer[SHM_SIZE];
                size_t bytes_read = fread(buffer, 1, SHM_SIZE - 1, file);
                buffer[bytes_read] = '\0';
                fclose(file);
                
                // Записываем содержимое в разделяемую память
                strncpy(shared_memory, buffer, SHM_SIZE - 1);
            } else {
                // Записываем сообщение об ошибке
                snprintf(shared_memory, SHM_SIZE, "Error: Cannot open file '%s'", shared_memory);
            }
            
            // Очищаем для следующего запроса
            sleep(1); // Даем время клиенту прочитать
            shared_memory[0] = '\0';
        }
        usleep(100000); // 100ms задержка
    }
    
    // Очистка
    munmap(shared_memory, SHM_SIZE);
    close(shm_fd);
    shm_unlink("/my_shared_memory");
    
    return 0;
}

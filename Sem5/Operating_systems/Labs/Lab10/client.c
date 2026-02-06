#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>

#define SHM_NAME "/shm_example"
#define SEM_SERVER_NAME "/sem_server"
#define SEM_CLIENT_NAME "/sem_client"

typedef struct {
    char filename[256];
    char data[4096];
    int data_ready;
} shmem_data;

int main() {
    // Открытие разделяемой памяти
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    shmem_data *shmem = mmap(NULL, sizeof(shmem_data), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Открытие семафоров
    sem_t *sem_server = sem_open(SEM_SERVER_NAME, 0);
    sem_t *sem_client = sem_open(SEM_CLIENT_NAME, 0);

    char input[256];
    printf("Введите имя файла (или 'exit' для выхода): ");

    while (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = '\0';  // Удаление символа новой строки

        if (strcmp(input, "exit") == 0) {
            memset(shmem->filename, 0, sizeof(shmem->filename));
            sem_post(sem_client);
            break;
        }

        // Копирование имени файла в разделяемую память
        strncpy(shmem->filename, input, sizeof(shmem->filename) - 1);
        shmem->data_ready = 0;

        // Уведомление сервера
        sem_post(sem_client);

        // Ожидание ответа от сервера
        sem_wait(sem_server);

        // Вывод результата
        printf("Результат: %s\n", shmem->data);
        printf("Введите следующее имя файла (или 'exit' для выхода): ");
    }

    // Освобождение ресурсов
    munmap(shmem, sizeof(shmem_data));
    close(shm_fd);
    sem_close(sem_server);
    sem_close(sem_client);

    printf("Клиент завершил работу.\n");
    return 0;
}

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
    // Создание разделяемой памяти
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(shmem_data));
    shmem_data *shmem = mmap(NULL, sizeof(shmem_data), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Инициализация семафоров
    sem_t *sem_server = sem_open(SEM_SERVER_NAME, O_CREAT, 0666, 0);
    sem_t *sem_client = sem_open(SEM_CLIENT_NAME, O_CREAT, 0666, 0);

    printf("Сервер запущен. Ожидание имени файла...\n");

    while (1) {
        // Ожидание данных от клиента
        sem_wait(sem_client);

        // Проверка на завершение работы
        if (strlen(shmem->filename) == 0) break;

        printf("Сервер получил имя файла: %s\n", shmem->filename);

        // Попытка открыть файл
        FILE *file = fopen(shmem->filename, "r");
        if (file) {
            size_t bytes_read = fread(shmem->data, 1, sizeof(shmem->data) - 1, file);
            shmem->data[bytes_read] = '\0';
            fclose(file);
        } else {
            snprintf(shmem->data, sizeof(shmem->data), "Ошибка: файл '%s' не найден", shmem->filename);
        }

        shmem->data_ready = 1;
        sem_post(sem_server);  // Уведомление клиента
    }

    // Освобождение ресурсов
    munmap(shmem, sizeof(shmem_data));
    close(shm_fd);
    shm_unlink(SHM_NAME);
    sem_close(sem_server);
    sem_close(sem_client);
    sem_unlink(SEM_SERVER_NAME);
    sem_unlink(SEM_CLIENT_NAME);

    printf("Сервер завершил работу.\n");
    return 0;
}

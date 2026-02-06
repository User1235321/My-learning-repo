#include "shmem.h"

int main() {
    key_t key;
    int shmid, semid;
    Message *msgptr;

    // Получение ключа
    if ((key = ftok(KEY_FILE, KEY_ID)) < 0) {
        perror("ftok");
        exit(1);
    }

    // Создание разделяемой памяти
    if ((shmid = shmget(key, sizeof(Message), PERM | IPC_CREAT)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Присоединение разделяемой памяти
    if ((msgptr = (Message *)shmat(shmid, NULL, 0)) == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    // Создание семафоров
    if ((semid = semget(key, 3, PERM | IPC_CREAT)) < 0) {
        perror("semget");
        exit(1);
    }

    // Инициализация семафоров
    semctl(semid, MUTEX, SETVAL, 1);      // Мьютекс
    semctl(semid, CLIENT_READY, SETVAL, 0); // Ожидание клиента
    semctl(semid, SERVER_READY, SETVAL, 0); // Ожидание сервера

    while (1) {
        // Ожидание запроса от клиента
        wait_semaphore(semid, CLIENT_READY);

        // Блокировка разделяемой памяти
        wait_semaphore(semid, MUTEX);

        // Чтение имени файла из разделяемой памяти
        char *filename = msgptr->filename;

        // Попытка открыть файл
        FILE *file = fopen(filename, "rb");
        if (file == NULL) {
            // Ошибка: файл не найден
            char *error_msg = "Error: Cannot open file\n";
            strcpy(msgptr->data, error_msg);
            msgptr->data_size = strlen(error_msg);
        } else {
            // Чтение содержимого файла
            size_t nread = fread(msgptr->data, 1, MAXBUFF, file);
            msgptr->data_size = nread;
            fclose(file);
        }

        // Разблокировка разделяемой памяти
        signal_semaphore(semid, MUTEX);

        // Уведомление клиента о готовности данных
        signal_semaphore(semid, SERVER_READY);
    }

    // Отсоединение разделяемой памяти
    shmdt(msgptr);
    return 0;
}

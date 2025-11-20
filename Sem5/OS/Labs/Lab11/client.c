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

    // Доступ к разделяемой памяти
    if ((shmid = shmget(key, sizeof(Message), 0)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Присоединение разделяемой памяти
    if ((msgptr = (Message *)shmat(shmid, NULL, 0)) == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    // Доступ к семафорам
    if ((semid = semget(key, 3, 0)) < 0) {
        perror("semget");
        exit(1);
    }

    // Ввод имени файла
    printf("Enter file name: ");
    fgets(msgptr->filename, sizeof(msgptr->filename), stdin);
    msgptr->filename[strcspn(msgptr->filename, "\n")] = '\0'; // Удаление символа новой строки

    // Уведомление сервера о готовности данных
    signal_semaphore(semid, CLIENT_READY);

    // Ожидание ответа от сервера
    wait_semaphore(semid, SERVER_READY);

    // Блокировка разделяемой памяти для чтения
    wait_semaphore(semid, MUTEX);

    // Вывод данных из разделяемой памяти
    write(STDOUT_FILENO, msgptr->data, msgptr->data_size);

    // Разблокировка разделяемой памяти
    signal_semaphore(semid, MUTEX);

    // Отсоединение разделяемой памяти
    shmdt(msgptr);
    return 0;
}

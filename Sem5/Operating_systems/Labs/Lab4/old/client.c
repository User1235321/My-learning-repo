#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "shmem.h"

int main(void) {
    Message *msgptr;
    key_t key;
    int shmid, semid;
    
    // Получение ключа
    if((key = ftok("/home/pavel/Desktop/PolikekWorks/My-learning-repo/Sem5/OS/Labs/Lab3/test.txt", 'A')) < 0) {
        printf("Client: can't get a key\n");
        exit(-1);
    }
    
    // Получение доступа к разделяемой памяти
    if((shmid = shmget(key, sizeof(Message), 0)) < 0) {
        printf("Client: access denied\n");
        exit(-1);
    }
    
    // Присоединение разделяемой памяти
    if((msgptr = (Message*)shmat(shmid, 0, 0)) == (void*)-1) {
        printf("Client: error of joining\n");
        exit(-1);
    }
    
    // Получение доступа к семафору
    if((semid = semget(key, 2, PERM)) < 0) {
        printf("Client: access denied\n");
        exit(-1);
    }
    
    // Блокировка разделяемой памяти
    if(semop(semid, &mem_lock[0], 2) < 0) {
        printf("Client: can't execute a operation\n");
        exit(-1);
    }
    
    // Уведомление сервера о начале работы
    if(semop(semid, &proc_start[0], 1) < 0) {
        printf("Client: can't execute a operation\n");
        exit(-1);
    }
    
    // Запись сообщения в разделяемую память
    sprintf(msgptr->buff, "Message from client with PID = %d\n", getpid());
    
    // Освобождение ресурса
    if(semop(semid, &mem_unlock[0], 1) < 0) {
        printf("Client: can't execute a operation\n");
        exit(-1);
    }
    
    // Ожидание завершения работы сервера с разделяемой памятью
    if(semop(semid, &mem_lock[0], 2) < 0) {
        printf("Client: can't execute a operation\n");
        exit(-1);
    }
    
    // Чтение сообщения из разделяемой памяти
    printf("Client: read message\n%s", msgptr->buff);
    
    // Освобождение разделяемой памяти
    if(semop(semid, &mem_unlock[0], 1) < 0) {
        printf("Client: can't execute a operation\n");
        exit(-1);
    }
    
    // Отключение от области разделяемой памяти
    if(shmdt(msgptr) < 0) {
        printf("Client: error\n");
        exit(-1);
    }
    
    return 0;
}

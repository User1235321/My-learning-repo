#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "shmem.h"

Message *msgptr;
int shmid, semid;

void hndlr(int sig) {
    signal(SIGINT, hndlr);
    
    // Отключение от области разделяемой памяти
    if(shmdt(msgptr) < 0) {
        printf("Server: error\n");
        exit(-1);
    }
    
    // Удаление созданных объектов
    if(shmctl(shmid, IPC_RMID, 0) < 0) {
        printf("Server: can't delete area\n");
        exit(-1);
    }
    printf("Server: area is deleted\n");
    
    if(semctl(semid, 0, IPC_RMID) < 0) {
        printf("Server: can't delete semaphore\n");
        exit(-1);
    }
    printf("Server: semaphores are deleted\n");
    exit(0);
}

int main(void) {
    key_t key;
    signal(SIGINT, hndlr);
    
    // Получение ключа
    if((key = ftok("/home/pavel/Desktop/PolikekWorks/My-learning-repo/Sem5/OS/Labs/Lab3/test.txt", 'A')) < 0) {
        printf("Server: can't get a key\n");
        exit(-1);
    }
    
    // Создание области разделяемой памяти
    if((shmid = shmget(key, sizeof(Message), PERM | IPC_CREAT)) < 0) {
        printf("Server: can't create an area\n");
        exit(-1);
    }
    printf("Server: area is created\n");
    
    // Присоединение области
    if((msgptr = (Message*)shmat(shmid, 0, 0)) == (void*)-1) {
        printf("Server: error of joining\n");
        exit(-1);
    }
    printf("Server: area is joined\n");
    
    // Создание группы из 2 семафоров
    if((semid = semget(key, 2, PERM | IPC_CREAT)) < 0) {
        printf("Server: can't create a semaphore\n");
        exit(-1);
    }
    printf("Server: semaphores are created\n");
    
    while(1) {
        // Ожидание начала работы клиента
        if(semop(semid, &proc_wait[0], 1) < 0) {
            printf("Server: execution complete\n");
            exit(-1);
        }
        
        // Ожидание завершения работы клиента с разделяемой памятью
        if(semop(semid, &mem_lock[0], 2) < 0) {
            printf("Server: can't execute a operation\n");
            exit(-1);
        }
        
        // Вывод сообщения, записанного клиентом в разделяемую память
        printf("Server: read message\n%s", msgptr->buff);
        
        // Запись сообщения в разделяемую память
        sprintf(msgptr->buff, "Message from server with PID = %d\n", getpid());
        
        // Освобождение ресурса
        if(semop(semid, &mem_unlock[0], 1) < 0) {
            printf("Server: can't execute a operation\n");
            exit(-1);
        }
    }
    return 0;
}

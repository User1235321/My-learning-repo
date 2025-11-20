#ifndef SHMEM_H
#define SHMEM_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MAXBUFF 1024
#define PERM 0666

// Структура для разделяемой памяти
typedef struct {
    char filename[256];
    char data[MAXBUFF];
    int data_size;
} Message;

// Ключ для IPC
#define KEY_FILE "server"
#define KEY_ID 'A'

// Индексы семафоров
#define MUTEX 0
#define CLIENT_READY 1
#define SERVER_READY 2

// Операции с семафорами
void wait_semaphore(int semid, int sem_num);
void signal_semaphore(int semid, int sem_num);

#endif

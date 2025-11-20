#include "shmem.h"

void wait_semaphore(int semid, int sem_num) {
    struct sembuf op = {sem_num, -1, 0};
    semop(semid, &op, 1);
}

void signal_semaphore(int semid, int sem_num) {
    struct sembuf op = {sem_num, 1, 0};
    semop(semid, &op, 1);
}

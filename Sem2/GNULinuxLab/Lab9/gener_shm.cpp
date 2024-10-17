/* The program gener_shma.cpp */
/*
 * Allocating a shared memory segments
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(void)
{
  key_t	key = 15;
  int	shmid_1, shmid_2;

  if ((shmid_1 = shmget(key, 1000, 0644|IPC_CREAT)) == -1)
  {
    perror("shmget shmid_1");
    exit(1);
  }

  printf("First memory identifire is %d \n", shmid_1);

  if((shmid_2 = shmget(IPC_PRIVATE, 20, 0644)) == -1)
  {
    perror("shmget shmid_2");
    exit(2);
	}

  printf("Second shared memory identifire is %d \n", shmid_2);
  exit(0);
}

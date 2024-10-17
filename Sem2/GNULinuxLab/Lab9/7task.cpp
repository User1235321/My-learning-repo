#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1023

extern int etext, edata, end;

int main(void)
{
  int	shmid[3] = {0, 0, 0};
  char * shm[3] = {nullptr, nullptr, nullptr};
  for (size_t i = 0; i < 3; ++i)
  {
    if ((shmid[i] = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT|0666)) == -1)
    {
      perror("shmget fail");
      exit(1);
    }

    if ((shm[i] = (char *) shmat(shmid[i], 0, 0)) == "-1")
    {
      perror("shmat : parent ");
      exit(2);
    }

    printf("shared mem: %10p\n", shm[i]);
  }

  (*(shm[0] + 1022)) = 'a';
  (*(shm[0] + 1023)) = 'b';
  (*(shm[0] + 1024)) = 'c';
  printf("shared mem: %s\n", shm[0]);

  exit(0);
}

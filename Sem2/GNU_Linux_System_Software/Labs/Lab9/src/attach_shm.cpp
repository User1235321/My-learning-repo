/* The program attach_shm.cpp */
/* Creating, attaching and manipulating shared memory */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 30

extern int etext, edata, end;

int main(void)
{
  pid_t	pid;
  int	shmid;
  char c, *shm, *s;

  if ((shmid = shmget(IPC_PRIVATE,SHM_SIZE,IPC_CREAT|0666)) < 0)
  {
    perror("shmget fail");
    exit(1);
  }
  if ((shm = (char *) shmat(shmid, 0, 0)) == (char *) - 1)
  {
  perror("shmat : parent ");
  exit(2);
  }
  printf("Addresses in parent\n\n");
  printf("shared mem: %10p\n program text (etext): %10p\n initialized data (edata): %10p\n uninitialized data (end): %10p\n\n", shm, &etext, &edata, &end);
  s = shm; /* s now references shared mem */
  for (c = 'A'; c <= 'Z'; ++c) /* Put some info there */
    *s++ = c;
  *s = 0; /* Terminate the sequence */
  printf("In parent before fork, memory is : %s \n", shm);
  pid = fork();
  switch (pid)
  {
  case -1:
    perror("fork ");
    exit(3);
  default:
    sleep(3); /* Let the child finish*/
    printf("\nIn parent after fork, memory is  : %s\n", shm);
    printf("Parent removing shared memory\n");
    shmdt(shm);
    shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0);
    exit(0);
  case 0:
    printf("In child after fork, memory is : %s \n", shm);
    for (; *shm; ++shm)          /*  Modify shared memory   */
      *shm += 32;
    shmdt (shm);
  exit(0);
  }
}

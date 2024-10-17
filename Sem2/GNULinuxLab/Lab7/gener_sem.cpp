/* The program gener_sem.cpp */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

int main(void)
{
  int	sem1 = 0, sem2 = 0, sem3 = 0;
  char	u_char = 'S';
  key_t ipc_key;
  ipc_key = ftok(".", u_char);

  if ((sem1 = semget(ipc_key, 3, IPC_CREAT | 0666)) == -1)
  {
    perror("semget: IPC_CREAT | 0666");
  }

  printf("sem1 identifier is %d\n", sem1);

  if((sem2 = semget(ipc_key, 3, IPC_CREAT | IPC_EXCL | 0666)) == -1)
  {
    perror("semget: IPC_CREATE | IPC_EXCL | 0666");
  }
  printf("sem2 identifier is %d\n", sem2);

  if((sem3 = semget(IPC_PRIVATE, 3, 0600)) == -1)
  {
    perror("semget: IPC_PRIVATE");
  }

  printf("sem3 identifier is %d\n", sem3);
  exit(0);
}

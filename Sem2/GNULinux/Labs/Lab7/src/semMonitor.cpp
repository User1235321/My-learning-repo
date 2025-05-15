#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

int main(void)
{
	key_t key;
	int semid;

  while (1)
  {
    if ((key = ftok(".", 'J')) == -1)
    {
      perror("ftok");
      exit(1);
    }

    if ((semid = semget(key, 1, 0)) == -1)
    {
      perror("semget");
      exit(1);
    }

    int semNum = semctl(semid, 0, GETNCNT);
    printf("%i", semNum);
    sleep(1);
    printf("\n");
  }
	return 0;
}

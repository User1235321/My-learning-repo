/* The program gener_mq.cpp */
/* Message queue generation */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX 5

int main(void){
  FILE *fin;
  char	buffer[PIPE_BUF];
  char	u_char = 'A';
  int	i, n, mid[MAX];
  key_t	key;

  for (i = 0; i < MAX ; ++i, ++u_char)
  {
    key = ftok(".", u_char);
    if ((mid[i] = msgget(key, IPC_CREAT | 0660)) == -1)
    {
      perror("Queue create");
      exit(1);
    }
  }

  fin = popen("ipcs", "r");	/* Run the ipcs command */
  while((n = read(fileno(fin), buffer, PIPE_BUF)) > 0)
    write(fileno(stdout), buffer, n);
    /*Display the ipcs output */

  pclose(fin);

  for (i = 0; i < MAX; ++i)
    msgctl(mid[i], IPC_RMID, NULL);
  /* remove */

  exit(0);
}









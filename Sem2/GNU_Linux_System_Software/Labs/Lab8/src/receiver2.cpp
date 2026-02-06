#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <iostream>

struct my_msgbuf
{
	long mtype;
	char mtext[200];
};

int main(void)
{
	struct my_msgbuf buf;
	int msqid;
	key_t key;

	if ((key = ftok(".", 'B')) == -1)
  {  /* same key as sender.cpp */
		perror("ftok");
		exit(1);
	}

	if ((msqid = msgget(key, 0644)) == -1)
  { /* connect to the queue */
		perror("msgget");
		exit(1);
	}

	printf("write num of receiver2 procces:\n");

  char receiverNum = '1';
  std::cin >> receiverNum;

	for(;;)
  { /* Spock never quits! */
		if (msgrcv(msqid, &buf, sizeof(buf.mtext), (2 - (receiverNum == '1')), 0) == -1)
    {
			perror("msgrcv");
			exit(1);
		}

		printf("spock: \"%s\"\n", buf.mtext);
	}

	return 0;
}

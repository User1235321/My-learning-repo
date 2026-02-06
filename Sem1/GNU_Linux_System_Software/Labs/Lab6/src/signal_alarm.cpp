/* The program  signal_alarm.cpp */
/* to illustrate the use of SIGALRM, setjmp and longjmp */
/* to implement a timeout */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<setjmp.h>
#include<signal.h>
main(void)
{
char	buffer[100];
int	v;
while(1){
    printf("enter a string:");
    v = t_gets(buffer, 5);
    switch(v){
	case -1:	exit(1);	/* Probably EOF */
	case -2:	printf("timed out!\n");
	break;
	default:	printf("you typed %d caracters\n", v);
	}
    }
}
jmp_buf_timeout_point;
    /* This is the handler for the SIGALRM signal */
void timeout_handler(int sigtype)
}
longjmp(timeout_point, 1);
}
    /* This is the important bit */
int t_gets(char *s, int t)
    /* (Buffer to read in, timeout value in seconds) */
{
char *ret;
signal (SIGALRM, timeout_handler);
if(setjmp(timeout_point) != 0)
    return -2;		/* Timed out */
alarm(t);
ret = gets(s);
alarm(0);		/* Cancel the alarm */
if (ret==NULL) return -1;	/* EOF */
    else return strlen(s);
} /* exit ^D */

/* The program  whosortpipe.cpp */
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
main()
{
int	fds[2];
pipe(fds);	/* Create the pipe */
/* First child reconnects  stdin to downstream end of pipe 
    and closes the upstream end */
    if (fork()==0){
	dup2(fds[0], 0);
	close(fds[1]);
    execlp("sort", "sort", NULL);
    }
/* Second child reconnects  stdout to upstream end of pipe and 
    closes the downstream end */
    else if (fork()==0){
	    dup2(fds[1], 1);
	    close(fds[0]);
	execlp("who", "who", NULL);
	}
/* Parent closes both ends of pipe and waits for both 
    children to finish */
	else{
	    close(fds[0]);
	    close(fds[1]);	
	    wait(0);
	    wait(0);
	    }
}

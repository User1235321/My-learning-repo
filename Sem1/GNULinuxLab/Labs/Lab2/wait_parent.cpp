/* The program  wait_parent.cpp */
/* A parent process waits for a child to finish */ 
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
int main()
{
pid_t	pid, w;
int	i, status;
char	value[3];	/*place to store index as string */
    for(i=0; i<3; ++i)
    {		/* Gener. 3 child processes */
    if ((pid=fork())==0){
	sprintf(value, "%d", i);
	execl("wait_child", "wait_child", value, (char *)0);
	}
	else		/* assuming no falures here ...*/
	    printf("Forked child %d\n", pid);
    }
		/* Wait for the children */
    while((w=wait(&status)) && w!=-1){
	if(w!=-1)
	    printf("Wait on PID: %d returns status of: %04X\n", w, status);
	}
	exit(0);
}

	
	
    
    
    
    
    

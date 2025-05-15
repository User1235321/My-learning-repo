/* The program  wait_child.cpp */
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<signal.h>
int main(int argc, char *argv[])
{
pid_t	pid;
int	ret_value;
pid  =  getpid();
ret_value = (int) (pid % 256);
srand((unsigned) pid);
sleep(rand() %5);
if(atoi(*(argv+1)) % 2){
	/* assuming argv[1] exists! */ 	
    printf("Child %d is terminating with signal 0009\n", pid);
    kill(pid, 9);	/* hara-kiri */
    }
    else{
	printf("Child %d is terminating with exit (%04X)\n", pid, ret_value);
	exit(ret_value);
	}
}
    
    
    
    
    

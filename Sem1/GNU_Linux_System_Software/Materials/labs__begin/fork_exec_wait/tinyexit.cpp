/* The program  tinyexit.cpp */
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
main()
{
	/* Hard-wired list of commands */ 	
static char *cmmd[3];
cmmd[0]=(char *)"who";
cmmd[1]=(char *)"ls";
cmmd[2]=(char *)"date";
int i;
    while(1){
	/* Prompt for read a command number */	
    printf("0=who, 1=ls, 2=date:");
    scanf("%d",&i);
	/* If selection is invalid, parent terminates */
	if(i<0 || i>2)
		exit(2);
	if (fork()==0){		/* Child */  
	/* The child executes the selected command */
    execlp(cmmd[i], cmmd[i], NULL);
    printf("Command not found\n");
	/* exec failed */
    exit(1);
    }     	
    else
	{	/* The parent waits for the child to finish */
	wait(0);
	}
    }
}	
	
	
    
    
    
    
    

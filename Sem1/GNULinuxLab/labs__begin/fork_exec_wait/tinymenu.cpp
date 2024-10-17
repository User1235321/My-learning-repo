/* The program  tinymenu.cpp */
#include<stdio.h>
#include<unistd.h>
main()
{
	/* Hard-wired list of commands */
static char *cmd[]={(char *)"who",(char *)"ls",(char *)"date"};
int i;
	/* Prompt for read a command number */
printf("0=who, 1=ls, 2=date:");
scanf("%d",&i);
	/* Execute the selected command */
    execlp(cmd[i], cmd[i], (char *)0);
printf("Command not found\n");
	/* exec failed */
}     	

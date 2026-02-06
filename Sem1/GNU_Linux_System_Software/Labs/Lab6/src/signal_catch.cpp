/* The program  signal_catch.cpp */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
int main(void)
{
int	i;
void	signal_catcher(int);
if(signal(SIGINT, signal_catcher)==SIG_ERR){
    perror("SIGINT");
    exit(1);
    }
if(signal(SIGQUIT, signal_catcher)==SIG_ERR){
    perror("SIGQUIT");
    exit(2);
    }
for(i=0; ;++i){		/* Forever... */
    printf("%i\n", i);	/* Display a number */
    sleep(1);
    }
}
void
signal_catcher(int the_sig){
    signal(the_sig, signal_catcher);	/* Reset */
    printf("\nSignal %d received.\n", the_sig);
    if(the_sig==SIGQUIT)
    exit(3);
}

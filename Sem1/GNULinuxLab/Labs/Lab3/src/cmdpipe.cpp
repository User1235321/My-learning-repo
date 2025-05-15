/* The program  cmdpipe.cpp */
/* Using the popen and pclose I/O commands */
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
//#include<sys/types.h>
#include<limits.h>
int main(int argc, char *argv[])
{
FILE	*fin, *fout;
char	buffer[PIPE_BUF];
int	n;
    if(argc<3){
	fprintf(stderr, "Usage %s cmd1 cmd2\n", argv[0]);
	exit(1);
	}
fin  = popen(argv[1], "r");
fout = popen(argv[2], "w");
while((n = read(fileno(fin), buffer, PIPE_BUF))>0)
    write(fileno(fout), buffer, n);   
pclose(fin);
pclose(fout);
exit(0);
}

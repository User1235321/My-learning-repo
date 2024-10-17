/* The server program  pipe_server.cpp */
#include"pipe_local.h"
void main(void)
{
int	n, done, dummyfifo, privatefifo, publicfifo;
static char buffer[PIPE_BUF];
FILE *fin;
struct message msg;
        /* Generate the public FIFO */
mknod(PUBLIC, S_IFIFO | 0666, 0);
	/* OPEN the public FIFO for reading and writing */
if ((publicfifo=open(PUBLIC, O_RDONLY))==-1) || 
     (dummyfifo=open(PUBLIC, O_WRONLY | O_NDELAY))==-1){
    perror(PUBLIC);
    exit(1);
    }
    /* Message can be read from the PUBLIC pipe */
while(read(publicfifo, (char *) &msg, sizeof(msg))>0){
    n = done = 0; /* Clear counters | flags */
    do{		/* Try OPEN of private FIFO */
	if ((privatefifo=open(msg.fifo_name, O_WRONLY | O_DELAY))==-1
	sleep(3);	/* Sleep a while  */
	else{	/* OPEN succesful */
	    fin = popen(msg.cmd_line, "r");	/* Execute the cmd */
	    write(privatefifo, "\n", 1);	/* Keep output pretty */
	    while((n=read(fileno(fin), buffer, PIPE_BUF))>0){
		write(privatefifo, buffer, n);	/*to private FIFO */
		memset(buffer, 0x0, PIPE_BUF);	/* Clear it out */
		}
		pclose(fin);
		close(privatefifo);
		done = 1;	/* Record succes */
	    }
	}while(++n<5 && !done);
	
	if(!done)		/* Indicate failure */
	    write(fileno(stderr), "\nNOTE: SERVER ** NEVER ** 
				accessed private FIFO\n", 48);
    }
}

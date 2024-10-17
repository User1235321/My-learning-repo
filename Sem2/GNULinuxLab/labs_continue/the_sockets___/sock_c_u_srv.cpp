/* The program sock_c_u_srv.cpp */
/*
 * Server - UNIX domain, connection oriented
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#define NAME "my_sock"

void	clean_up( int, char *); /* Close socket and remove routine */

main( void ) {
int		orig_sock, /* Original socket descriptor */
		new_sock,  /* New socket descriptor from connect */
		clnt_len,  /* Lenght of client address */
		i,	   /* Loop counter */
                cpid;      /* PID */
static struct sockaddr_un
		clnt_adr,  /* UNIX addresses of client-server */
		serv_adr;
static char 	buf[10];   /* Buffer for messages */

if((orig_sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) { /* SOCKET */
	perror("Generate error");
	exit(1);
}
serv_adr.sun_family = AF_UNIX;  /* Set tag appropriately	*/
strcpy(serv_adr.sun_path,NAME); /* Assign name(108 chars max)	*/
unlink(NAME);                   /* Remove old copy,if present	*/

if(bind(orig_sock, (struct sockaddr *) &serv_adr,	 /* BIND */
	sizeof(serv_adr.sun_family)+strlen(serv_adr.sun_path)) < 0) {
	perror("bind error");
	clean_up(orig_sock,NAME);
	exit(2);
};
listen(orig_sock,3);					/* LISTEN */
clnt_len = sizeof(clnt_adr);

if ((new_sock = accept(orig_sock, (struct sockaddr *) &clnt_adr,
				&clnt_len)) < 0) {  	/* ACCEPT */
        	 	perror("accept error");
	        	clean_up(orig_sock, NAME);	
		        exit(3);
               
                }
	/*printf("new sock = %d\n",new_sock);*/
       	for(i=1; i<= 10; i++){ 				/* PROCESS */
		sleep(1);
		read(new_sock, buf, sizeof(buf));
		printf("s-> %s", buf);
  
 	}

         
         close(new_sock);
	 clean_up(orig_sock, NAME);
		exit(0);
}

void 
clean_up(int sd, char *the_file ) {
	close(sd);					/* Close it */
	unlink(the_file);				/* rm it    */
}
	


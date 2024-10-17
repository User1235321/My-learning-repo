/* The program sock_c_u_clt.cpp */
/*
 * Client - Unix domain, connection oriented
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#define NAME "my_sock"

main(void) {
	int	orig_sock,/* Original socket descriptor in client */
		i;	  /* Loop counter			 */
static struct	sockaddr_un
		serv_adr; /* Unix address of the server process */
static	char	buf[10];  /* Buffer for messages	       */

if ((orig_sock = socket(AF_UNIX, SOCK_STREAM,0)) < 0) { /* SOCKET */
	perror("generate error");
	exit(1);
}
serv_adr.sun_family = AF_UNIX;  /* Set tag appropriately	*/
strcpy(serv_adr.sun_path,NAME); /* Assign name			*/

if (connect(orig_sock, (struct sockaddr *) &serv_adr, /* CONNECT */
	sizeof(serv_adr.sun_family)+strlen(serv_adr.sun_path))<0) {
		perror("connect error");
		exit(1);
}
for(i=1;i<=10;i++){			/* Send messages */
	sprintf(buf,"c: %d\n",i);
	write(orig_sock,buf,sizeof(buf));
}
close(orig_sock);
exit(0);
}

/* The program sock_c_i_srv.cpp */
/*
 * Server - Internet domain, connection-oriented 
 */
#include "local_c_i.h"
main(void) {
int		orig_sock,/* Original socket descriptor in server */
		new_sock, /* New socket descriptor from connect	 */
		clnt_len; /* Length of client address		*/
struct	sockaddr_in
		clnt_adr, /* Internet address of client & server */
		serv_adr;
int		len,i;    /* Misc counters,etc			*/
if((orig_sock=socket(AF_INET,SOCK_STREAM,0)) < 0) {/*SOCKET     */
	perror("generate error");
	exit(1);
};
memset(&serv_adr, 0 , sizeof(serv_adr));	/* Clear structure  */
serv_adr.sin_family	=AF_INET;		/* Set address type */
serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);	/* Any interface    */
serv_adr.sin_port	=htons(PORT);		/* Use our fake port*/
						/* BIND		    */
if (bind(orig_sock, (struct sockaddr *) &serv_adr,
		sizeof(serv_adr)) < 0){
	perror("bind error");
	close(orig_sock);
	exit(2);
};
if (listen(orig_sock,5)<0){ 			  /* LISTEN */
	perror("listen error");
	exit(3);
};
do {
	clnt_len = sizeof(clnt_adr);
	if ((new_sock = accept(orig_sock,(struct sockaddr *) &clnt_adr, (socklen_t *)&clnt_len)) < 0) {	  /* ACCEPT */
		perror("accept error");
		close(orig_sock);
		exit(4);
	};
	if ( fork () == 0) {		  	  /* In child process */
		while ((len=read(new_sock,buf,BUFSIZ))>0){
			for(i=0;i<len;++i)	  /* Change the case   */
				buf[i] = toupper(buf[i]);
			write(new_sock,buf,len);  /* Write it back	    */
			if (buf[0] == '.') break; /* Are we done yet?*/ 
		}
		close(new_sock);		/* In child process  */
		exit(0);
	} else close(new_sock);			/* In parent process */
     }while(1);					/* Forever*/
}

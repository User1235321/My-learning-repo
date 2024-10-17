/* The program get_serv.cpp */
/*
 * Checking service  --port entries for a host */
 */
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>

main(void) {
	struct servent	*serv;
	static char protocol[10], service[10];
	printf("Enter service to look up: ");
	scanf("%9s", sevice);
	printf("Enter protocol to look up:");
	scanf("%9s", protocol);
	serv = getservbyname(sevice, protocol);
	if (serv != (struct servent *) NULL) {
		printf("Here is what I found\n");
		printf("Official name 	: %s\n", serv->s_name);
		printf("Aliases		:");
		while (*serv->s_aliases) {
			printf("%s", *serv->s_aliases);
			++serv->s_aliases;
		    }
		printf("\nPort number    : %i\n", htons(serv->s_port));
		printf  ("Protocol Family: %s\n\n", serv->s_proto);
	    }
	    else
		printf("Service %s for   protocol %s not found\n",
					service, protocol);
} 

/* The program  rlimit.cpp */
/* to illustrate the maximum and current limits in use */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

void disp_limit(int resource, char *rname);
{
	struct rlimit rlm;
	getrlimit(resource, &rlm);
	printf(“%-13s ”, rname);
	/* The value of soft limit */
	if (rlm.rlim_cur == RLIM_INFINITY)	
		printf(“infinite   ”);
	else
		printf(“%101d  ”, rlm.rlim_cur);
	/* The value of hard limit */
	if (rlm.rlim_max == RLIM_INFINITY)	
		printf(“infinite  \n”);
	else
		printf(“%101d  ”, rlm.rlim_max);
}

main()
{
	disp_limit(RLIMIT_CORE, “RLIMIT_CORE”);
	disp_limit(RLIMIT_CPU, “RLIMIT_CPU”);
	disp_limit(RLIMIT_DATA, “RLIMIT_DATA”);
	disp_limit(RLIMIT_FSIZE, “RLIMIT_FSIZE”);
	disp_limit(RLIMIT_NOFILE, “RLIMIT_NOFILE”);
	disp_limit(RLIMIT_STACK, “RLIMIT_STACK”);
		
	/* BSD */
 	#ifdef RLIMIT_NPROC
		 disp_limit(RLIMIT_NPROC, “RLIMIT_NPROC”);
	#endif

	/* BSD */
 	#ifdef RLIMIT_RSS
		 disp_limit(RLIMIT_RSS, “RLIMIT_RSS”);
	#endif
	/* BSD */
 	#ifdef RLIMIT_MEMLOCK
		 disp_limit(RLIMIT_MEMLOCK, “RLIMIT_MEMLOCK”);
	#endif
	/* System V */
 	#ifdef RLIMIT_VMEM
		 disp_limit(RLIMIT_VMEM, “RLIMIT_VMEM”);
	#endif
}





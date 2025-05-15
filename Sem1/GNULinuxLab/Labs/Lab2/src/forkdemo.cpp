#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int main()
{
 int i;
  if (fork()) {    /* I must be the parent  */
      for(i=0; i<1000 ; i++)
          printf("\t\tPARENT %d\n", i);
      }
       else {      /* I must be the child   */
        for(i=0; i<1000 ; i++)
            printf("CHILD %d\n",i);
        }
}      

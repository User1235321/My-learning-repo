/* rtest.c
 * Test the random number generator.
 */

#include <stdio.h>
#include "random.h"

main ()
{
int    seed, i, n;
double rnd;

n = 10;
seed = 3;
rnd = random01(&seed);
printf ("seed = %d\n", seed);

for (i = 0; i < n; ++i)
   {
   printf ("i = %d, rnd = %f \n", i, random01(&seed) );
   }

printf ("Press <RETURN>...");
getchar ();

return 0;
}
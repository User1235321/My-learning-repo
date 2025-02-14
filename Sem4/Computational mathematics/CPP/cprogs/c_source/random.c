/* random.c
 * Random number routines.
 * ------------------------------------------------------------
 *
 * P. A. Jacobs
 * Department of Mechanical Engineering
 * University of Queensland
 *
 */

#include <stdio.h>
#include "random.h"

#define  MBIG    1000000000
#define  MSEED    161803398
#define  MZ               0
#define  FAC   (1.0 / MBIG)

/*--------------------------------------------------------------*/

double random01 (int *restart)
/*
 * Purpose...
 * -------
 * Provide uniformly distributed random numbers between 0.0 and 1.0.
 *
 * Input...
 * -----
 * *restart  : Set negative to initialize or restart the sequence.
 *             The value is also used as part of the seed on the
 *             first call.
 *
 * Output...
 * ------
 * random01() returns a double value as the next random number.
 *
 * Reference...
 * ---------
 * Based on the translated code ran3() in
 * Press et al. Numerical Recipes in C (1989), Cambridge.
 * In turn, they used code from
 * D. E. Knuth Seminumerical Algorithms (1981), Addison-Wesley
 *
 */

{  /* Begin random01() */

static int  inext, inextp;
static long ma[56];
static int  iff = 0;
long   mj, mk;
int    i, ii, k;

/*
 * Initialization
 */
if (*restart < 0 || iff == 0)
   {
   iff = 1;   /* leave our mark */

   /*
    * Initialize ma[55] using the seed *restart and the large
    * number MSEED.
    */
   mj = MSEED - (*restart < 0 ? -(*restart) : (*restart) );
   mj %= MBIG;
   ma[55] = mj;

   /*
    * Now, initialize the rest of the table, in a slightly
    * random order, but with numbers that are not especially
    * random.
    */
   mk = 1;
   for (i = 1; i <= 54; i++)
      {
      ii = (21 * i) % 55;
      ma[ii] = mk;
      mk = mj - mk;
      if (mk < MZ) mk += MBIG;
      mj = ma[ii];
      /* printf ("ma[%d] = %ld, ", ii, ma[ii]); */
      }

   /*
    * Randomize the numbers.
    */
   for (k = 1; k <= 4; k++)
      {
      for (i = 1; i <= 55; i++)
         {
         ma[i] -= ma[1 + (i+30) % 55];
         if (ma[i] < MZ) ma[i] += MBIG;
         }
      }

   /*
    * Prepare indices for the first random number.
    * Note that the value 31 is special.
    */
   inext = 0;
   inextp = 31;
   *restart = 1;
   }  /* if (*restart... */

/*
 * ----------
 * Start here, except for initialization or restarting.
 * ----------
 *
 * increment the indices, with wraparound
 */
if (++inext == 56) inext = 1;
if (++inextp == 56) inextp = 1;

/*
 * Generate a new random number subtractively, make sure it is in
 * range and then store it.
 */
mj = ma[inext] - ma[inextp];
if (mj < MZ) mj += MBIG;
ma[inext] = mj;

/*
 * Return the derived uniform deviate as a double value
 * between 0.0 and 1.0.
 */
return (mj * FAC);
}  /* end of random01() */

/*--------------------------------------------------------------*/

int flip (double p)
/*
 * Purpose...
 * -------
 * Biased coin toss.
 *
 * Input...
 * -----
 * p    : probability of returning a 1
 *
 * Output...
 * ------
 * Return a 1 with probability p
 *          0 with probability (1.0 - p)
 */
{
double rn;
int    restart;

if (p == 1.0) return 1;

/*
 * Using the standard-library generator...
 * rn = ( (double) rand() ) / ( (double) RAND_MAX + 1.0 );
 */

/*
 * Call the portable random number generator.
 */
restart = 1;
rn = random01 ( &restart );

/*
 * Return the result of the coin toss.
 */
if (rn <= p) return 1; else  return 0;

}  /* end of flip() */

/*--------------------------------------------------------------*/

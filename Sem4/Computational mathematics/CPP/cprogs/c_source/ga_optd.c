/* gatest.c
 * Test driver for ga_optimize() : Genetic Algorithm Optimizer.
 * ----------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ga_opt.h"

double objfunc (int n, double x[]);

/*-------------------------------------------------------------*/

main ()
{
struct ga_data gad;
double fbest, xbest[10];
int    jx;

printf ("\n\nTest driver for ga_optimize()...\n");

/*
 * Set parameters for the GA
 */
gad.popsize = 100;                  /* size of populations      */
gad.nx      = 1;                    /* number of parameters     */
gad.xbits   = 30;                   /* bits in a parameter      */
gad.maxgen  = 50;                   /* number of generations    */
gad.spin    = 5;                    /* stop if no improvement   */
                                    /* in spin generations      */

gad.pcross    = 0.6;                /* probability of crossover */
gad.pmutation = 1.0 / gad.popsize;  /* probability of mutation  */
gad.elitist   = 1;                  /* preserve best string     */

gad.iseed     = 1;                  /* Seed for random numbers  */

gad.report_flag = 0;                /* =1: write reports        */


printf ("Call GA optimizer...\n");
ga_optimize (objfunc, &gad, xbest, &fbest);


printf ("\n------- Result --------\n");
printf ("Objective = %f\n", fbest);
for (jx = 0; jx < gad.nx; ++jx)
   printf ("x[%d] = %f\n", jx, xbest[jx]);

printf ("\nPress <RETURN> to continue...");
getchar();

return 0;

}  /* end of gatest main function */

/*--------------------------------------------------------------------*/

double objfunc (int n, double x[])
/*
 * Purpose...
 * -------
 * Given the n parameter values, compute the objective
 * (or raw fitness) function.
 *
 * Input...
 * -----
 * n    : number of parameters
 * x[]  : array of parameters x[i], 0 <= i < n
 *        Note that the optimizer searches only
 *        the range 0.0 <= x < 1.0.
 *
 * Output...
 * ------
 * objfun returns a double value
 *
 */
{
double value;

value = x[0] * x[0] * x[0] - 1.0;

#if GA_DEBUG >= 3
   printf ("objfunc: value = %f\n", value);
#endif

return (value);
}  /* end of objfun() */


/* gatest2.c
 * Test driver for ga_optimize() : Genetic Algorithm Optimizer.
 *
 * f6 test case from Schaffer, Caruana, Eshelman and Das
 *
 * P. Jacobs
 * Department of Mechanical Engineering
 * The University of Queensland
 * 
 * Updated...
 * 20-Jul-96 : timing for OS/2 machine
 * 20-Feb-98 : changed optimisation parameters (pmutation etc)
 *
 * ----------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "ga_opt.h"

double objfunc (int n, double x[]);

/*-------------------------------------------------------------*/

main ()
{
struct ga_data gad;
double fbest, xbest[10];
int    jx;
time_t start, now;

printf ("\n\nTest driver for ga_optimize(), f6 problem...\n");

/*
 * Set parameters for the GA
 */
gad.popsize = 50;                   /* size of populations      */
gad.nx      = 2;                    /* number of parameters     */
gad.xbits   = 22;                   /* bits in a parameter      */
gad.maxgen  = 8000 / gad.popsize;   /* number of generations    */
gad.spin    = 30;                   /* stop if no improvement   */
                                    /* in spin generations      */

gad.pcross    = 0.6;                /* probability of crossover */
gad.pmutation = 2.0 / gad.popsize;  /* probability of mutation  */
gad.elitist   = 1;                  /* preserve best string     */

gad.iseed     = 1;                  /* Seed for random numbers  */

gad.report_flag = 0;                /* =1: write reports        */


printf ("Call GA optimizer...\n");
start = time(NULL);
ga_optimize (objfunc, &gad, xbest, &fbest);
now = time(NULL);

printf ("\n------- Result --------\n");
printf ("Objective = %f\n", fbest);
for (jx = 0; jx < gad.nx; ++jx)
   printf ("x[%d] = %f\n", jx, xbest[jx]);
printf ("Wall-clock time = %d seconds\n", (int)(now-start) );

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
/* 
 * This objective function is labelled F6 in the paper
 * J. D. Schaffer, R. A. Caruana, L. J. Eshelman and R. Das
 * A study of control parameters affecting online performance
 * of genetic algorithms for function optimization.
 * Proceedings of the Third International Conference on 
 * Genetic Algorithms, Arlington, VA, 1989.
 */

double value;
double xx, yy, temp, temp2;

xx = (x[0] - 0.5) * 200.0;
yy = (x[1] - 0.5) * 200.0;

temp = xx * xx + yy * yy;
temp2 = sqrt(temp);
temp2 = sin(temp2);

value = 0.5 + (temp2 * temp2 - 0.5) / (1.0 + 0.001 * temp * temp);
value *= -1.0;  /* The original paper searched for a minimum. */

#if GA_DEBUG >= 3
   printf ("objfunc: x[0] = %f, x[1] = %f value = %f\n", 
           x[0], x[1], value);
#endif

return (value);
}  /* end of objfun() */


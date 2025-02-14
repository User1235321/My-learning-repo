head	1.1;
access;
symbols;
locks; strict;
comment	@ * @;


1.1
date	98.08.08.11.36.05;	author peterj;	state Exp;
branches;
next	;


desc
@CMATH 1989 code
@


1.1
log
@Initial revision
@
text
@/*  QUANC8D.C
    sample driver for quanc8
*/

#include <math.h>
#include <stdio.h>
#include "cmath.h"

main ()
{
double f();
double a, b, epsabs, epsrel, result, errest, posn;
int    nfe, flag;

a = 0.0;
b = 2.0;
epsrel = 1.0e-10;
epsabs = 0.0;

printf ("\n\n  --- CMATH --- Design Software 1989\n");
printf ("\nTrial for function integrator quanc8() ...\n");
quanc8 (f, a, b, epsabs, epsrel, &result, &errest,
        &nfe, &posn, &flag);
printf ("%s\n\n", cmathmsg(QUANC8_C, flag));

printf ("nfe = %d \n", nfe);
printf ("integral = %20.12f\n", result);
printf ("error    = %e \n", errest);
if (flag < 0)
   {
   printf ("trouble spot at x = %e \n", posn);
   printf ("%d unconverged subintervals\n", abs(flag));
   }
printf ("correct answer is 1.6054129768 \n");

return (0);
}  /* end of main */


double f (x)   /* test function */
double x;
{
double temp;
if (x == 0.0) temp = 1.0;
if (x != 0.0) temp = sin(x) / x;
return (temp);
}
@

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
@/* zeroin.c
   Function to find the zero of a supplied function.  */

/************************************************/
/*                                              */
/*  CMATH.  Copyright (c) 1989 Design Software  */
/*                                              */
/************************************************/

#include <math.h>
#include "cmath.h"

/*-----------------------------------------------------------------*/

#if (PROTOTYPE)

double zeroin (double left, double right,
               double (*f)(double x),
               double tol,
               int *flag)

#else

double zeroin (left, right, f, tol, flag)

double (*f)();
double left,
       right,
       tol;
int    *flag;

#endif

/* Purpose ...
   -------
   A zero of the function f(x) is computed, (possibly) in the
   interval left, right.  If the user specifies an interval that
   does not contain an odd number of zeros, zeroin() will attempt
   to bracket such an interval.

   Input ...
   -----
   left     : left end-point of the initial interval
   right    : right end-point of the initial interval
   (*f)()   : pointer to function which evaluates f(x) for any x
	      in the interval left, right
   tol      : desired length of interval of uncertainty of
	      the final result.  tol >= 0.0

   Output ...
   ------
   zeroin   : abscissa approximating a zero of f possibly in the
              interval left, right
   flag     : =  0, normal return
	      =  1, could not bracket a zero
	      =  2, tol is <= 0.0, or left == right

   This C code written by ...  Peter & Nigel,
   ----------------------      Design Software,
                               42 Gubberley St,
                               Kenmore, 4069,
                               Australia.

   Version ... 2.0, 11-Feb-89
   -------     1.1,  2-Dec-87
               2.1, 30-Apr-89 bracketing added

   Notes ...
   -----
   (1) Uses the math function fabs(x).

   (2) This program has been adapted from a FORTRAN program published
       in the book :
       Forsythe G.E., Malcolm, M.A. and Moler, C.B.
       Computer Methods for Mathematical Computations
       Prentice Hall

*/
/*------------------------------------------------------------------*/

{                 /* ---- start of zeroin() ---- */

/* local variables ... */
double zero, half, one, two, three;
double a, b, c, d, e;
double fa, fb, fc, tol1;
double xm, p, q, r, s;
int    zflag, skip, exit, bracket;
int    nstep, i, nseg;
double factor, x1, x2, dx, f1, f2;

/* constants */
#define  TRUE   1
#define  FALSE  0
zero = 0.0;
half = 0.5;
one = 1.0;
two = 2.0;
three = 3.0;

/* Initialization */
zflag = 0;
exit = FALSE;
a = left;
b = right;
fa = (*f)(a);
fb = (*f)(b);

/* Check constraints */
if ( tol <= zero || left == right)
  {
  exit = TRUE;
  zflag = 2;
  }

if ( fa * (fb / fabs(fb)) > zero )
   {
   /* try to bracket a zero ... */
   bracket = FALSE;

   /* first check the possibility of an even number of zeros
      within the user supplied range */
   nseg = 10;
   dx   = (b - a) / nseg;
   x1   = a; f1 = fa;
   for (i = 0; i < nseg; ++i)
      {
      x2 = x1 + dx;
      f2 = (*f) (x2);
      if (f1 * (f2 / fabs(f2)) < zero)
         {
         /* this segment brackets a zero */
         bracket = TRUE;
         a = x1; fa = f1;
         b = x2; fb = f2;
         break;
         }
      x1 = x2;
      f1 = f2;
      }

   if (!bracket)
      {
      /* now try extending the user supplied range ... */
      factor = 1.6;   /* increase the range by this factor */
      nstep  = 20;    /* maximum number of steps */
      x1 = a; f1 = fa;
      x2 = b; f2 = fb;
      for (i = 0; i < nstep; ++i)
         {
         /* extend the range in the downhill direction */
         if (fabs(f1) < fabs(f2))
            {
            x1 -= (x2 - x1) * factor;
            f1 = (*f) (x1);
            }
         else
            {
            x2 += (x2 - x1) * factor;
            f2 = (*f) (x2);
            }
         if (f1 * (f2 / fabs(f2)) <= zero)
            {
            /* we have bracketed a zero (or odd number of) */
            bracket = TRUE;
            a = x1; fa = f1;
            b = x2; fb = f2;
            break;
            }
         }
      }
   if (!bracket)
      {
      /* we have been unsuccessful in trying to bracket an
         odd number of zeros */
      exit = TRUE;
      zflag = 1;
      }
   }


/* Begin step */
skip = FALSE;
while ( !exit )
  {

  if ( !skip )
    {
    c = a;        /* ensure that the zero is between b and c */
    fc = fa;
    d = b - a;
    e = d;
    }

  if ( fabs(fc) < fabs(fb) )
    {
    a = b;             /* swap b and c to give fc >= fb */
    b = c;             /* b is then the best estimate for the zero */
    c = a;
    fa = fb;
    fb = fc;
    fc = fa;
    }

  /* Convergence test */
  tol1 = two * EPSILON * fabs(b) + half * tol;
  xm = half * (c - b);
  /* bail out if the solution is found to the desired accuracy */
  if ( (fabs(xm) < tol1) || (fb == zero) ) exit = TRUE;

  if (!exit)
    {  /* proceed with step */

    /* Is bisection necessary ? */
    if ( (fabs(e) < tol1) || (fabs(fa) <= fabs(fb)) )
      {
      d = xm;          /* bisection */
      e = d;
      }
    else
      {
      if ( a == c )  /* use quadratic interp. if are a and c distinct */
        {
        s = fb / fa;          /* linear interpolation */
        p = two * xm * s;
        q = one - s;
        }
      else
        {
        q = fa / fc;          /* quadratic interpolation */
        r = fb / fc;
        s = fb / fa;
        p = s * (two * xm * q * (q - r) - (b - a) * (r - one));
        q = (q - one) * (r - one) * (s - one);
        }

      if ( p > zero ) q = -q;     /* adjust signs */
      p = fabs(p);

      /* Is the interpolation acceptable? */

      if ( ((two * p) > (three * xm * q - fabs(tol1 * q))) ||
           (p >= fabs(half * e * q)) )
        {
        d = xm;           /* use bisection */
        e = d;
        }
      else
        {
        e = d;            /* use previously selected  */
        d = p / q;        /* interpolation            */
        }
      }  /* if .. bisection necessary ? */

    /* Complete step */
    a = b;                   /* save old point b as a */
    fa = fb;
    if (fabs(d) > tol1)
      b = b + d;         /* move b to a new point closer to the zero */
    else
      {                  /* move b by a relatively small amount */
      if (xm > zero)
        b = b + fabs(tol1);
      else
        b = b - fabs(tol1);
      }
    fb = (*f)(b);            /* function value at the new point */

    if ( (fb * (fc / fabs(fc))) <= zero )
      skip = TRUE;       /* zero is already between b and c */
    else skip = FALSE; /* swap a and c to get zero between b and c */

    }  /* if not exit , end of step */
  }  /* while */


/* all done */
*flag = zflag;

/* return the abscissa with the minimum absolute value */
return(b);

#undef TRUE
#undef FALSE

}  /* ---- end of zeroin() ---- */

/*-----------------------------------------------------------------*/

@

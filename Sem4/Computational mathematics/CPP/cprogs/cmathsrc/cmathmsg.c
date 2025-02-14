/* cmathmsg.c
   CMATH error message translator.  */


/************************************************/
/*                                              */
/*  CMATH.  Copyright (c) 1989 Design Software  */
/*                                              */
/************************************************/

#include "cmath.h"
#if (STDLIBH)
#include <stdlib.h>
#endif
#include <stdio.h>
#include <math.h>
#if (STRINGH)
#include <string.h>
#endif

#ifndef NULL
#define  NULL  0
#endif

/*-----------------------------------------------------------------*/

#if (PROTOTYPE)

char *cmathmsg (int routine, int flag)

#else

char *cmathmsg (routine, flag)

#endif

/* Purpose ...
   -------
   Translate the error flag returned by a particular CMATH routine
   into an English error message.

   Input ...
   -----
   routine  : An integer value specifying the CMATH routine.
              You should use the macro definitions supplied
              in the cmath.h header file.  The name of the
              constant for each CMATH routine is written in
              UPPER CASE and is suffixed by "_C".  For example
              the constant for the routine decomp() is DECOMP_C.
   flag     : The integer value of the error flag (or status
              indicator) as returned by the CMATH routine.

   Output ...
   ------
   cmathmsg() : returns a pointer to a string containing the text
                of the error message.

   Version ... 1.0,  15-August-1989
   -------

   This C code written by ...  Peter & Nigel,
   ----------------------      Design Software,
                               42 Gubberley St,
                               Kenmore, 4069,
                               Australia.

*/

{  /* begin cmathmsg() */
static char s[64];

switch (routine)
   {
   case BANDFAC_C :
      switch (flag)
         {
         case 0  :  strcpy (s, "bandfac () : normal return.");
                    break;
         case 1  :  strcpy (s, "bandfac () : invalid input parameter(s).");
                    break;
         case 2  :  strcpy (s, "bandfac () : pivot of matrix = 0.0");
                    break;
         case 3  :  strcpy (s, "bandfac () : last diagonal term = 0.0");
                    break;
         default :  strcpy (s, "bandfac () : no such error.");
         }
      break;

   case BSPLINIT_C :
      switch (flag)
         {
         case 0  :  strcpy (s, "bsplinit () : normal return.");
                    break;
         case 1  :  strcpy (s, "bsplinit () : unable to reserve memory.");
                    break;
         default :  strcpy (s, "bsplinit () : no such error.");
         }
      break;

   case BISECT_C :
      switch (flag)
         {
         default :  strcpy (s, "bisect () :  no such error.");
                    break;
         }
      break;

   case INTERV_C :
      switch (flag)
         {
         case 0  :  strcpy (s, "interv () : normal return.");
                    break;
         case -1  :  strcpy (s, "interv () : parameter value < knot[0].");
                    break;
         case 1  :  strcpy (s, "interv () : parameter value >= knot[maxi].");
                    break;
         default  :  strcpy (s, "interv () : no such error.");
         }
      break;


   case BSPLVB_C :
      switch (flag)
         {
         case 0  :  strcpy (s, "bsplvb () : normal return.");
                    break;
         case 1  :  strcpy (s, "bsplvb () : invalid value for index.");
                    break;
         default :  strcpy (s, "bsplvb () : no such error.");
         }
      break;

   case BSPLEND_C :
      switch (flag)
         {
         default :  strcpy (s, "bsplend () :  no such error.");
                    break;
         }
      break;

   case BSPLINE_C :
      switch (flag)
         {
         case 0  :  strcpy (s, "bspline () : normal return.");
                    break;
         case 1  :  strcpy (s, "bspline () : unable to assign memory.");
                    break;
         case 2  :  strcpy (s, "bspline () : parameter out of knot range.");
                    break;
         case 3  :  strcpy (s, "bspline () : error in banded matrix solution.");
                    break;
         default :  strcpy (s, "bspline () : no such error.");
         }
      break;

   case BSEVAL_C :
      switch (flag)
         {
         case 0  :  strcpy (s, "bseval () : normal return.");
                    break;
	 case 1  :  strcpy (s, "bseval () : parameter value not valid.");
		    break;
	 case 2  :  strcpy (s, "bseval () : invalid value for index.");
                    break;
         default :  strcpy (s, "bseval () : no such error.");
         }
      break;

    case BSDCPNTS_C :
       switch (flag)
          {
          case  0  :  strcpy (s, "bsdcpnt () : normal return.");
                      break;
          case  1  :  strcpy (s, "bsdcpnt () : NULL pointer to matrix.");
                      break;
          default  :  strcpy (s, "bsdcpnt () : no such error.");
          }
       break;

   case DECOMP_C :
      switch (flag)
         {
         case 0  : strcpy (s, "decomp() : normal return");
                   break;
         case 1  : strcpy (s, "decomp() : could not allocate workspace");
                   break;
         case 2  : strcpy (s, "decomp() : illegal user input");
                   break;
         case 3  : strcpy (s, "decomp() : matrix is singular");
                   break;
         default : strcpy (s, "decomp() : no such error");
         };
      break;

   case CDECOMP_C :
      switch (flag)
         {
         case 0  : strcpy (s, "cdecomp() : normal return");
                   break;
         case 1  : strcpy (s, "cdecomp() : could not allocate workspace");
                   break;
         case 2  : strcpy (s, "cdecomp() : illegal user input");
                   break;
         case 3  : strcpy (s, "cdecomp() : matrix is singular");
                   break;
         default : strcpy (s, "cdecomp() : no such error");
         };
      break;

   case SVD_C :
      if (flag > 0)
         {
         strcpy (s, "svd() : too many iterations required");
         }
      else
         {
         switch (flag)
            {
            case 0  : strcpy (s, "svd() : normal return");
                      break;
            case -1 : strcpy (s, "svd() : could not allocate workspace");
                      break;
            case -2 : strcpy (s, "svd() : invalid user input");
                      break;
            default : strcpy (s, "svd() : no such error");
            };
         }
      break;

   case SVDSOLVE_C :
      switch (flag)
         {
         case 0  : strcpy (s, "svdsolve() : normal return");
                   break;
         case -1 : strcpy (s, "svdsolve() : could not allocate workspace");
                   break;
         case -2 : strcpy (s, "svdsolve() : invalid user input");
                   break;
         default : strcpy (s, "svdsolve() : no such error");
         };
      break;

   case TRIDIAG_C :
      switch (flag)
         {
         case 0  : strcpy (s, "tridiag() : normal return");
                   break;
         case 1  : strcpy (s, "tridiag() : elimination failure");
                   break;
         case 2  : strcpy (s, "tridiag() : illegal user input");
                   break;
         default : strcpy (s, "tridiag() : no such error");
         };
      break;

   case TRISOLVE_C :
      switch (flag)
         {
         case 0  : strcpy (s, "trisolve() : normal return");
                   break;
         case 1  : strcpy (s, "trisolve() : back-substitution failure");
                   break;
         default : strcpy (s, "trisolve() : no such error");
         };
      break;

   case INVERT_C :
      switch (flag)
         {
         case 0  : strcpy (s, "invert() : normal return");
                   break;
         case 1  : strcpy (s, "invert() : matrix is singular");
                   break;
         case 2  : strcpy (s, "invert() : could not allocate workspace");
                   break;
         case 3  : strcpy (s, "invert() : invalid user input");
                   break;
         default : strcpy (s, "invert() : no such error");
         };
      break;

   case QR_C :
      if (flag > 0)
         {
         strcpy (s, "qr() : too many iterations required");
         }
      else
         {
         switch (flag)
            {
            case 0  : strcpy (s, "qr() : normal return");
                      break;
            case -1 : strcpy (s, "qr() : could not allocate workspace");
                      break;
            case -2 : strcpy (s, "qr() : invalid user input");
                      break;
            default : strcpy (s, "qr() : no such error");
            };
         }
      break;

   case QRV_C :
      if (flag > 0)
         {
         strcpy (s, "qrv() : too many iterations required");
         }
      else
         {
         switch (flag)
            {
            case 0  : strcpy (s, "qrv() : normal return");
                      break;
            case -1 : strcpy (s, "qrv() : could not allocate workspace");
                      break;
            case -2 : strcpy (s, "qrv() : invalid user input");
                      break;
            default : strcpy (s, "qrv() : no such error");
            };
         }
      break;

   case QRVECTOR_C :
      switch (flag)
         {
         case 0  : strcpy (s, "qrvector() : normal return");
                   break;
         case 1  : strcpy (s, "qrvector() : invalid user input");
                   break;
         case 2  : strcpy (s, "qrvector() : could not find conjugate");
                   break;
         default : strcpy (s, "qrvector() : no such error");
         };
      break;

   case ZEROIN_C :
      switch (flag)
         {
         case 0  : strcpy (s, "zeroin() : normal return");
                   break;
         case 1  : strcpy (s, "zeroin() : could not bracket a zero");
                   break;
         case 2  : strcpy (s, "zeroin() : invalid user input");
                   break;
         default : strcpy (s, "zeroin() : no such error");
         };
      break;

   case ZEROV_C :
      switch (flag)
         {
         case 0  : strcpy (s, "zerov() : normal return");
                   break;
         case 1  : strcpy (s, "zerov() : invalid user input");
                   break;
         case 2  : strcpy (s, "zerov() : could not allocate workspace");
                   break;
         case 3  : strcpy (s, "zerov() : minimizer did not converge");
                   break;
         case 4  : strcpy (s, "zerov() : local minimum is not a zero");
                   break;
         case 5  : strcpy (s, "zerov() : Newton-Raphson diverging");
                   break;
         case 6  : strcpy (s, "zerov() : singular Jacobian");
                   break;
         default : strcpy (s, "zerov() : no such error");
         };
      break;

   case POLYROOT_C :
      switch (flag)
         {
         case 0  : strcpy (s, "polyroot() : normal return");
                   break;
         case 1  : strcpy (s, "polyroot() : a root did not converge");
                   break;
         case 2  : strcpy (s, "polyroot() : could not allocate workspace");
                   break;
         case 3  : strcpy (s, "polyroot() : invalid user input");
                   break;
         case 4  : strcpy (s, "polyroot() : invalid order for polynomial");
                   break;
         default : strcpy (s, "polyroot() : no such error");
         };
      break;

   case SPLINE_C :
      switch (flag)
         {
         case 0  : strcpy (s, "spline() : normal return");
                   break;
         case 1  : strcpy (s, "spline() : n < 2, cannot interpolate");
                   break;
         case 2  : strcpy (s, "spline() : x[i] not in ascending order");
                   break;
         default : strcpy (s, "spline() : no such error");
         };
      break;

   case FITSPL_C :
      switch (flag)
         {
         case 0  : strcpy (s, "fitspl() : normal return");
                   break;
         case 1  : strcpy (s, "fitspl() : illegal values for ns, nd");
                   break;
         case 2  : strcpy (s, "fitspl() : xs[i] not in ascending order");
                   break;
         case 3  : strcpy (s, "fitspl() : all weights are not positive");
                   break;
         case 4  : strcpy (s, "fitspl() : could not allocate workspace");
                   break;
         case 5  : strcpy (s, "fitspl() : minimizer did not converge");
                   break;
         default : strcpy (s, "fitspl() : no such error");
         };
      break;

   case FITPOLY_C :
      switch (flag)
         {
         case 0  : strcpy (s, "fitpoly() : normal return");
                   break;
         case 1  : strcpy (s, "fitpoly() : illegal values for ns, m");
                   break;
         case 2  : strcpy (s, "fitpoly() : x[i] not in range [a, b]");
                   break;
         case 3  : strcpy (s, "fitpoly() : all weights are not positive");
                   break;
         case 4  : strcpy (s, "fitpoly() : could not allocate workspace");
                   break;
         case 5  : strcpy (s, "fitpoly() : solution failed");
                   break;
         default : strcpy (s, "fitpoly() : no such error");
         };
      break;

   case LSP_C :
      if (flag > 0)
         {
         strcpy (s, "lsp() : too many iterations required");
         }
      else
         {
         switch (flag)
            {
            case 0  : strcpy (s, "lsp() : normal return");
                      break;
            case -1 : strcpy (s, "lsp() : could not allocate workspace");
                      break;
            case -2 : strcpy (s, "lsp() : invalid user input");
                      break;
            default : strcpy (s, "lsp() : no such error");
            };
         }
      break;

   case CHEBY_C :
      switch (flag)
         {
         case 0  : strcpy (s, "cheby() : normal return");
                   break;
         case 1  : strcpy (s, "cheby() : invalid user input");
                   break;
         default : strcpy (s, "cheby() : no such error");
         };
      break;

   case CHEBYC_C :
      switch (flag)
         {
         case 0  : strcpy (s, "chebyc() : normal return");
                   break;
         case 1  : strcpy (s, "chebyc() : invalid user input");
                   break;
         case 2  : strcpy (s, "chebyc() : cannot allocate workspace");
                   break;
         default : strcpy (s, "chebyc() : no such error");
         };
      break;

   case CHEBYD_C :
      switch (flag)
         {
         case 0  : strcpy (s, "chebyd() : normal return");
                   break;
         case 1  : strcpy (s, "chebyd() : invalid user input");
                   break;
         default : strcpy (s, "chebyd() : no such error");
         };
      break;

   case CHEBYI_C :
      switch (flag)
         {
         case 0  : strcpy (s, "chebyi() : normal return");
                   break;
         case 1  : strcpy (s, "chebyi() : invalid user input");
                   break;
         default : strcpy (s, "chebyi() : no such error");
         };
      break;

   case QK21INIT_C :
      strcpy (s, "qk21init() : no such error");
      break;

   case QK21_C :
      strcpy (s, "qk21() : no such error");
      break;

   case QAGS_C :
      switch (flag)
         {
         case 0  : strcpy (s, "qags() : normal return");
                   break;
         case 1  : strcpy (s, "qags() : maximum subdivisions reached");
                   break;
         case 2  : strcpy (s, "qags() : round-off error detected");
                   break;
         case 3  : strcpy (s, "qags() : badly behaved integrand");
                   break;
         case 4  : strcpy (s, "qags() : integral has not converged");
                   break;
         case 5  : strcpy (s, "qags() : integral divergent");
                   break;
         case 6  : strcpy (s, "qags() : invalid user input");
                   break;
         default : strcpy (s, "qags() : no such error");
         };
      break;

   case QINF_C :
      switch (flag)
         {
         case 0  : strcpy (s, "qinf() : normal return");
                   break;
         case 1  : strcpy (s, "qinf() : maximum subdivisions reached");
                   break;
         case 2  : strcpy (s, "qinf() : round-off error detected");
                   break;
         case 3  : strcpy (s, "qinf() : badly behaved integrand");
                   break;
         case 4  : strcpy (s, "qinf() : integral has not converged");
                   break;
         case 5  : strcpy (s, "qinf() : integral divergent");
                   break;
         case 6  : strcpy (s, "qinf() : invalid user input");
                   break;
         default : strcpy (s, "qinf() : no such error");
         };
      break;

   case QUANC8_C :
      if (flag < 0)
         strcpy (s, "quanc8() : result unreliable");
      else
         {
         switch (flag)
            {
            case 0  : strcpy (s, "quanc8() : normal return");
                      break;
            case 1  : strcpy (s, "quanc8() : invalid user input");
                      break;
            default : strcpy (s, "quanc8() : no such error");
            };
         }
      break;

   case RKFINIT_C :
      switch (flag)
         {
         case 0  : strcpy (s, "rkfinit() : normal return");
                   break;
         case 1  : strcpy (s, "rkfinit() : could not allocate workspace");
                   break;
         case 2  : strcpy (s, "rkfinit() : illegal value for n");
                   break;
         default : strcpy (s, "rkfinit() : no such error");
         };
      break;

   case RKF45_C :
      switch (flag)
         {
         case -2 : strcpy (s, "rkf45() : normal return");
                   break;
         case  2 : strcpy (s, "rkf45() : normal return");
                   break;
         case  3 : strcpy (s, "rkf45() : relerr too small");
                   break;
         case  4 : strcpy (s, "rkf45() : too many steps");
                   break;
         case  5 : strcpy (s, "rkf45() : abserr needs to be nonzero");
                   break;
         case  6 : strcpy (s, "rkf45() : stepsize has become too small");
                   break;
         case  7 : strcpy (s, "rkf45() : rkf45 is inefficient");
                   break;
         case  8 : strcpy (s, "rkf45() : invalid user input");
                   break;
         default : strcpy (s, "rkf45() : no such error");
         };
      break;

   case RKFEND_C :
      strcpy (s, "rkfend() : no such error");
      break;

   case STINT0_C :
      switch (flag)
         {
         case 0  : strcpy (s, "stint0() : normal return");
                   break;
         case 1  : strcpy (s, "stint0() : illegal value for n");
                   break;
         case 2  : strcpy (s, "stint0() : could not allocate workspace");
                   break;
         default : strcpy (s, "stint0() : no such error");
         };
      break;

   case STINT1_C :
      if (flag > 0)
         {
         strcpy (s, "stint1() : normal return");
         }
      else
         {
         switch (flag)
            {
            case -1 : strcpy (s, "stint1() : 1 step, step size too small");
                      break;
            case -2 : strcpy (s, "stint1() : 2 steps, step size too small");
                      break;
            case -3 : strcpy (s, "stint1() : 3 steps, step size too small");
                      break;
            case -4 : strcpy (s, "stint1() : 4 steps, step size too small");
                      break;
            case -5 : strcpy (s, "stint1() : requested precision too small");
                      break;
            case -6 : strcpy (s, "stint1() : corrector did not converge");
                      break;
            default : strcpy (s, "stint1() : no such error");
            };
         }
      break;

   case STINT2_C :
      strcpy (s, "stint2() : errors unavailable, check source code");
      break;

   case STINT3_C :
      strcpy (s, "stint3() : no such error");
      break;

   case NELMIN_C :
      switch (flag)
         {
         case 0  : strcpy (s, "nelmin() : normal return");
                   break;
         case 1  : strcpy (s, "nelmin() : invalid user input");
                   break;
         case 2  : strcpy (s, "nelmin() : too many steps taken");
                   break;
         case 3  : strcpy (s, "nelmin() : could not allocate workspace");
                   break;
         default : strcpy (s, "nelmin() : no such error");
         };
      break;

   case CONJGG_C :
      switch (flag)
         {
         case 0  : strcpy (s, "conjgg() : normal return");
                   break;
         case 1  : strcpy (s, "conjgg() : too many 1-D searches needed");
                   break;
         case 2  : strcpy (s, "conjgg() : could not bracket a minimum");
                   break;
         case 3  : strcpy (s, "conjgg() : could not allocate workspace");
                   break;
         case 4  : strcpy (s, "conjgg() : invalid user input");
                   break;
         default : strcpy (s, "conjgg() : no such error");
         };
      break;

   case FFT_C :
      switch (flag)
         {
         case 0  : strcpy (s, "fft() : normal return");
                   break;
         case 1  : strcpy (s, "fft() : invalid user input");
                   break;
         case 2  : strcpy (s, "fft() : n is not an integer power of 2");
                   break;
         case 3  : strcpy (s, "fft() : direct not +1 or -1");
                   break;
         default : strcpy (s, "fft() : no such error");
         };
      break;

   case DFT_C :
      switch (flag)
         {
         case 0  : strcpy (s, "dft() : normal return");
                   break;
         case 1  : strcpy (s, "dft() : invalid user input");
                   break;
         case 2  : strcpy (s, "dft() : n2 is not an integer power of 2");
                   break;
         case 3  : strcpy (s, "dft() : direct not +1 or -1");
                   break;
         default : strcpy (s, "dft() : no such error");
         };
      break;

   case CHIRP_C :
      switch (flag)
         {
         case 0  : strcpy (s, "chirp() : normal return");
                   break;
         case 1  : strcpy (s, "chirp() : invalid user input");
                   break;
         case 2  : strcpy (s, "chirp() : n2 not a power of 2 or too small");
                   break;
         default : strcpy (s, "chirp() : no such error");
         };
      break;

   case CDIVSN_C :
      strcpy (s, "cdivsn() : no such error");
      break;

   case CMULTN_C :
      strcpy (s, "cmultn() : no such error");
      break;

   case CSQROOT_C :
      strcpy (s, "csqroot() : no such error");
      break;

   case CABSLT_C :
      strcpy (s, "cabslt() : no such error");
      break;

   case INDEXX_C :
      strcpy (s, "indexx() : no such error");
      break;

   case CMATHMSG_C :
      strcpy (s, "cmathmsg() : no such error");
      break;

   default : strcpy (s, "CMATH : no such routine");
   }
return (s);
}


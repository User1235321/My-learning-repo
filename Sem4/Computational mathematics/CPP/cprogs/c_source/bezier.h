/* bezier.h
 * Bezier curves of third order and arbitrary order -- header file.
 * ----------------------------------------------------------------
 */

#ifndef COMPILER_H
/* #  error We need to include compiler.h. */
#include "compiler.h"
#endif

#ifndef GEOM_H
/* #  error We need to include geom.h. */
#include "geom.h"
#endif

#ifdef BEZIER_H
/* #  error bezier.h has already been included. */
#else
#  define BEZIER_H  1
#endif

/*====================== Data Structures =========================*/

/*
 * Data for a single Bezier curve of third order.
 */
struct bezier_3_data
   {
   int n;                          /* order of curve                */
   struct point_3D B[4];           /* the control points            */
   };

/*
 * Data for a single n-degree Bezier curve.
 * For convenience, we will set the upper limit on the degree
 * to be a constant.  This way we can avoid having to allocate
 * memory for work arrays etc.
 */
#define  MAX_BEZ_DEGREE  20
struct bezier_n_data
   {
   int n;                                  /* degree of the curve */
   struct point_3D B[MAX_BEZ_DEGREE+1];    /* the control points  */
   };

/*
 * Data for a polyline constructed from a number of
 * third-order Bezier curves.
 */
struct bezier_3_poly_data
   {
   int n;                          /* number of Bezier segments     */
   struct bezier_3_data *b3_seg;   /* data for each of the segments */
   double *t_star;                 /* Normalized distance along     */
				   /* the polyline                  */
   int closed;                     /* =0, open curve                */
				   /* =1, closed curve              */
   };

/*===================== Function Declarations ======================*/

#if (PROTO)

int alloc_bezier_3_poly (struct bezier_3_poly_data *bp, int n);

int init_bezier_3 (struct bezier_3_data *b3,
		   struct point_3D *loc0,
		   struct point_3D *loc1,
		   struct point_3D *loc2,
		   struct point_3D *loc3);

int init_bezier_n (int n,
                   struct bezier_n_data *bn,
		   struct point_3D locarray[] );

int eval_bezier_3 (struct bezier_3_data *b3,
		   double t,
		   struct point_3D *loc);

int eval_bezier_n (struct bezier_n_data *bn,
		   double t,
		   struct point_3D *loc);

int deriv_bezier_3 (struct bezier_3_data *b3,
		    double t,
		    struct point_3D *dxyzdt);

int segment_bezier_3_poly (struct bezier_3_poly_data *bp,
			   struct point_3D *loc0,
			   struct point_3D *loc1,
			   struct point_3D *loc2,
			   struct point_3D *loc3,
			   int i);

int normalize_bezier_3_poly (struct bezier_3_poly_data *bp);

int eval_bezier_3_poly (struct bezier_3_poly_data *bp,
			double t_star,
			struct point_3D *loc);

int bezier_3_spline( struct bezier_3_poly_data *bp,
                     int m,
                     struct point_3D p[] );

int coons_bezier_3 (struct bezier_3_poly_data *c1,
		    struct bezier_3_poly_data *c2,
		    struct bezier_3_poly_data *c3,
		    struct bezier_3_poly_data *c4,
		    double r, double s,
		    struct point_3D *d);

#else

int alloc_bezier_3_poly (); /* allocate data for a bezier polyline */

int init_bezier_3 ();    /* put control points into a bezier curve */

int eval_bezier_3 ();    /* evaluate a bezier curve location       */

int deriv_bezier_3 ();   /* evaluate the derivative wrt t          */

int segment_bezier_3_poly ();
	      /* add control points for a specified bezier segment */

int normalize_bezier_3_poly ();
	  /* compute the parameter values at the curve breakpoints */

int eval_bezier_3_poly ();
		     /* evaluate a location on the bezier polyline */

int coons_bezier_3 ();                     /* linear Coons surface */

#endif

/*==================================================================*/


/* bezierd.c
 * Sample driver for the Bezier curves of third order
 * -----------------------------------------------------------------
 */


#include <math.h>
#include <stdio.h>

#include "compiler.h"

#if (STDLIBH)
#  include <stdlib.h>
#endif

#if (CONIOH)
#  include <conio.h>
#endif

#include "geom.h"
#include "bezier.h"

/*--------------------------------------------------------------*/

main ()
{
struct bezier_3_poly_data A, AA, c1, c2, c3, c4;
struct bezier_3_data B;
int n;
struct point_3D loc0, loc1, loc2, loc3, pos;
double s, r, t;
struct bezier_n_data Bn;
struct point_3D loca[10];

printf ("Test drive Bezier curves...\n");

#if (TURBO_C || TOPSPEED_C || IBMCSET)
   printf ("Press a key to continue..."); fflush(stdout);
   fflush(stdin);
   getchar();
#endif

printf ("A single third-degree Bezier curve...\n");
loc0.x = 0.0; loc0.y = 0.0; loc0.z = 0.0;
loc1.x = 0.2; loc1.y = 1.0; loc1.z = 0.0;
loc2.x = 0.8; loc2.y = 1.0; loc2.z = 0.0;
loc3.x = 1.0; loc3.y = 0.0; loc3.z = 0.0;
init_bezier_3 (&B, &loc0, &loc1, &loc2, &loc3);

for (t = 0; t <= 1.001; t += 0.1)
   {
   eval_bezier_3 (&B, t, &pos);
   printf ("%f %f %f\n", t, pos.x, pos.y);
   }

#if (TURBO_C || TOPSPEED_C || IBMCSET)
   printf ("Press a key to continue..."); fflush(stdout);
   fflush(stdin);
   getchar();
#endif

printf ("A single n-degree Bezier curve...\n");
loca[0].x = 0.0; loca[0].y = 0.0; loca[0].z = 0.0;
loca[1].x = 0.2; loca[1].y = 1.0; loca[1].z = 0.0;
loca[2].x = 0.8; loca[2].y = 1.0; loca[2].z = 0.0;
loca[3].x = 1.0; loca[3].y = 0.0; loca[3].z = 0.0;
loca[4].x = 1.2; loca[4].y = 1.0; loca[4].z = 0.0;
loca[5].x = 1.8; loca[5].y = 1.0; loca[5].z = 0.0;
loca[6].x = 2.0; loca[6].y = 0.0; loca[6].z = 0.0;
init_bezier_n (6, &Bn, loca);

for (t = 0; t <= 1.001; t += 0.1)
   {
   eval_bezier_n (&Bn, t, &pos);
   printf ("%f %f %f %f\n", t, pos.x, pos.y, pos.z);
   }

#if (TURBO_C || TOPSPEED_C || IBMCSET)
   printf ("Press a key to continue..."); fflush(stdout);
   fflush(stdin);
   getchar();
#endif

printf ("\nAllocate memory for a polyline of two 3-degree curves.\n");
n = 2;
alloc_bezier_3_poly (&A, n);

#if (TURBO_C || TOPSPEED_C || IBMCSET)
   printf ("Press a key to continue..."); fflush(stdout);
   fflush(stdin);
   getchar();
#endif

printf ("\n--------------------------------------\n");
printf ("A Bezier polyline...\n");
loc0.x = 0.0; loc0.y = 0.0; loc0.z = 0.0;
loc1.x = 0.2; loc1.y = 1.0; loc1.z = 0.0;
loc2.x = 0.8; loc2.y = 1.0; loc2.z = 0.0;
loc3.x = 1.0; loc3.y = 0.0; loc3.z = 0.0;
segment_bezier_3_poly (&A, &loc0, &loc1, &loc2, &loc3, 0);

loc0.x = 1.0; loc0.y =  0.0; loc0.z = 0.0;
loc1.x = 1.2; loc1.y = -1.0; loc1.z = 0.0;
loc2.x = 1.8; loc2.y = -1.0; loc2.z = 0.0;
loc3.x = 2.0; loc3.y =  0.0; loc3.z = 0.0;
segment_bezier_3_poly (&A, &loc0, &loc1, &loc2, &loc3, 1);

normalize_bezier_3_poly (&A);

for (t = 0; t <= 1.0; t += 0.1)
   {
   eval_bezier_3_poly (&A, t, &pos);
   printf ("%f %f %f\n", t, pos.x, pos.y);
   }

#if (TURBO_C || TOPSPEED_C || IBMCSET)
   printf ("Press a key to continue..."); fflush(stdout);
   fflush(stdin);
   getchar();
#endif

printf ("\n--------------------------------------\n");
printf ("A Bezier spline...\n");

printf ("\nAllocate memory for a polyline of two 3-degree curves.\n");
n = 4;
alloc_bezier_3_poly (&AA, n);

#if (TURBO_C || TOPSPEED_C || IBMCSET)
   printf ("Press a key to continue..."); fflush(stdout);
   fflush(stdin);
   getchar();
#endif

loca[0].x = 0.0; loca[0].y = sin(0.0); loca[0].z = 0.0;
loca[1].x = 0.5; loca[1].y = sin(0.5); loca[1].z = 0.0;
loca[2].x = 1.0; loca[2].y = sin(1.0); loca[2].z = 0.0;
loca[3].x = 1.5; loca[3].y = sin(1.5); loca[3].z = 0.0;
loca[4].x = 2.0; loca[4].y = sin(2.0); loca[4].z = 0.0;
bezier_3_spline (&AA, n, loca);

for (t = 0; t <= 1.001; t += 0.05) {
   eval_bezier_3_poly (&AA, t, &pos);
   printf ("%f %f %f %f %f\n", t, pos.x, pos.y, pos.z, sin(pos.x));
} /* end for */

#if (TURBO_C || TOPSPEED_C || IBMCSET)
   printf ("Press a key to continue..."); fflush(stdout);
   fflush(stdin);
   getchar();
#endif

printf ("\n--------------------------------------\n");
printf ("Bezier Coons patch...\n");

n = 2;
alloc_bezier_3_poly (&c1, n);
alloc_bezier_3_poly (&c2, n);
alloc_bezier_3_poly (&c3, n);
alloc_bezier_3_poly (&c4, n);

loc0.x = 0.0; loc0.y = 0.0; loc0.z = 0.0;
loc1.x = 0.2; loc1.y = 0.2; loc1.z = 0.0;
loc2.x = 0.8; loc2.y = 0.2; loc2.z = 0.0;
loc3.x = 1.0; loc3.y = 0.0; loc3.z = 0.0;
segment_bezier_3_poly (&c1, &loc0, &loc1, &loc2, &loc3, 0);
loc0.x = 1.0; loc0.y =  0.0; loc0.z = 0.0;
loc1.x = 1.2; loc1.y = -0.2; loc1.z = 0.0;
loc2.x = 1.8; loc2.y = -0.2; loc2.z = 0.0;
loc3.x = 2.0; loc3.y =  0.0; loc3.z = 0.0;
segment_bezier_3_poly (&c1, &loc0, &loc1, &loc2, &loc3, 1);
normalize_bezier_3_poly (&c1);

loc0.x = 0.0; loc0.y = 2.0; loc0.z = 0.0;
loc1.x = 0.2; loc1.y = 2.2; loc1.z = 0.0;
loc2.x = 0.8; loc2.y = 2.2; loc2.z = 0.0;
loc3.x = 1.0; loc3.y = 2.0; loc3.z = 0.0;
segment_bezier_3_poly (&c2, &loc0, &loc1, &loc2, &loc3, 0);
loc0.x = 1.0; loc0.y = 2.0; loc0.z = 0.0;
loc1.x = 1.2; loc1.y = 1.8; loc1.z = 0.0;
loc2.x = 1.8; loc2.y = 1.8; loc2.z = 0.0;
loc3.x = 2.0; loc3.y = 2.0; loc3.z = 0.0;
segment_bezier_3_poly (&c2, &loc0, &loc1, &loc2, &loc3, 1);
normalize_bezier_3_poly (&c2);

loc0.x = 0.0; loc0.y = 0.0; loc0.z = 0.0;
loc1.x = 0.2; loc1.y = 0.2; loc1.z = 0.0;
loc2.x = 0.2; loc2.y = 0.8; loc2.z = 0.0;
loc3.x = 0.0; loc3.y = 1.0; loc3.z = 0.0;
segment_bezier_3_poly (&c3, &loc0, &loc1, &loc2, &loc3, 0);
loc0.x =  0.0; loc0.y = 1.0; loc0.z = 0.0;
loc1.x = -0.2; loc1.y = 1.2; loc1.z = 0.0;
loc2.x = -0.2; loc2.y = 1.8; loc2.z = 0.0;
loc3.x =  0.0; loc3.y = 2.0; loc3.z = 0.0;
segment_bezier_3_poly (&c3, &loc0, &loc1, &loc2, &loc3, 1);
normalize_bezier_3_poly (&c3);

loc0.x = 2.0; loc0.y = 0.0; loc0.z = 0.0;
loc1.x = 2.2; loc1.y = 0.2; loc1.z = 0.0;
loc2.x = 2.2; loc2.y = 0.8; loc2.z = 0.0;
loc3.x = 2.0; loc3.y = 1.0; loc3.z = 0.0;
segment_bezier_3_poly (&c4, &loc0, &loc1, &loc2, &loc3, 0);
loc0.x = 2.0; loc0.y = 1.0; loc0.z = 0.0;
loc1.x = 1.8; loc1.y = 1.2; loc1.z = 0.0;
loc2.x = 1.8; loc2.y = 1.8; loc2.z = 0.0;
loc3.x = 2.0; loc3.y = 2.0; loc3.z = 0.0;
segment_bezier_3_poly (&c4, &loc0, &loc1, &loc2, &loc3, 1);
normalize_bezier_3_poly (&c4);

for (s = 0.0; s <= 1.0; s += 0.333333)
   {
   for (r = 0.0; r <= 1.0; r += 0.333333)
      {
      coons_bezier_3 (&c1, &c2, &c3, &c4, r, s, &pos);
      printf ("s=%12.6f, r=%12.6f, x=%12.6f, y=%12.6f\n",
	      s, r, pos.x, pos.y);
      }
   printf ("\n");
   }

printf ("\n========= End of Bezier test drive. ===========\n");

#if (TURBO_C || TOPSPEED_C || IBMCSET)
   printf ("Press a key to continue..."); fflush(stdout);
   fflush(stdin);
   getchar();
#endif

return (0);
}


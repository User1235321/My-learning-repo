/* hsv2rgbd.c
 * Try out HSV to RGB conversion.
 */
 
#include "hsv2rgb.h"
#include <stdio.h>

int main () {
   struct rgb_colour rgb1;
   struct hsv_colour hsv1;
   
   hsv1.h = 0.2;
   hsv1.s = 0.8;
   hsv1.v = 0.9;
   hsv2rgb( &hsv1, &rgb1 );
   printf("Hue %g Saturation %g Value %g\n", hsv1.h, hsv1.s, hsv1.v);
   printf("Red %g Green %g Blue %g\n", rgb1.r, rgb1.g, rgb1.b);

   return 0;
} /* end main */

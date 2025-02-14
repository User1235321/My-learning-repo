      subroutine cdivsn (xr, xi, yr, yi, zr, zi)
c
c   Purpose ...
c   -------
c   Complex number division routine without complex numbers.
c
c   Input ...
c   -----
c   xr + i xi  : numerator
c   yr + i yi  : denominator
c
c   Output ...
c   ------
c   zr + i zi  : (xr + i xi) / (yr + i yi)
c
c   Version 1.0
c   -------
c   P. Jacobs   Department of Mechanical Engineering
c               University of Queensland
c
c   July 1989
c
c   Reference ...
c   ---------
c   J.H. Wilkinson & C.Reinsch "Handbook for automatic computation.
c   Vol II. Linear algebra."  Springer-Verlag 1971, pp357-358.
c
      real*8 xi, xr, yi, yr, zi, zr
      real*8 factor
c
      if (abs(yr) .gt. abs(yi)) then
         factor = yi / yr
         zr = (xr + factor * xi) / (factor * yi + yr)
         zi = (xi - factor * xr) / (factor * yi + yr)
      else
         factor = yr / yi
         zr = (factor * xr + xi) / (factor * yr + yi)
         zi = (factor * xi - xr) / (factor * yr + yi)
      endif
c
      return
      end

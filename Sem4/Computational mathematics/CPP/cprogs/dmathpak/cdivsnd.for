      Program test
c
c     Try out complex division.
c
      real*8 xr, xi, yr, yi, zr, zi
      complex*16 z
c
10    read (*,*) xr, xi, yr, yi
      call cdivsn(xr, xi, yr, yi, zr, zi)
      write (*,*) zr, zi
      z = dcmplx(xr, xi) / dcmplx(yr, yi)
      write (*, *) z
c
      goto 10
      end

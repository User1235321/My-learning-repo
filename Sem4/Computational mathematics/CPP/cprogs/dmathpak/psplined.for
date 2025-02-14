c* psplined.c
c  Exercise the periodic parabolic spline routine.
      parameter (M=30)
      real*8 y(0:M),v(0:M),w1(0:M),w2(0:M),w3(0:M)
      real*8 pi, delx
      integer n, j, flag

      pi = 4.0 * atan(1.0)
      n = 20
      delx = 2.0 * pi / n

      write (*,*) 'Data points'
      do 10 j=0,n
         y(j) = sin(delx * j)
         write (*,5) j, y(j)
  5      format (1x,i4,f10.4)
  10     continue

      call PSpline (y, v, n, flag, w1, w2, w3)
      write (*,*) 'Flag = ', flag
c
      write (*,*) 'Break-points'
      do 20 j = 0, n-1
         write (*,15) j, v(j)
  15     format (1x,i4,f10.4)
  20     continue

      end

      subroutine PSpline (y, v, n, flag, d, lastc, lastr)
      parameter (M=30)
      real*8 y(0:M)
      real*8 v(0:M)
      integer n
      integer flag
      real*8  d(0:M), lastc(0:M), lastr(0:M)

c  Purpose ...
c  -------
c  Fit a parabolic spline to the periodic data vector y where
c  y(0) = y(n).  The data points are assumed to be equally
c  spaced and in ascending order of x (the independent variable).
c
c  Input ...
c  -----
c  y -- vector of data points with y(0) = y(n)
c  n -- number of data points y(0) .. y(n)
c
c  Output ...
c  ------
c  v -- vector of break points such that subinterval j is
c       defined on v(j-1), y(j), v(j) and v(0) = v(n).
c       The equation on each subinterval is
c       s(e) = a * e * e + b * e + y(j)
c       where e = 2 (x - x(j)) / h
c             h = x(j+1) - x(j)
c             a = (v(j-1) - 2 y(j) + v(j)) / 2
c             b = (v(j) - v(j-1)) / 2
c             x is the independent variable (not explicitly used)
c  flag -- = 0 normal return
c          = 1 y(0) != y(n).  If this occurs the routine will still
c              fit a spline using y(0)
c          = 2 n < 3.  There is not much point in fitting a periodic
c              to this vector.
c
c  Workspace ...
c  ---------
c  d, lastr, lastc -- internal workspace with same
c                     dimensions as y() and v()
c
c  Version ... 1.0, march 1988
c  -------
c
c  Notes ...
c  -----
c  (1) Written by
c      P.A. Jacobs    Department of Mechanical Engineering
c                     University of Queensland
c
      integer j
      real*8 ratio, offdiag
      flag = 0
      if (n .lt. 3) then
         flag = 2
         return
      endif
      if (abs(y(0) - y(n)) .gt. 1.0e-10) flag = 1
      offdiag = 0.25
      do 10 j = 0,n-1
         d(j) = 1.5
         lastc(j) = 0.0
         lastr(j) = 0.0
  10     continue
      lastc(0) = offdiag
      lastr(0) = offdiag
      lastc(n-2) = offdiag
      lastr(n-2) = offdiag
      lastr(n-1) = d(n-1)
      do 20 j = 0,n-2
  20     v(j) = y(j+1) + y(j)
      v(n-1) = y(0) + y(n-1)
      do 30 j = 1, n-2
         ratio = offdiag / d(j-1)
         d(j) = d(j) - offdiag * ratio
         lastc(j) = lastc(j) - lastc(j-1) * ratio
         v(j) = v(j) - v(j-1) * ratio
  30     continue
      do 40 j = 0, n-2
         ratio = lastr(j) / d(j)
         if (j .ne. n-2) lastr(j+1) = lastr(j+1) - offdiag * ratio
         lastr(n-1) = lastr(n-1) - lastc(j) * ratio
         v(n-1) = v(n-1) - v(j) * ratio
  40     continue
      d(n-1) = lastr(n-1)
      v(n-1) = v(n-1) / d(n-1)
      v(n-2) = (v(n-2) - lastc(n-2) * v(n-1)) / d(n-2)
      do 50 j = n-3, 0, -1
  50     v(j) = (v(j) - lastc(j) * v(n-1) - offdiag * v(j+1)) / d(j)
      v(n) = v(0)
      return
      end

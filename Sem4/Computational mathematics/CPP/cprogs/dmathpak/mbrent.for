c     brent.for
c     One dimensional function minimizer.
c
      real*8 function brent (f, ax, bx, cx, tol, xmin, nfe, flag)
c
c     Purpose ...
c     -------
c     Given a function F, and given a bracketing triplet of abscissas
c     AX, BX, CX, and F(BX) is less than both F(AX) and F(CX),
c     this routine isolates the minimum to a fractional precision of
c     about TOL using brents method.
c
c     Input ...
c     -----
c     f        : externally defined objective function f(x)
c     ax,bx,cx : bracketing values of the abscissa
c     tol      : precision to which the minimum should be found
c     nfe      : number of function evaluations made before entry
c
c     Output ...
c     ------
c     xmin     : abscissa of minimum
c     brent    : minimum value of objective function
c     nfe      : total number of function evaluations
c     flag     : = 0, normal return
c                = 1, exceeded maximum number of iterations
c
c     Version ... 1.0  October, 1988
c     -------
c
c     Notes ...
c     -----
c     (1) For well behaved functions the value of TOL should
c         not be set less than the square root of the machine
c         precision.
c     (2) Adapted from the text
c         W.H. Press et al
c         Numerical Recipes. The art of scientific computing.
c         by
c         P.A. Jacobs & N.J. Lott
c         Department of Mechanical Engineering
c         University of Queensland
c---------------------------------------------------------------------
c
      parameter (itmax = 100, cgold = 0.3819660d0, zeps = 1.0d-16)
c     itmax = maximum allowed number of iterations
c     cgold = golden ratio
c     zeps  = a small number which protects against trying to achieve
c             fractional accuracy for a minimum that happens to be
c             exactly zero.
c
      integer nfe, flag
      real*8 ax, bx, cx, f, tol, xmin
      external f
c
      integer iter
      real*8 a, b, v, w, x, e, fx, fv, fw
      real*8 xm, tol1, tol2,r, q, p, etemp, d
      real*8 u, fu
c
      flag = 0
c
c     A and B must be in ascending order, though the abscissas
c     need not be.
      a = min(ax, cx)
      b = max(ax, cx)
c
c     Initializations ...
c
      v = bx
      w = v
      x = v
c     e will be the distance moved on the step before the last
      e = 0.0d0
      fx = f(x)
      nfe = nfe + 1
      fv = fx
      fw = fx
c
c     Main Loop ...
c
      do 11 iter = 1, itmax
         xm = 0.5d0 * (a + b)
         tol1 = tol * abs(x) + zeps
         tol2 = 2.0d0 * tol1
c
c        Test done here.
         if (abs(x-xm) .le. (tol2-0.5d0*(b-a))) goto 3
c
         if (abs(e) .gt. tol1) then
c           Construct a trial parabolic fit.
            r = (x - w) * (fx - fv)
            q = (x - v) * (fx - fw)
            p = (x - v) * q - (x - w) * r
            q = 2.0d0 * (q - r)
            if (q .gt. 0.0d0) p = -p
            q = abs(q)
            etemp = e
            e = d
c
c           Is the parabolic fit acceptable?
            if (abs(p) .ge. abs(0.5d0*q*etemp) .or. p .le. q*(a-x)
     &          .or. p .ge. q*(b-x)) goto 1
c
c           Yes, it is, take the parabolic step.
            d = p / q
            u = x + d
            if (u-a .lt. tol2 .or. b-u .lt. tol2) d = sign(tol1, xm-x)
c           Skip over the golden section step.
            goto 2
         endif
c
c        We arrive here for a golden step, which we take into the
c        larger of the two segments.
c
1        if(x .ge. xm) then
            e = a - x
         else
            e = b - x
         endif
c        Take the golden section step.
         d = cgold * e
c
c        Arrive here with D computed either from the parabolic fit
c        or the golden section
c
2        if (abs(d) .ge. tol1) then
            u = x + d
         else
            u = x + sign(tol1, d)
         endif
c
c        This is the one function evaluation per iteration.
         fu = f(u)
         nfe = nfe + 1
c
c        Now we have to decide what to do with the function value.
         if (fu .le. fx) then
            if (u .ge. x) then
               a = x
            else
               b = x
            endif
            v = w
            fv = fw
            w = x
            fw = fx
            x = u
            fx = fu
         else
            if (u .lt. x) then
               a = u
            else
               b = u
            endif
            if (fu .le. fw .or. w .eq. x) then
               v = w
               fv = fw
               w = u
               fw = fu
            else if (fu .le. fv .or. v .eq. x .or. v .eq. w) then
               v = u
               fv = fu
            endif
         endif
c
c        Return for another iteration.
11       continue
c
c     have exceeded maximum iterations
      flag = 1
c
3     xmin = x
      brent = fx
      return
      end

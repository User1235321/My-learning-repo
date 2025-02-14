c     conjgg.for
c     Multidimensional minimization using function derivatives.
c
      subroutine conjgg (f, df, p, n, ftol, fret, flag, itmax,
     &                  xmax, iter, nfe, nje, uvect, g, h, xi)
c
c     Purpose ...
c     -------
c     Given a starting point P, Fletcher-Reeves-Polak-Ribiere
c     minimization is performed on a function F, using its gradient
c     as calculated by a routine DF.
c
c     Input...
c     -----
c     f     : user defined objective function
c     df    : routine for evaluating derivatives
c     p     : starting point in n-dimensional space
c     n     : number of elements in p
c     ftol  : convergence tolerance on the function value
c     itmax : maximum allowed number of iterations
c     xmax  : bounds on parameter values for 1D minimization
c
c     Output...
c     ------
c     fret  : minimum of f
c     iter  : number of iterations performed
c     nfe   : number of function evaluations
c     nje   : number of derivative evaluations
c     flag  : = 0, normal return
c             = 1, did not converge within itmax iterations
c             = 2, could not bracket a minimum in a line minimization
c
c     Workspace...
c     ---------
c     uvect    : vector of dimension n
c     g, h, xi : vectors of dimension n
c
c     Version... 1.0, October 1988
c     -------
c
c     Notes ...
c     -----
c     (1) Uses routines LINEM() and BRAKET() to perform the one-
c         dimensional minimizations.
c     (2) Adapted from the FORTRAN code FRPRMN in
c         W.H. Press et al
c         Numerical Recipes. The art of scientific computing.
c         by
c         P.A. Jacobs & N.J. Lott
c         Department of Mechanical Engineering
c         University of Queensland
c
      parameter (eps = 1.d-16)
c
      external f, df
      real*8 f
      real*8 ftol, fret, xmax, linem
      real*8 p(*), g(*), h(*), xi(*), uvect(*)
      integer n, itmax, iter, flag
c
      integer j, Lflag, its
      real*8 fp, gg, dgg, gam, xmin
c
      nfe = 0
      fp = f(p)
      nfe = nfe + 1
      call df(p, xi)
      nje = nje + 1
c
      do 11 j = 1, n
         g(j) = -xi(j)
         h(j) = g(j)
         xi(j) = h(j)
11       continue
c
c     Iterate !
c
      do 14 its = 1, itmax
         iter = its
c        Search along direction xi
         fret = linem (f, p, xi, n, ftol, 100, xmin, xmax, uvect,
     &                 nfe, Lflag)
         if (Lflag .ne. 0) then
            flag = 2
            return
         endif
c
c        Check convergence.
         if (abs(fret-fp) .le.
     &       ftol * (1.0d0 + 0.5d0*(abs(fret)+abs(fp))) + eps) then
c           Normal return
            flag = 0
            return
         endif
c
         fp = f(p)
         nfe = nfe + 1
         call df(p, xi)
         nje = nje + 1
         gg = 0.0d0
         dgg = 0.0d0
c
         do 12 j = 1, n
            gg = gg + g(j)**2
c           The following statement for Fletcher-Reeves
c           dgg = dgg + xi(j)**2
c           The following statement for Polak-Ribiere
            dgg = dgg + (xi(j) + g(j)) * xi(j)
12          continue
c
         if (gg .eq. 0.0d0) then
c           Unlikely, but if the gradients are zero then we are done.
            flag = 0
            return
         endif
c
c        Determine a new direction
         gam = dgg / gg
         do 13 j = 1, n
            g(j) = -xi(j)
            h(j) = g(j) + gam * h(j)
            xi(j) = h(j)
13          continue
c
14     continue
c
c     Too many iterations without convergence.
      flag = 1
      return
      end
c
c---------------------------------------------------------------------
c
c     One dimensional function minimizer along a specified line.
c
      real*8 function linem (f, pvect, direct, n,
     &                       tol, itmax, xmin, bound,
     &                       uvect, nfe, flag)
c
c     Purpose ...
c     -------
c     Given a function F, a starting point PVECT and a direction
c     to search DIRECT, this routine first brackets and then
c     isolates the minimum to a fractional precision of
c     about TOL using Brent's method.
c
c     Input ...
c     -----
c     f        : externally defined objective function f(x)
c     pvect    : origin of line along which to search
c     direct   : vector direction of search
c     n        : number of elements in pvect
c     tol      : precision to which the minimum should be found
c                For well behaved functions the value of TOL should
c                be set greater than the square root of the machine
c                precision.
c     itmax    : number of iterations allowed
c                There is one function evaluation per iteration.
c                For a well behaved function, 100 should be plenty.
c     bound    : limit on the magnitude of the distance moved along
c                the search line (say 1000.0)
c     nfe      : number of function evaluations made before entry
c
c     Output ...
c     ------
c     xmin     : parameter value at minimum
c     pvect    : vector "abscissa" at minimum
c     direct   : initial direction scaled by xmin
c     linem    : minimum value of objective function
c     nfe      : total number of function evaluations
c     flag     : = 0, normal return
c                = 1, exceeded maximum number of iterations
c                = 2, out of bounds without bracketing, no valid
c                     result is returned
c
c     WorkSpace ...
c     ---------
c     uvect    : vector of dimension n defining a point
c                It has elements
c                uvect(jj) = pvect(jj) + u * direct(jj)
c                where u is a parameter measuring along
c                the line "DIRECT".
c
c     Version ... 1.0  October, 1988
c     -------     2.0  July     1989
c
c     Notes ...
c     -----
c     (1) Adapted from the text
c         W.H. Press et al
c         Numerical Recipes. The art of scientific computing.
c         by
c         P.A. Jacobs & N.J. Lott
c         Department of Mechanical Engineering
c         University of Queensland
c     (2) Does not require derivative information.
c---------------------------------------------------------------------
c
      parameter (cgold = 0.3819660d0, zeps = 1.0d-16)
c     cgold = golden ratio
c     zeps  = a small number which protects against trying to achieve
c             fractional accuracy for a minimum that happens to be
c             exactly zero.
c
      integer nfe, flag, n, itmax
      real*8 pvect(*), uvect(*), direct(*)
      real*8 f, tol, xmin, bound
      external f
c
      integer iter, jj, bflag
      real*8 ax, bx, cx, fa, fb, fc
      real*8 a, b, v, w, x, e, fx, fv, fw
      real*8 xm, tol1, tol2,r, q, p, etemp, d
      real*8 u, fu
c
      flag = 0
c
c     Bracket the minimum
c
c     First, guess the bracket
      ax = 1.0
      bx = 2.0
c     then, improve it
      call braket (f, pvect, direct, n, ax, bx, cx, bound,
     &             fa, fb, fc, uvect, nfe, bflag)
      if (bflag .ne. 0) then
c        Could not bracket minimum in bounds
         flag = 2
         linem = 0.0d0
         return
      endif
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
      do 101 jj = 1, n
         uvect(jj) = pvect(jj) + x * direct(jj)
101      continue
      fx = f(uvect)
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
c        Test done here. If converged then exit.
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
c           Now take the step
c
            if (abs(p) .ge. abs(0.5d0*q*etemp) .or. p .le. q*(a-x)
     &          .or. p .ge. q*(b-x) ) then
c
c              Take a golden step, into the larger
c              of the two segments.
               if(x .ge. xm) then
                  e = a - x
               else
                  e = b - x
               endif
               d = cgold * e
c
            else
c
c              The parabolic step is ok, take it.
               d = p / q
               u = x + d
               if (u-a .lt. tol2 .or. b-u .lt. tol2)
     &            d = sign(tol1, xm-x)
            endif
c
         else
c
c           Take a golden step, into the larger of the two segments.
            if(x .ge. xm) then
               e = a - x
            else
               e = b - x
            endif
            d = cgold * e
         endif
c
c        Arrive here with D computed either from the parabolic fit
c        or the golden section
c
         if (abs(d) .ge. tol1) then
            u = x + d
         else
c           Move at least a little.
            u = x + sign(tol1, d)
         endif
c
c        This is the one function evaluation per iteration.
         do 102 jj = 1, n
            uvect(jj) = pvect(jj) + u * direct(jj)
102         continue
         fu = f(uvect)
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
            if (fu .le. fw .or. abs(w - x) .lt. zeps) then
               v = w
               fv = fw
               w = u
               fw = fu
            else if (fu.le.fv .or. abs(v-x).lt.zeps .or.
     &               abs(v-w).lt.zeps) then
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
c     Return the best guess for the minimum even if we did not
c     achieve the desired tolerance.
c
3     xmin = x
      do 104 jj = 1, n
         direct(jj) = direct(jj) * xmin
         pvect(jj) = pvect(jj) + direct(jj)
104      continue
      linem = fx
      return
      end
c
c--------------------------------------------------------------------
c
c     Bracket a minimum along a line.
c
      subroutine braket (f, pvect, direct, n,
     &                   ax, bx, cx, bound, fa, fb, fc,
     &                   uvect, nfe, flag)
c
c     Purpose ...
c     -------
c     Given a function F, a point in N-dimensional space,
c     a direction to search and given distinct initial parameter
c     values AX and BX, this routine searches in the downhill
c     direction (defined by the function evaluated at the initial points)
c     and returns new parameter values AX, BX, CX which bracket
c     a minimum of the function.
c
c     Input ...
c     -----
c     f      : externally defined objective function that returns
c              a value for each n-dimensional point
c     pvect  : origin for line along which to search
c     direct : direction vector for search
c     n      : number of elements in pvect
c     ax     : guess for left bracketing parameter
c     bx     : guess for right bracketing parameter
c     bound  : limit on magnitude of ax, bx, cx (say 1000.0)
c     nfe    : number of function evaluations so far
c
c     Output ...
c     ------
c     ax, bx, cx : values of parameter bracketing a minimum
c                  such that fc < fb < fa and cx lies between
c                  ax and bx
c     fa, fb, fc : values of the objective function at ax, bx and cx
c     nfe        : number of function evaluations
c     flag       : = 0, normal return
c
c     Workspace ...
c     ---------
c     uvect  : n-dimensional points corresponding to parameter u
c              where uvect(jj) = pvect(jj) + u * direct(jj)
c
c     Version ... 1.0, October 1988.
c     -------
c
c     Notes ...
c     -----
c     (1) gold = default ratio by which successive intervals are
c                magnified
c     (2) glimit = maximum magnification allowed by the parabolic-fit
c                  step
c     (3) Adapted from the FORTRAN code MNBRAK in
c         W.H. Press et al
c         Numerical Recipes. The art of scientific computing.
c         by
c         P.A. Jacobs & N.J. Lott
c         Department of Mechanical Engineering
c         University of Queensland
c----------------------------------------------------------------------
c
      parameter (gold = 1.618034d0, glimit = 100.0d0, tiny = 1.0d-20)
      parameter (zero = 0.0d0)
c
      real*8  f
      integer n, flag, nfe
      real*8  pvect(*), direct(*)
      real*8  ax, bx, cx, fa, fb, fc, bound
      real*8  uvect(*)
c
      external f
c
      integer jj
      real*8  u, fu, temp, r, q, ulim
c
      flag = 0
c
      do 10 jj = 1, n
         uvect(jj) = pvect(jj) + ax * direct(jj)
 10      continue
      fa = f(uvect)
      do 11 jj = 1, n
         uvect(jj) = pvect(jj) + bx * direct(jj)
 11      continue
      fb = f(uvect)
      nfe = nfe + 2
c
      if (fb .gt. fa) then
c        Switch roles of A and B so that we go downhill in the
c        direction from A to B
         temp = ax
         ax = bx
         bx = temp
         temp = fb
         fb = fa
         fa = temp
      endif
c
c     First guess for C
      cx = bx + gold * (bx - ax)
      do 30 jj = 1, n
         uvect(jj) = pvect(jj) + cx * direct(jj)
 30      continue
      fc = f(uvect)
      nfe = nfe + 1
c
c     DO WHILE : keep returning here until we bracket
1     if (fb .ge. fc) then
c        Compute U by parabolic extrapolation until we bracket.
         r = (bx - ax) * (fb - fc)
         q = (bx - cx) * (fb - fa)
c        Tiny is used to prevent possible division by zero.
         u = bx - ((bx - cx) * q - (bx - ax) * r) /
     &             (2.0d0 * sign(max(abs(q-r),tiny),q-r))
         do 40 jj = 1, n
            uvect(jj) = pvect(jj) + u * direct(jj)
 40         continue
c        We won't go farther than ulim.
         ulim = bx + glimit * (cx - bx)
c
c        Now test various possibilities...
         if ((bx - u) * (u-cx) .gt. zero) then
c           Parabolic U is between B and C, try it
            fu = f(uvect)
            if (fu .lt. fc) then
c              Got a minimum between B and C
               ax = bx
               fa = fb
               bx = u
               fb = fu
C              exit
               go to 1
            else if (fu .gt. fb) then
c              Got a minimum between A and U
               cx = u
               fc = fu
c              exit
               go to 1
            endif
C           Parabolic fit was no use. Use default magnification.
            u = cx + gold * (cx - bx)
            do 48 jj = 1, n
               uvect(jj) = pvect(jj) + u * direct(jj)
  48           continue
            fu = f(uvect)
            nfe = nfe + 1
c
         else if ((cx - u) * (u - ulim) .gt. zero) then
c           Parabolic fit is between C and its allowed limit.
            fu = f(uvect)
            nfe = nfe + 1
            if (fu .lt. fc) then
               bx = cx
               fb = fc
               cx = u
               fc = fu
               u = cx + gold * (cx - bx)
               do 54 jj = 1, n
                  uvect(jj) = pvect(jj) + u * direct(jj)
  54              continue
               fu = f(uvect)
               nfe = nfe + 1
            endif
c
         else if ((u - ulim) * (ulim - cx) .ge. zero) then
c           Limit parabolic U to its maximum allowed value.
            u = ulim
            do 56 jj = 1, n
               uvect(jj) = pvect(jj) + u * direct(jj)
  56           continue
            fu = f(uvect)
            nfe = nfe + 1
c
         else
c           Reject parabolic U, use default magnification.
            u = cx + gold * (cx - bx)
            do 58 jj = 1, n
               uvect(jj) = pvect(jj) + u * direct(jj)
  58           continue
            fu = f(uvect)
            nfe = nfe + 1
         endif
c
c        Eliminate oldest point and continue.
         ax = bx
         fa = fb
         bx = cx
         fb = fc
         cx = u
         fc = fu
c
c        Check limit on parameter values
         if (abs(u) .gt. bound) then
c           We are out of bounds without bracketing
            flag = 1
            return
         endif
c
c        Take another step
         go to 1
      endif
c
      return
      end

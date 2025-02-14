c     bracket.for
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

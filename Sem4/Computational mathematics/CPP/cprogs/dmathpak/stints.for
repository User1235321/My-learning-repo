c  stints.for   -- a not-so-well structured version of stint.
c                  This code is difficult to understand by itself
c                  so I hope that I have not introduced too many BUGS.
c                  Note that it will not even compile on some machines
c                  because of a jump into the body of a DO loop.
c
c  P. Jacobs march 1987
c-----------------------------------------------------------------------
        subroutine stint1(n,n0,t,tout,hi,error,mf,y,y0,
     &          ydot,saved,rj,ymax,rw,ipiv,kflag,diffun,pederv)
c
c       Easy to use version of stint.
c
c       Input:
c               n       :order of system.
c               n0      :declared dimension of arrays.
c               t,tout  :initial,final values of independent variable,t
c               hi      :initial step size.
c               error   :relative error tolerance.
c               mf      :method flag.
c                        =1,jacobian must be supplied.
c                        =2,no jacobian need be supplied.
c               y       :initial value of dependent variable,y.
c
c       Output:
c               t       :(=tout),value of independent variable.
c               y       :value of dependent variable at t.
c               kflag   :completion code.
c
        double precision t,tout,hi,error,hmax,hnext,hmin,h0,
     &          y(n),y0(n,8,4),ydot(1),
     &          saved(1),rj(1),ymax(n),rw(1),ts,s,d
        integer ipiv(n)
        external diffun,pederv
c
c       set the normalizing vector matrix ymax.
c
        do 10 i = 1,n
          ymax(i) = max(abs(y(i)),1.0d0)
          y0(i,1,1) = y(i)
   10   continue
        hmax = (tout - t)*10.0d0
        hnext = hi
        hmin = hi*0.01d0
        maxder = 7
        jstart = 0
   20   call stint(n,t,y0,ydot,saved,h0,hnext,hmin,hmax,error,
     &          ymax,kflag,knext,jstart,maxder,rw,rj,mf,ipiv,
     &          diffun,pederv)
        if (kflag .lt. 0) return
   30   kflgp1 = kflag + 1
c
c       check whether the computed solution reached beyond the
c       interpolation point tout.
c
        ii = 0
        do 40 i = 1,kflgp1
          ii = ii + 1
          ts = tout - t + h0*dble(i - 1)
          if (ts .ge. 0.0) then
            if (i .le. 1) then
              goto 20
            else
              goto 50
            endif
          endif
   40     continue
c
c       the solution reached beyond tout.
c       perform interpolation at tout.
c
   50   ind = kflag + 3 - ii
        if (ii .eq. 2) ind = 1
        s = (ts - h0)/h0
        t = tout
        do 61 i = 1,n
          d = 1.0
          y(i) = y0(i,1,ind)
          do 60 j = 1,jstart
            d = d*(dble(j - 1) + s)/dble(j)
            y(i) = y(i) + d*y0(i,j+1,ind)
   60     continue
   61   continue
        return
        end


c----------------------------------------------------------------------
c   The code that follows is the not-so-easy-to-use version of stint
c----------------------------------------------------------------------

      subroutine stint (n, t, y, dy, saved, h, hnext, hmin, hmax, eps,
     +                  ymax, kflag, knext, jstart, maxord, rw, rj, mf,
     +                  ipiv,diffun,pederv)
      external diffun,pederv
c
c   This program integrates a set of n first order ordinary
c   differential equations.  A block of three or four solution points,
c   each separated by a step-size h, is computed at each call. The
c   step-size magnitude may be specified by the user at each call.
c   Alternatively, it may be increased or decreased by stint within
c   the range  abs(hmin) to  abs(hmax),
c   in order to achieve as large a step as possible, while not
c   commiting a single step error which is larger than eps in the
c   rms norm, where each component of the error vector is divided by
c   the corresponding component of ymax.
c
c    The program requires 4 subroutines named
c
c        diffun(n, t, y, dy)
c        pederv(n, t, y, rj, n0)
c        dec(n, n0, a, ipiv, ier)
c        sol(n, n0, a, b, ipiv)
c
c    diffun evaluates the derivatives of the dependent variable y(i)
c   for i=1,...,n and t and stores the result in the array dy.
c
c    pederv computes the partial derivatives of the differential
c  equations at the values y(i) for i=1,...,n and t and stores the
c  result in array rj.  Thus, rj(j+(k-1)*n0) is the partial of dy(j)
c  with respect to y(k) for j,k=1,...,n evaluated at y(i) for i=1,..
c ..,n and t.  Note--n0 is the value of n on the first call.  If the
c  analytic expressions for the partial derivatives are not
c  available, their approximate values can be obtained by numerical
c  differencing.  (see parameter mf.)  In this case subroutine pederv
c  may simply be
c
c      subroutine pederv(n,t,y,rj,n0)
c      return
c      end
c
c   dec performs an lu decomposition of a matrix a. If the
c  decomposition is successful ier should be set to 0, otherwise it
c  should be set to +1.
c
c  sol solves the linear algebraic system a*x=b, for which the
c  matrix a was processed by dec.
c
c  This program uses double precision for all floating point
c  variables, except for those starting with p, which are in single
c  precision.
c
c   Temporary storage space is provided (by the user) in the
c  arrays ipiv, rj, rw, and saved. The array ipiv holds a vector of
c  the same name. The arrays rj and rw are used to hold matrices of
c  the same names.  The array saved is partitioned as follows
c
c   saved(j,i)     1.le.j.le.8 and 1.le.i.le.n is used to saved
c                 y(i,j) in case a step (and hence the whole cycle)
c                 has to be repeated.
c   saved(9,i)     1.le.i.le.n is used to store the derivative of the
c                 i-th dependent variable scaled by h.
c   saved(n9+i,1)  is used to store the derivatives as they are
c                 computed by diffun for the corrector. It is also
c                 accessed as a complete array saved(n9p1,1).
c                 In addition it is used in the error control test.
c                 (n9 = n0 * 9  and  n9p1 = n9 + 1.)
c   saved(n10+i,1) is used to hold the correction terms for the entire
c                 corrector iteration in the case, the corrector has
c                 to be repeated. (n10 = n0 * 10.)
c   saved(n11+i,1) is used to hold the derivatives evaluated at
c                 y(i)+d and t, where d is the increment used in the
c                 numerical differencing scheme invoked, in order to
c                 obtain approximate values of the partial
c                 derivatives. (n11 = n0 * 11.)
c   saved(n12+i,1) is used to hold the derivatives evaluated at y(i)
c                 and t in order to obtain approximate values of the
c                 partial derivatives. (n12 = n0 * 12.)
c
c  n        the number of first order differential equations to be
c            integrated.  n may be decreased on subsequent calls if
c            the number of active equations decreases, with the
c            first ones being those retained.  But, it must
c            not be increased without calling with jstart = 0.
c  t        the independent variable.  On entry t is the current
c            setting of the independent variable.  On return to the
c            calling program, t corresponds to the setting of the
c            independent variable for the most forward point obtained
c            thus far.
c  y        an n by 8 by 4 array containing the dependent variables
c            and their backward differences.  On each call up to four
c            solution points are obtained. The most forward point is
c            always at y(i,1,1). The point next to the most forward
c            point is returned in y(i,1,kflag). The most backward
c            point in the new block is returned in y(i,1,2). Only the
c            initial solution values, entered in y(i,1,1) for
c            i=1,...,n, need to be supplied on the first
c            call (jstart = 0).
c            y(i,j+1,k) contains the j-th backward difference of the
c            i-th dependent variable (for k=1,...,kflag).
c            if it is desired to interpolated to non-mesh points,
c            these values can be used.  If the current step-size is
c            h and the value at t-e (0.lt. abs(e).lt. abs(h)) is
c            needed, form s=e/h and then compute
c                           nq
c              y(i)(t-e) = sum y(i,j+1,k)*b(j)
c                          j=0
c            where k, which corresponds to t, is the first mesh point
c            beyond the point t-e, and b(j+1) = b(j)*(j-s)/(j+1)
c            with b(0) = 1.
c  dy       an n by 4 array.  dy(i,k), 1.le.i.le.n, 1.le.k.le.kflag,
c            contains the derivative of the i-th dependent variable
c            scaled by h.  The dy(i,1) array need not be supplied at
c            the first call (jstart = 0).
c  saved     a block of at least 13*n0 double precision floating point
c            locations.
c  h        the step-size used for the just completed bock.
c  hnext    the step-size for the next block.  On the
c            first call (jstart=0) the user must specify an initial
c            step-size.  A good estimate of its magnitude is given
c            by 0.2*(eps/ abs(e))**0.5, where e is the largest
c            eigenvalue of the jacobian evaluated at the initial
c            values of t and y.  The sign of the initial hnext
c            should be positive (negative) if the final time is
c            greater (less) than the initial time.  If the initial
c            step-size choice does not cause an error greater than
c            eps, in the rms norm, it will be accepted. Otherwise,
c            its magnitude will be decreased until an error less
c            than eps is achieved.  The subroutine automatically
c            adjusts the step-size after the initial and subsequent
c            calls for the step-size of largest possible magnitude.
c            The magnitude may be adjusted down on any subsequent
c            call.  Note--a magnitude adjustment up or a sign change
c            will be ignored.
c  hmin      abs(hmin) is the minimum step-size magnitude to be
c            allowed for the next integration cycle.  (On the first
c            call (jstart=0),  abs(hmin) should be chosen signif-
c            icantly smaller than  abs(hnext) so as to avoid start-up
c            problems if the error criterion is not met with the user
c            specified hnext.)  Note--the sign of hmin is ignored.
c            hmin may be changed on subsequent calls.
c  hmax     abs(hmax) is the maximum step-size magnitude to be
c            allowed for the next integration cycle.  Note--if
c             abs(hmax) is less than  abs(hmin), then the subroutine
c            functions as though  abs(hmax) equals  abs(hmin). hmax
c            may be changed on subsequent calls.
c  eps      the error test constant.  The single step error estimate
c            for y, computed as a weighted rms norm, must not exceed
c            eps.  The weight for the i-th element in the error
c            estimate is 1/ymax(i).  (See parameter ymax.)  The
c            step-size and/or order are adjusted to achieve this.
c  ymax     an array of n0 locations ,with--for i=1,..,n--ymax(i)
c            being the maximum of unity and the maximum value of
c            abs(y(i)) seen thus far.  On the first call it should
c            be set to the maximum of unity and the initial value of
c            abs(y(i)).
c  kflag    a completion code. If kflag is greater than 0,then
c            kflag points have been computed. If kflag is
c            -2, -3, or -4 then 2, 3, or 4 mesh points, respectively,
c            have been computed with  abs(h) equal to  abs(hmin),
c            but the requested error was not achieved.  Other values
c            kflag can assume are as follows
c             -5  the requested error was smaller than can be handled
c                 for this problem.
c             -6  corrector convergence could not be achieved for
c                  abs(h).gt. abs(hmin).
c             -7  the maximum order specified was too large.
c  knext    after the initial call (jstart=0),the value of knext is
c            the number of points to be computed during the next
c            cycle.  the value is supplied for information only.the
c            user cannot control the number of points by setting
c            knext.
c   jstart  an input indicator with the following meanings
c             .eq.0  initialization call.  (jstart must be set to 0
c                    on the first call.)
c             .gt.0  continue from the last step.
c            on return jstart is set to nq, the maximum backward
c            difference available in the y array.  (This also corres-
c            ponds to the order of the method used to compute the
c            just completed block of points.)
c  maxord   the maximum order (1.le.maxord.le.7) that may be used.
c            Note--if maxord is reset between cycles to a value less
c            than the order determined for the next cycle, then the
c            order may for several cycles exceed maxord.  However,
c            it cannot exceed the above determined value and, once
c            the order is less than or equal to maxord, it cannot
c            then exceed maxord.
c   rj      a block of at least n0**2 double precision floating point
c            locations, which contains an estimate of the jacobian
c            of the differential equation.
c   rw      a block of at least n0**2 double precision floating point
c            locations.
c   ipiv    a block of at least n0 integers used to hold pivot data
c            generated during an lu decomposition.
c   mf      method flag.  It determines the mode by which the partial
c            derivatives are obtained with the following meanings
c            1  analytic expressions for the partial derivatives are
c               supplied by the user in the subroutine pederv.
c            2  the analytic expressions are not available.
c               approximate values of the partial derivatives are
c               obtained by numerical differencing.
c
c--------------------- declarations ---------------------------------
c
      double precision b, bnd, c, crate, d, df, di, dy,
     +                 d1, d2, d3, e, edown, enqdwn, enqsam,
     +                 enqup, eps, es, eup, fn, h, hmax, hmin,
     +                 hnew, hnext, hold, q, ratio, rc, rj, rmax,
     +                 rrdown, rrsame, rrup, rw, saved, sqrtur, t,
     +                 tdl, told, uround, y, yj1, ymax
      double precision temp
      dimension y(n,8,4), dy(n,4), saved(9,1), ymax(1), rw(1), ipiv(1),
     +          index(7,2), b(82), c(16), perror(9), pc(16), pd(7),
     +          rj(1)
      common /stat/ nstep,nfe,nje,ninvs
c
c---------------------- constants ------------------------------------
c
c  seven of eight data statements contain
c  array names and various  lines of code
c  include subscript expressions
c
c  the array index holds pointers and constants for the various
c  order methods.  for nq=1,...,7 the entries are as follows
c   index(nq,1)   base index for b array (h*dy predictor).
c   index(nq,2)   base index for c array (corrector).
c
c     data index/ 1, 2, 4, 11, 20, 38, 59,
c    +            1, 2, 3,  5,  7, 10, 14/
      index(1,1) = 1
      index(2,1) = 2
      index(3,1) = 4
      index(4,1) = 11
      index(5,1) = 20
      index(6,1) = 38
      index(7,1) = 59
      index(1,2) = 1
      index(2,2) = 2
      index(3,2) = 3
      index(4,2) = 5
      index(5,2) = 7
      index(6,2) = 10
      index(7,2) = 14
c
c  the coefficients in the perror array are used in the error test,
c  the first time it is performed, as well as in the step-size/order
c  selection segment. perror(i+1) = 1/d(i+1), i=1,...,7, where
c  d(i+1) is the discretization error constant corresponding to the
c  second pass of the integration cycle of order i. perror(1) and
c  perror(9) are defined solely for programming ease. they are not
c  used.
c
c     data perror/  1.0, 1.0, 1.92857, 2.78161, 3.56735,
c    +              4.29497, 4.9065, 5.6066, 1.0/
      perror(1) = 1.0
      perror(2) = 1.0
      perror(3) = 1.92857
      perror(4) = 2.78161
      perror(5) = 3.56735
      perror(6) = 4.29497
      perror(7) = 4.9065
      perror(8) = 5.6066
      perror(9) = 1.0
c
c  the coefficients in the array pc are used both in the convergence
c  and error tests. they are the reciprocal values of the
c  discretization error constants for equations constituting the
c  methods of order 1 thru 7.
c
c     data pc/  2.0, 4.5, 7.3333, 6.0, 10.4167, 9.3, 13.7, 13.8687,
c    +          9.6904, 17.15, 16.9504, 17.4349, 9.472, 20.7429, 15.921,
c    +          14.7809/
      pc(1) = 2.0
      pc(2) = 4.5
      pc(3) = 7.3333
      pc(4) = 6.0
      pc(5) = 10.4167
      pc(6) = 9.3
      pc(7) = 13.7
      pc(8) = 13.8687
      pc(9) = 9.6904
      pc(10) = 17.15
      pc(11) = 16.9504
      pc(12) = 17.4349
      pc(13) = 9.472
      pc(14) = 20.7429
      pc(15) = 15.921
      pc(16) = 14.7809
c
c  the coefficients appearing in the array pd are used in the testing
c  of the *outdatedness* of the array rw.  the pd(i) element contains
c  the average value of the coefficients in array c corresponding to
c  order i.
c
c     data pd/  1.0, 0.6667, 0.6061, 0.4981, 0.4644, 0.4368, 0.412/
c
      pd(1) = 1.0
      pd(2) = 0.6667
      pd(3) = 0.6061
      pd(4) = 0.4981
      pd(5) = 0.4644
      pd(6) = 0.4368
      pd(7) = 0.412
c  the constant uround should be set equal to the unit round-off
c  for the machine.  the constant sqrtur should be set equal to the
c  square root of uround.
c
      uround = 1.0d-16
      sqrtur = 1.0d-8
c
c  the coefficients appearing in the next data statements for the
c  b array should be defined to the maximum accuracy permitted by
c  the machine. they are, in the order specified,...
c   1
c   -2, 3
c   -9/2, -5/4, 11/2
c   -15/2, -3/4, 13/2, 2
c   -22/3, -8/3, -17/18, 25/3
c   -9, -9/4, -7/8, 35/4, 5/4
c   -125/12, -101/24, -71/36, -37/48, 137/12
c   -123/24, -1001/240, -707/360, -123/160, 1373/120, 1/10
c   -57/4, -25/8, -17/10, -169/240, 61/5, -1/20, 31/10
c   -137/10, -117/20, -46/15, -191/120, -197/300, 147/10
c   -353/25, -571/100, -1819/600, -79/50, -3919/6000, 1477/100, 7/20
c   -3529/200, -1889/400, -1609/600, -3527/2400, -931/1500, 3079/200,
c       -3/20, 17/5
c   -343/20, -303/40, -253/60, -589/240, -101/75, -23/40, 363/20
c   -266/15, -221/30, -749/180, -73/30, -803/600, -103/180, 547/30,
c       1/2
c   -1316/75, -1151/150, -3689/900, -121/50, -4003/3000, -257/450,
c       2737/150, -1/5, 1/2
c
       b(1) = 1.0d0
       b(2) = -2.0d0
       b(3) = 3.0d0
       b(4) = -4.5d0
       b(5) = -1.25d0
       b(6) = 5.5d0
       b(7) = -7.5d0
       b(8) = -.75d0
       b(9) = 6.5d0
       b(10) = 2.0d0
       b(11) = -7.3333333333333333d0
       b(12) = -2.6666666666666667d0
       b(13) = -.94444444444444444d0
       b(14) = 8.3333333333333333d0
       b(15) = -9.0d0
       b(16) = -2.25d0
       b(17) = -.875d0
       b(18) = 8.75d0
       b(19) = 1.25d0
       b(20) = -10.416666666666667d0
       b(21) = -4.2083333333333333d0
       b(22) = -1.9722222222222222d0
       b(23) = -.77083333333333333d0
       b(24) = 11.416666666666667d0
       b(25) = -10.541666666666667d0
       b(26) = -4.1708333333333333d0
       b(27) = -1.9638888888888889d0
       b(28) = -.76875d0
       b(29) = 11.441666666666667d0
       b(30) = 0.1d0
       b(31) = -14.25d0
       b(32) = -3.125d0
       b(33) = -1.7d0
       b(34) = -.70416666666666667d0
       b(35) = 12.2d0
       b(36) = -.05d0
       b(37) = 3.1d0
       b(38) = -13.7d0
       b(39) = -5.85d0
       b(40) = -3.0666666666666667d0
       b(41) = -1.5916666666666667d0
       b(42) = -.65666666666666667d0
       b(43) = 14.7d0
       b(44) = -14.12d0
       b(45) = -5.71d0
       b(46) = -3.0316666666666667d0
       b(47) = -1.58d0
       b(48) = -.65316666666666667d0
c
       b(48+1) = 14.77d0
       b(48+2) = 0.35d0
       b(48+3) = -17.645d0
       b(48+4) = -4.7225d0
       b(48+5) = -2.6816666666666667d0
       b(48+6) = -1.4695833333333333d0
       b(48+7) = -.62066666666666667d0
       b(48+8) = 15.395d0
       b(48+9) = -.15d0
       b(48+10) = 3.4d0
       b(48+11) = -17.15d0
       b(48+12) = -7.575d0
       b(48+13) = -4.2166666666666667d0
       b(48+14) = -2.4541666666666667d0
       b(48+15) = -1.3466666666666667d0
       b(48+16) = -.575d0
       b(48+17) = 18.15d0
       b(48+18) = -17.733333333333333d0
       b(48+19) = -7.3666666666666667d0
       b(48+20) = -4.1611111111111111d0
       b(48+21) = -2.4333333333333333d0
       b(48+22) = -1.3383333333333333d0
       b(48+23) = -.57222222222222222d0
       b(48+24) = -18.23333333333333d0
       b(48+25) = 0.5d0
       b(48+26) = -17.546666666666667d0
       b(48+27) = -7.6733333333333333d0
       b(48+28) = -4.0988888888888889d0
       b(48+29) = -2.42d0
       b(48+30) = -1.3343333333333333d0
       b(48+31) = -.57111111111111111d0
       b(48+32) = 18.246666666666667d0
       b(48+33) = -0.2d0
       b(48+34) = 0.5d0
c
c  the coefficients appearing the the next data statements for the
c  c array should be defined to the maximum accuracy permitted by
c  the machine.  they are, in the order specified,...
c   -1
c   -2/3
c   -6/11, -2/3
c   -12/25, -16/31
c   -60/137, -600/1373, -100/193
c   -20/49, -120/293, -75/184, -1200/2299
c   -140/363, -60/143, -1050/2437
c
       c(1) = -1.0d0
       c(2) = -.66666666666666667d0
       c(3) = -.545454545454545d0
       c(4) = -.66666666666666667d0
       c(5) = -.48d0
       c(6) = -.51612903225806452d0
       c(7) = -.43796520437956204d0
       c(8) = -.43699927166788056d0
       c(9) = -.51813471502590674d0
       c(10) = -.40816326530612245d0
       c(11) = -.40955631399317406d0
       c(12) = -.40760869565217391d0
       c(13) = -.52196607220530666d0
       c(14) = -.38567493112947659d0
       c(15) = -.41958041958041958d0
       c(16) = -.43085761181780879d0
c
c------------------ start of the code --------------------------------
c
c     --- on the first call jstart = 0, on subsequent calls jstart > 0
c
      kflag = 0
      ifail = 0
      if (jstart .ne. 0) go to 80
c
c     --- initialization --- first call
c
      n0 = n
      fn = dble(n)
      n9 = n0 * 9
      n9p1 = n9 + 1
      n10 = n9 + n0
      n11 = n10 + n0
      n11p1 = n11 + 1
      n12 = n11 + n0
      n12p1 = n12 + 1
      nsq = n0*n0
      iweval = +1
      tdl = t
      h = max(abs(hmin), min(abs(hnext), abs(hmax)))
      if (hnext .lt. 0.0) h = -h
c
c     --- Start afresh with order 1 method ...
c
   30 continue
      nqold = 0
      isw1 = 0
      isw2 = 0
      crate = 1.0
      call diffun (n, t, y, dy)
      nfe = nfe + 1
      do 40 i = 1, n
         dy(i,1) = h * dy(i,1)
   40    continue
      nq = 1
      ist = 1
      idel = 0
      go to 180
c
c    continue with the step-size h
c
   80 continue
      hnew = max(abs(hmin), min(abs(hnew), abs(hnext), abs(hmax)))
      if (h .lt. 0.0) hnew = -hnew
      if (h .eq. hnew) then
c        --- use the old step-size
         ist = nqp1
         idel = nqp1
         isw2 = 0
      else
c        --- new step-size --- interpolate for new points
         ratio = hnew / h
         h = ratio * h
         rc = rc * ratio * (pd(nq)/pdold)
         pdold = pd(nq)
         if (nq .ne. 1) then
            ieq = neq
            d = 0.0
            do 141 j=2, nq
               d = d + ratio
               if (d .gt. (dble(neq+nqp1-ieq))) ieq = 2
               d1 = (dble(neq-ieq-1)) - d
               do 140 i=1, n
                  d2 = 1.0
                  d3 = 0.0
                  do 120 j1=2, nqp1
                     d2 = d2 * ((dble(j1)) + d1) / (dble(j1-1))
                     d3 = d3 + d2 * y(i,j1,ieq)
  120                continue
                  y(i,j,1) = d3 + y(i,1,ieq)
  140             continue
  141          continue
         endif
c
         ist = nq
         idel = 0
         do 160 i=1, n
            dy(i,1) = dy(i,1) * ratio
  160       continue
         iret = 3
         go to 4000
      endif
c
c     initialize saved array
c
  180 continue
      do 201 i=1, n
         saved(9,i) = dy(i,1)
         do 200 j=1, ist
            saved(j,i) = y(i,j,1)
  200       continue
  201    continue
      nqst = nq
      ratio = 1.0
      told = t
      hold = h
c
  220 continue
      if ((nq.ne.nqold) .or. (fn.ne.(dble(n)))) then
         if ((nq.ne.nqold) .or. (fn.eq.(dble(n)))) then
            if (maxord .ge. 8) then
c              --- maximum order specified is too large --- bail out
               kflag = -7
               j1 = nqst + 1
               do 222 i=1, n
                  dy(i,1) = saved(9,i)
                  do 221 j=1,j1
                     y(i,j,1) = saved(j,i)
 221                 continue
 222              continue
               h = hold
               t = told
               jstart = nqst
               return
            endif
c
  260       continue
c           --- set appropriate parameters and constants
c               for new cycle of order nq
c           --- NOTE that this is used as an entry point (VERY naughty)
c               once the backward differences are formed
            indb = index(nq,1)
            indc = index(nq,2)
            neq = 3 + nq / 5
            jstart = nq
            nqold = nq
            nqm1 = nq - 1
            nqp1 = nq + 1
            q = (dble(nq))
            enqdwn = 0.5d0 / q
            enqsam = 0.5d0 / (q + 1.0)
            enqup = 0.5d0 / (q + 2.0d0)
         endif
         fn = (dble(n))
         edown = fn * ((perror(nq)) * eps)**2
         eup = fn * ((perror(nq+2)) * eps)**2
         es = fn * ((perror(nqp1)) * eps)**2
         if (edown .le. 0.0) then
c           --- the error tolerance requested for this problem is too
c               small
            kflag = -5
            j1 = nqst + 1
            do 224 i=1,n
               dy(i,1) = saved(9,i)
               do 223 j=1,j1
                  y(i,j,1) = saved(j,i)
 223              continue
 224           continue
            h = hold
            t = told
            jstart = nqst
            return
         endif
      endif
c
c     check for reevaluation of jacobian
c
      if(iweval .le. 0) then
         if (abs(rc-1.) .ge. 4.0d-1) iweval = +2
         if ((tdl-told)*h .le. 0.0) then
            if (abs(rc-1.0) .ge. 8.0d-1) iweval = +1
         endif
      endif
c
  320 continue
      indbb = indb
      indcc = indc
      do 1000 ieq=1,neq
        ist = mod (ieq, neq) + 1
        t = t + h
        bnd = fn * ((pc(indcc)) * enqup * eps)**2
        e = es
        if (ieq .gt. 2) e = fn * ((pc(indcc)) * eps)**2
c
c       predict y and dy for the next mesh point
c
        do 460 i=1,n
          d = dy(i,ieq)
          d1 = y(i,1,ieq) + q*d
          d2 = b(indbb+nqm1) * d
          if (nq .gt. 2) then
             if (ieq .gt. 3) d2 = d2 + b(indbb+nqp1) * dy(i,3)
             if (ieq .ge. 3) d2 = d2 + b(indbb+nq) * dy(i,2)
          endif
c
          if (nq .ge. 2) then
             do 420 j=2,nq
                d = y(i,j,ieq)
                d3 = (dble(j-1))
                d1 = d1 + d * (d3-q) / d3
                d2 = d2 + d * b(indbb+j-2)
  420           continue
          endif
          y(i,1,ist) = d1
  460     dy(i,ist) = d2
c
        if (nq .gt. 2) then
           if (ieq .gt. 1) indbb = indbb + nq + ieq - 2
        endif
c
c     iterate the corrector up to three times. accumulate the
c     correction terms in saved(n10+i,1) for redoing the entire
c     corrector if convergence is not achieved
c
        d1 = c(indcc)
  500   continue
        do 510 i=1,n
           saved(n10+i,1) = 0.0
  510      continue
        do 780 j=1,3
          call diffun (n, t, y(1,1,ist), saved(n9p1,1))
          nfe = nfe + 1
c
          if (iweval .eq. 1) then
             ind = 1
             if (ieq .eq. 2) ind = 1
c
             if (mf .eq. 2) then
c               --- evaluate partial derivatives using finite differences
                call diffun (n, t-h*(dble(1+ieq-ind)), y(1,1,ind),
     +            saved(n12p1,1))
                nfe = nfe + 1
                d = 0.0
                do 540 i=1,n
                   d = d + saved(n12+i,1)**2
  540              continue
                d =  abs(h) * 1.d3 * uround * sqrt(d)
                nje = nje + 1
                j0 = 0
                do 560 j1=1,n
                   di = sqrtur * ymax(j1)
                   di = max(di,d)
                   yj1 = y(j1,1,ind)
                   y(j1,1,ind) = y(j1,1,ind) + di
                   call diffun (n, t-h*(dble(1+ieq-ind)), y(1,1,ind),
     +               saved(n11p1,1))
                   nfe = nfe + 1
                   do 550 i=1,n
                      rj(i+j0) = (saved(n11+i,1) - saved(n12+i,1)) / di
  550                 continue
                   j0 = j0 + n0
                   y(j1,1,ind) = yj1
  560              continue
             else
c               --- evaluate the jacobian directly
                call pederv(n, t-h*(dble(1+ieq-ind)), y(1,1,ind),rj,n0)
                nje=nje+1
             endif
          endif
c
          if (iweval .ge. 1) then
             d = d1 * h
             do 640 i=1, nsq
                rw(i) = rj(i) * d
  640           continue
             j1 = 1
             do 660 i=1,n
                rw(j1) = 1.0 + rw(j1)
                j1 = j1 + n0 + 1
  660           continue
             call dec (n, n0, rw, ipiv, ier)
             ninvs = ninvs + 1
             iweval = -ieq
             rc = 1.0
             pdold = pd(nq)
c            --- do we have problems with the matrix being singular?
             if (ier .ne. 0) go to 800
          endif
c
  680   continue
        do 700 i=1,n
           saved(n9+i,1) = dy(i,ist) - h * saved(n9+i,1)
  700      continue
        call sol (n, n0, rw, saved(n9p1,1), ipiv)
        d2 = 0.0
        j3 = n9
        do 760 i=1,n
           j3 = j3 + 1
           j2 = j3 + n0
           saved(j2,1) = saved(j2,1) + saved(j3,1)
           y(i,1,ist) = y(i,1,ist) + d1*saved(j3,1)
           dy(i,ist) = dy(i,ist) - saved(j3,1)
           d2 = d2 + (saved(j3,1) / ymax(i))**2
  760      continue
        if (j .ne. 1) crate = max(crate*9.0d-1, d2/d3)
        if (d2* min(1.0d0, 2.0d0*crate) .le. bnd) go to 940
        d3 = d2
  780   continue
c
c     --- If we reach this point then ...
c     Corrector failed to converge in three iterations.
c     If jacobian was reevaluated during this cycle, step-size is
c     reduced to 3/10 of h. otherwise, jacobian is reevaluated
c
        tdl = told
        if (iweval .eq. 0) then
           do 920 i=1,n
              d = saved(n10+i,1)
              y(i,1,ist) = y(i,1,ist) - d1 * d
              dy(i,ist) = dy(i,ist) + d
  920         continue
           iweval = +1
c          --- now reapply the corrector
           go to 500
        endif
c
c
  800   continue
        if (abs(h) .le. (1.00001d0* abs(hmin))) then
           if (nq .lt. 2) then
c             --- We have tried the lowest order method and have found
c             that the corrector tolerance could not be obtained
c             with abs(h) > abs(hmin)   --- bail out ...
              kflag = -6
              j1 = nqst + 1
              do 802 i=1,n
                 dy(i,1) = saved(9,i)
                 do 801 j=1,j1
                    y(i,j,1) = saved(j,i)
  801               continue
  802            continue
              h = hold
              t = told
              jstart = nqst
              return
           elseif (nq .eq. 2) then
c             --- start over with order one method
              ifail = 0
              do 803 i=1,n
                 y(i,1,1) = saved(1,i)
  803            continue
              t = told
              h = h * max(1.0d-1, abs(hmin/h))
              iweval = +2
              go to 30
           else
              nq = 2
              ifail = 0
              iret = 2
              iweval = +2
              go to 3000
           endif
        endif
        ratio = ratio * 0.3d0
        iret = 1
        isw1 = 0
        isw2=1
        iweval = +2
        go to 3000
c
c     --- corrector converged. the backward differences of order
c         one through nq at the new mesh point are computed
c
  940   continue
c
c       --- the (nq+1)-st backward difference for all but the first
c           mesh point is established
c
        do 980 i=1,n
           do 960 j=1,nq
              y(i,j+1,ist) = y(i,j,ist) - y(i,j,ieq)
  960         continue
           if (ieq .ne. 1) then
              saved(n9+i,1) = y(i,nqp1,ist) - y(i,nqp1,ieq)
           endif
  980      continue
c
        if (ieq .ne. neq) then
          if ((nq.gt.2) .and. ((ieq.gt.1).or.(nq.eq.6))) indcc=indcc+1
          if(ieq .eq. 1) go to 1000
        end if
c
c     error test for all but the first mesh point is performed
c
        d = 0.
        do 1020 i=1,n
          d = d + (saved(n9+i,1)/ymax(i))**2
 1020     continue
c
        if(d.gt.e) then
c          --- the error criterion was not met
           ifail = ifail + 1
           if (ifail .le. 2) then
              if (abs(h) .le. (abs(hmin)*1.00001d0)) then
                 if (nq .lt. 2) then
c                   --- We can't do much better -- just accept the points
c                   computed with the smallest step-size --- bail out ...
                    iweval = 0
                    nstep = nstep + ieq
                    kflag = -ieq
                    return
                 elseif (nq .eq. 2) then
c                   --- drop order to one and try again
                    nq = 1
                    ifail = 0
                    iret = 2
                    iweval = +2
                    goto 3000
                 else
c                   --- drop the high order method to order 2
c                   and try again
                    nq = 2
                    ifail = 0
                    iret = 2
                    iweval = +2
                    goto 3000
                 endif
              endif
              iweval = +2
              if (ifail .eq. 1) go to 1200
              tdl = t
              ratio = ratio * 5.0d-1
              iret = 1
              isw1 = 0
              isw2 = 1
              go to 3000
           endif
c
c          --- start over with order 1 method
           ifail = 0
           do 1090 i=1,n
              y(i,1,1) = saved(1,i)
 1090         continue
           t = told
           h = h*max(1.0d-1, abs(hmin/h))
           iweval = +2
           go to 30
        endif
c
 1000   continue
c
      iweval = 0
      e = es
      kflag = neq
      nstep = nstep + kflag
      hnew = h
c
c     --- check for continuation with the same h and nq
c
      if (isw2 .eq. 1) then
         do 1002 i=1,n
            d = ymax(i)
            do 1001 j=1,neq
               d = max(d, abs(y(i,1,j)))
 1001          continue
            ymax(i) = d
 1002       continue
         hnext = hnew
         knext = 3 + nq / 5
         return
      endif
      if (nq .gt. 3) isw1 = 1 - isw1
      if (isw1 .eq. 1) then
         do 1004 i=1,n
            d = ymax(i)
            do 1003 j=1,neq
               d = max(d, abs(y(i,1,j)))
 1003          continue
            ymax(i) = d
 1004       continue
         hnext = hnew
         knext = 3 + nq / 5
         return
      endif
c
c     --- new step-size and/or order selection
c
 1200 continue
      rrsame = 1.2d0 * (d/e)**enqsam
      if (ifail .ne. 0) then
         ratio = ratio / rrsame
         iret = 1
         isw1 = 0
         isw2 = 1
         go to 3000
      endif
      rmax = 1.0d-4
      df = (dble(neq+nqm1))
      if (nq .ne. 1) rmax = (q - 1.0) / df
      rrsame = max(rrsame, rmax)
      rrup = 1.0d20
      rrdown = 1.0d20
      if (nq .lt. maxord) then
         d = 0.0
         do 1220 i=1,n
            d1 = y(i,nqp1,neq) - y(i,nqp1,neq-1)
            d = d + ((saved(n9+i,1) - d1)/ymax(i))**2
 1220       continue
         rrup = 1.2d0 * (d/eup)**enqup
         rmax = q / df
         rrup = max(rrup, rmax)
      endif
c
      if (nq .ne. 1) then
         d = 0.0
         do 1260 i=1,n
            d = d + (y(i,nqp1,1)/ymax(i))**2
 1260       continue
         rrdown = 1.2d0 * (d/edown)**enqdwn
         rmax = 1.0d-4
         if (nq .ne. 2) rmax = (q - 2.0d0) / df
         rrdown = max(rrdown, rmax)
      endif
c
      if (rrsame .gt. rrup) then
         if (rrup .lt. rrdown) then
            newq = nqp1
            d = 1.0 / rrup
         else
            newq = nqm1
            d = 1.0 / rrdown
         endif
      elseif (rrsame .le. rrdown) then
         newq = nq
         d = 1.0 / rrsame
      else
         newq = nqm1
         d = 1.0 / rrdown
      endif
c
      if (d .gt. 1.1d0) then
         hnew = h * d
         nq = newq
      endif
c
      do 1460 i=1,n
         d=ymax(i)
         do 1440 j=1,neq
            d = max(d, abs(y(i,1,j)))
 1440       continue
         ymax(i) = d
 1460    continue
      hnext = hnew
      knext = 3 + nq / 5
      return
c
c------------------- effective end of routine --------------------------
c
c
c     The following section is used when step-size or order is changed
c     during the cycle.  starting values are retrieved from the
c     saved array.
c     When jumping to this section of code the return flag indicates ...
c     iret = 1 : go back and compute more mesh points
c            2 : start a new cycle of order nq
c            3 : reset the saved array and start a new cycle of order nq
c
 3000 continue
      ratio =  max(abs(hmin/hold), min(ratio, 1.0d0))
      t = told
      if (ratio .ge. 1.0) then
         do 3021 i=1,n
            dy(i,1) = saved(9,i)
            do 3020 j=1,nq
               y(i,j,1) = saved(j,i)
 3020          continue
 3021       continue
         if (iret .eq. 1) goto 320
         if (iret .ge. 2) goto 260
c        go to (320,260), iret
c
      elseif (idel .le. 0) then
c        the (nqst+1)-st order backward difference is established
         idel = nqst + 1
         if ((nqst .ge. 2) .and. (nq .ge. 2)) then
            d = (dble(nqst))
            do 3060 i=1,n
               d1 = saved(9,i)
               do 3040 j=2,nqst
                  d1 = d1 - saved(j,i) / (dble(j-1))
 3040             continue
               saved(idel,i) = d * d1
 3060          continue
         endif
      endif
c
c     interpolate for new points
c
      do 3140 i=1,n
         dy(i,1) = ratio * saved(9,i)
         if (nq .ge. 2) then
            d1 = 0.0
            do 3120 j=2,nq
               d1 = d1 + ratio
               d2 = 1.0
               d = 0.0
               do 3100 j1=2,idel
                  d2 = d2 * ((dble(j1-2)) - d1) / (dble(j1-1))
                  d = d + d2 * saved(j1,i)
 3100             continue
               y(i,j,1) = d + saved(1,i)
 3120          continue
            y(i,1,1) = saved(1,i)
         endif
 3140    continue
      h = hold * ratio
c
c     --- form the backward differences
c
 4000 continue
      if (nq .ge. 2) then
         nqm1 = nq-1
         do 4022 i=1,n
            do 4021 j=1,nqm1
               j0 = j + 1
               do 4020 j1=j0,nq
                  j2 = nq - j1 + j + 1
                  y(i,j2,1) = y(i,j2-1,1) - y(i,j2,1)
 4020             continue
 4021          continue
 4022       continue
      endif
c
      if (iret .eq. 1) goto 320
      if (iret .eq. 2) goto 260
      if (iret .ge. 3) goto 180
c     go to (320,260,180), iret
c
      end
c
c-------------------- auxiliary routines follow ------------------------



      subroutine dec (n, ndim, a, ip, ier)
c
      integer n,ndim,ip,ier,nm1,k,kp1,m,i,j
      double precision a, t
      dimension a(ndim,n), ip(n)
c
c  matrix triangularization by gaussian elimination.
c  input..
c     n = order of matrix.
c     ndim = declared dimension of array  a .
c     a = matrix to be triangularized.
c  output..
c     a(i,j), i.le.j = upper triangular factor, u .
c     a(i,j), i.gt.j = multipliers = lower triangular factor, i - l.
c     ip(k), k.lt.n = index of k-th pivot row.
c     ip(n) = (-1)**(number of interchanges) or o .
c     ier = 0 if matrix a is nonsingular, or k if found to be
c           singular at stage k.
c  use  sol  to obtain solution of linear system.
c  determ(a) = ip(n)*a(1,1)*a(2,2)*...*a(n,n).
c  if ip(n)=o, a is singular, sol will divide by zero.
c
c  reference..
c     c. b. moler, algorithm 423, linear equation solver,
c     c.a.c.m. 15 (1972), p. 274.
c
      ier = 0
      ip(n) = 1
      if (n .eq. 1) go to 70
c
      nm1 = n - 1
      do 60 k = 1,nm1
        kp1 = k + 1
        m = k
        do 10 i = kp1,n
          if ( abs(a(i,k)) .gt.  abs(a(m,k))) m = i
   10     continue
        ip(k) = m
        t = a(m,k)
        if (m .ne. k) then
           ip(n) = -ip(n)
           a(m,k) = a(k,k)
           a(k,k) = t
        endif
c
        if (t .eq. 0.) go to 80
c
        t = 1.0 / t
        do 30 i = kp1,n
          a(i,k) = -a(i,k)*t
   30     continue
c
        do 50 j = kp1,n
          t = a(m,j)
          a(m,j) = a(k,j)
          a(k,j) = t
          if (t .ne. 0.) then
            do 40 i = kp1,n
              a(i,j) = a(i,j) + a(i,k)*t
   40         continue
          endif
   50     continue
   60   continue

   70 k = n
      if (a(n,n) .ne. 0.) return
c
   80 ier = k
      ip(n) = 0
      return
      end



      subroutine sol (n, ndim, a, b, ip)
c
      integer n,ndim,ip,nm1,k,kp1,m,i,kb,km1
      double precision a,b,t
      dimension a(ndim,n), b(n), ip(n)
c
c  solution of linear system, a*x = b .
c  input..
c    n = order of matrix.
c    ndim = declared dimension of array  a .
c    a = triangularized matrix obtained from dec.
c    b = right hand side vector.
c    ip = pivot vector obtained from dec.
c  do not use if dec has set ier .ne. 0.
c  output..
c    b = solution vector, x .
c
      if (n .eq. 1) go to 50
c
      nm1 = n - 1
      do 20 k = 1,nm1
        kp1 = k + 1
        m = ip(k)
        t = b(m)
        b(m) = b(k)
        b(k) = t
        do 10 i = kp1,n
          b(i) = b(i) + a(i,k)*t
   10     continue
   20   continue
c
      do 40 kb = 1,nm1
        km1 = n - kb
        k = km1 + 1
        b(k) = b(k)/a(k,k)
        t = -b(k)
        do 30 i = 1,km1
          b(i) = b(i) + a(i,k)*t
   30     continue
   40   continue
c
   50 b(1) = b(1)/a(1,1)
c
      return
      end

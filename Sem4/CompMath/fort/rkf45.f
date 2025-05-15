      function r8_epsilon ( )

c*********************************************************************72
c
cc r8_epsilon() returns the R8 roundoff unit.
c
c  Discussion:
c
c    The roundoff unit is a number R which is a power of 2 with the
c    property that, to the precision of the computer's arithmetic,
c      1 .lt. 1 + R
c    but
c      1 = ( 1 + R / 2 )
c
c    Fortran90 provides the superior library routine
c
c      EPSILON ( X )
c
c  Licensing:
c
c    This code is distributed under the MIT license.
c
c  Modified:
c
c    06 March 2006
c
c  Author:
c
c    John Burkardt
c
c  Parameters:
c
c    Output, double precision R8_EPSILON, the R8 roundoff unit.
c
      implicit none

      double precision r8
      double precision r8_epsilon
      double precision r8_test

      r8 = 1.0D+00
      r8_test = 1.0D+00 + ( r8 / 2.0D+00 )

10    continue

      if ( 1.0D+00 .lt. r8_test ) then
        r8 = r8 / 2.0D+00
        r8_test = 1.0D+00 + ( r8 / 2.0D+00 )
        go to 10
      end if

      r8_epsilon = r8

      return
      end
      subroutine r8_rkf45 ( f, neqn, y, t, tout, relerr, abserr, 
     &  iflag, work, iwork )

c*********************************************************************72
c
cc r8_rkf45() is an interface to the fehlberg fourth-fifth order runge-kutta method
c
c     written by h.a.watts and l.f.shampine
c                   sandia laboratories
c                  albuquerque,new mexico
c
c    rkf45 is primarily designed to solve non-stiff and mildly stiff
c    differential equations when derivative evaluations are inexpensive.
c    rkf45 should generally not be used when the user is demanding
c    high accuracy.
c
c abstract
c
c    subroutine  rkf45  integrates a system of neqn first order
c    ordinary differential equations of the form
c             dy(i)/dt = f(t,y(1),y(2),...,y(neqn))
c              where the y(i) are given at t .
c    typically the subroutine is used to integrate from t to tout but it
c    can be used as a one-step integrator to advance the solution a
c    single step in the direction of tout.  on return the parameters in
c    the call list are set for continuing the integration. the user has
c    only to call rkf45 again (and perhaps define a new value for tout).
c    actually, rkf45 is an interfacing routine which calls subroutine
c    rkfs for the solution.  rkfs in turn calls subroutine  fehl which
c    computes an approximate solution over one step.
c
c    rkf45  uses the runge-kutta-fehlberg (4,5)  method described
c    in the reference
c    e.fehlberg , low-order classical runge-kutta formulas with stepsize
c                 control , nasa tr r-315
c
c    the performance of rkf45 is illustrated in the reference
c    l.f.shampine,h.a.watts,s.davenport, solving non-stiff ordinary
c                 differential equations-the state of the art ,
c                 sandia laboratories report sand75-0182 ,
c                 to appear in siam review.
c
c
c    the parameters represent-
c      f -- subroutine f(t,y,yp) to evaluate derivatives yp(i)=dy(i)/dt
c      neqn -- number of equations to be integrated
c      y(*) -- solution vector at t
c      t -- independent variable
c      tout -- output point at which solution is desired
c      relerr,abserr -- relative and absolute error tolerances for local
c            error test. at each step the code requires that
c                 abs(local error) .le. relerr*abs(y) + abserr
c            for each component of the local error and solution vectors
c      iflag -- indicator for status of integration
c      work(*) -- array to hold information internal to rkf45 which is
c            necessary for subsequent calls. must be dimensioned
c            at least  3+6*neqn
c      iwork(*) -- integer array used to hold information internal to
c            rkf45 which is necessary for subsequent calls. must be
c            dimensioned at least  5
c
c
c  first call to rkf45
c
c    the user must provide storage in his calling program for the arrays
c    in the call list  -      y(neqn) , work(3+6*neqn) , iwork(5)  ,
c    declare f in an external statement, supply subroutine f(t,y,yp) and
c    initialize the following parameters-
c
c      neqn -- number of equations to be integrated.  (neqn .ge. 1)
c      y(*) -- vector of initial conditions
c      t -- starting point of integration , must be a variable
c      tout -- output point at which solution is desired.
c            t=tout is allowed on the first call only, in which case
c            rkf45 returns with iflag=2 if continuation is possible.
c      relerr,abserr -- relative and absolute local error tolerances
c            which must be non-negative. relerr must be a variable while
c            abserr may be a constant. the code should normally not be
c            used with relative error control smaller than about 1.e-8 .
c            to avoid limiting precision difficulties the code requires
c            relerr to be larger than an internally computed relative
c            error parameter which is machine dependent. in particular,
c            pure absolute error is not permitted. if a smaller than
c            allowable value of relerr is attempted, rkf45 increases
c            relerr appropriately and returns control to the user before
c            continuing the integration.
c      iflag -- +1,-1  indicator to initialize the code for each new
c            problem. normal input is +1. the user should set iflag=-1
c            only when one-step integrator control is essential. in this
c            case, rkf45 attempts to advance the solution a single step
c            in the direction of tout each time it is called. since this
c            mode of operation results in extra computing overhead, it
c            should be avoided unless needed.
c
c
c  output from rkf45
c
c      y(*) -- solution at t
c      t -- last point reached in integration.
c      iflag = 2 -- integration reached tout. indicates successful retur
c                   and is the normal mode for continuing integration.
c            =-2 -- a single successful step in the direction of tout
c                   has been taken. normal mode for continuing
c                   integration one step at a time.
c            = 3 -- integration was not completed because relative error
c                   tolerance was too small. relerr has been increased
c                   appropriately for continuing.
c            = 4 -- integration was not completed because more than
c                   3000 derivative evaluations were needed. this
c                   is approximately 500 steps.
c            = 5 -- integration was not completed because solution
c                   vanished making a pure relative error test
c                   impossible. must use non-zero abserr to continue.
c                   using the one-step integration mode for one step
c                   is a good way to proceed.
c            = 6 -- integration was not completed because requested
c                   accuracy could not be achieved using smallest
c                   allowable stepsize. user must increase the error
c                   tolerance before continued integration can be
c                   attempted.
c            = 7 -- it is likely that rkf45 is inefficient for solving
c                   this problem. too much output is restricting the
c                   natural stepsize choice. use the one-step integrator
c                   mode.
c            = 8 -- invalid input parameters
c                   this indicator occurs if any of the following is
c                   satisfied -   neqn .le. 0
c                                 t=tout  and  iflag .ne. +1 or -1
c                                 relerr or abserr .lt. 0.
c                                 iflag .eq. 0  or  .lt. -2  or  .gt. 8
c      work(*),iwork(*) -- information which is usually of no interest
c                   to the user but necessary for subsequent calls.
c                   work(1),...,work(neqn) contain the first derivatives
c                   of the solution vector y at t. work(neqn+1) contains
c                   the stepsize h to be attempted on the next step.
c                   iwork(1) contains the derivative evaluation counter.
c
c
c  subsequent calls to rkf45
c
c    subroutine rkf45 returns with all information needed to continue
c    the integration. if the integration reached tout, the user need onl
c    define a new tout and call rkf45 again. in the one-step integrator
c    mode (iflag=-2) the user must keep in mind that each step taken is
c    in the direction of the current tout. upon reaching tout (indicated
c    by changing iflag to 2),the user must then define a new tout and
c    reset iflag to -2 to continue in the one-step integrator mode.
c
c    if the integration was not completed but the user still wants to
c    continue (iflag=3,4 cases), he just calls rkf45 again. with iflag=3
c    the relerr parameter has been adjusted appropriately for continuing
c    the integration. in the case of iflag=4 the function counter will
c    be reset to 0 and another 3000 function evaluations are allowed.
c
c    however,in the case iflag=5, the user must first alter the error
c    criterion to use a positive value of abserr before integration can
c    proceed. if he does not,execution is terminated.
c
c    also,in the case iflag=6, it is necessary for the user to reset
c    iflag to 2 (or -2 when the one-step integration mode is being used)
c    as well as increasing either abserr,relerr or both before the
c    integration can be continued. if this is not done, execution will
c    be terminated. the occurrence of iflag=6 indicates a trouble spot
c    (solution is changing rapidly,singularity may be present) and it
c    often is inadvisable to continue.
c
c    if iflag=7 is encountered, the user should use the one-step
c    integration mode with the stepsize determined by the code or
c    consider switching to the adams codes de/step,intrp. if the user
c    insists upon continuing the integration with rkf45, he must reset
c    iflag to 2 before calling rkf45 again. otherwise,execution will be
c    terminated.
c
c    if iflag=8 is obtained, integration can not be continued unless
c    the invalid input parameters are corrected.
c
c    it should be noted that the arrays work,iwork contain information
c    required for subsequent integration. accordingly, work and iwork
c    should not be altered.
c
c
      integer neqn,iflag,iwork(5)
      double precision y(neqn),t,tout,relerr,abserr,work(1)

      external f

      integer k1,k2,k3,k4,k5,k6,k1m
c
c     compute indices for the splitting of the work array
c
      k1m=neqn+1
      k1=k1m+1
      k2=k1+neqn
      k3=k2+neqn
      k4=k3+neqn
      k5=k4+neqn
      k6=k5+neqn
c
c     this interfacing routine merely relieves the user of a long
c     calling list via the splitting apart of two working storage
c     arrays. if this is not compatible with the users compiler,
c     he must use rkfs directly.
c
      call r8_rkfs(f,neqn,y,t,tout,relerr,abserr,iflag,work(1),
     &  work(k1m),
     1          work(k1),work(k2),work(k3),work(k4),work(k5),work(k6),
     2          work(k6+1),iwork(1),iwork(2),iwork(3),iwork(4),iwork(5))

      return
      end
      subroutine r8_rkfs(f,neqn,y,t,tout,relerr,abserr,iflag,yp,h,
     &  f1,f2,f3,
     1                f4,f5,savre,savae,nfe,kop,init,jflag,kflag)

c*********************************************************************72
c
cc r8_rkfs() carries out the Fehlberg fourth-fifth order Runge-Kutta method.
c
c
c     rkfs integrates a system of first order ordinary differential
c     equations as described in the comments for rkf45 .
c     the arrays yp,f1,f2,f3,f4,and f5 (of dimension at least neqn) and
c     the variables h,savre,savae,nfe,kop,init,jflag,and kflag are used
c     internally by the code and appear in the call list to eliminate
c     local retention of variables between calls. accordingly, they
c     should not be altered. items of possible interest are
c         yp - derivative of solution vector at t
c         h  - an appropriate stepsize to be used for the next step
c         nfe- counter on the number of derivative function evaluations
c
c
      logical hfaild,output

      integer  neqn,iflag,nfe,kop,init,jflag,kflag
      double precision  y(neqn),t,tout,relerr,abserr,h,yp(neqn),
     1  f1(neqn),f2(neqn),f3(neqn),f4(neqn),f5(neqn),savre,
     2  savae
      double precision r8_epsilon

      external f

      double precision  a,ae,dt,ee,eeoet,esttol,et,hmin,remin,rer,s,
     1  scale,tol,toln,twoeps,u26,ypk

      integer  k,maxnfe,mflag

      double precision  dabs,dmax1,dmin1,dsign

      save
c
c  remin is the minimum acceptable value of relerr.  attempts
c  to obtain higher accuracy with this subroutine are usually
c  very expensive and often unsuccessful.
c
      data remin/1.d-12/
c
c     the expense is controlled by restricting the number
c     of function evaluations to be approximately maxnfe.
c     as set, this corresponds to about 500 steps.
c
      data maxnfe/3000/
c
c   here two constants emboding the machine epsilon is present
c   twoesp is set to twice the machine epsilon while u26 is set
c   to 26 times the machine epsilon
c
      twoeps = 2.0 * r8_epsilon ( )

      u26 = 13.*twoeps
c
c  Check input parameters
c
      if ( neqn < 1 ) then
        iflag = 8
        write ( *, '(a)' ) ' '
        write ( *, '(a)' ) 'r8_rkfs(): Fatal error!'
        write ( *, '(a)' ) '  neqn < 1.'
        return
      end if

      if ( relerr < 0.0 ) then
        iflag = 8
        write ( *, '(a)' ) ' '
        write ( *, '(a)' ) 'r8_rkfs(): Fatal error!'
        write ( *, '(a)' ) '  relerr < 0.0.'
        return
      end if

      if ( abswerr < 0.0 ) then
        iflag = 8
        write ( *, '(a)' ) ' '
        write ( *, '(a)' ) 'r8_rkfs(): Fatal error!'
        write ( *, '(a)' ) '  abserr < 0.0.'
        return
      end if

      mflag = iabs ( iflag )

      if ( mflag < 1 .or. 8 < mflag ) then
        iflag = 8
        write ( *, '(a)' ) ' '
        write ( *, '(a)' ) 'r8_rkfs(): Fatal error!'
        write ( *, '(a)' ) '  1 <= mflag <= 8 fails.'
        return
      end if
c
c  Is this the first call?
c
      if (mflag .eq. 1) go to 50
c
c  check continuation possibilities
c
      if ((t .eq. tout) .and. (kflag .ne. 3)) then
        iflag = 8
        write ( *, '(a)' ) ' '
        write ( *, '(a)' ) 'r8_rkfs(): Fatal error!'
        write ( *, '(a)' ) '  t == tout and kflag /= 3.'
        return
      end if

      if (mflag .ne. 2) go to 25
c
c  iflag = +2 or -2
c
      if (kflag .eq. 3) go to 45
      if (init .eq. 0) go to 45
      if (kflag .eq. 4) go to 40
      if ((kflag .eq. 5)  .and.  (abserr .eq. 0.0d0)) go to 30
      if ((kflag .eq. 6)  .and.  (relerr .le. savre)  .and.
     1    (abserr .le. savae)) go to 30
      go to 50
c
c  iflag = 3,4,5,6,7 or 8
c
   25 if (iflag .eq. 3) go to 45
      if (iflag .eq. 4) go to 40
      if ((iflag .eq. 5) .and. (abserr .gt. 0.0d0)) go to 45
c
c  integration cannot be continued since user did not respond to
c  the instructions pertaining to iflag=5,6,7 or 8
c
   30  continue
        write ( *, '(a)' ) ' '
        write ( *, '(a)' ) 'R8_RKFS - Fatal error!'
        write ( *, '(a)' ) '  Integration cannot be continued.'
        write ( *, '(a)' ) '  The user did not respond to the output'
        write ( *, '(a)' ) '  value FLAG = 5, 6, 7, or 8.'
       stop
c
c  reset function evaluation counter
c
   40 nfe=0
      if (mflag .eq. 2) go to 50
c
c  reset flag value from previous call.
c
   45 iflag=jflag
      if (kflag .eq. 3) mflag=iabs(iflag)
c
c  save input iflag and set continuation flag value for subsequent
c  input checking
c
   50 jflag=iflag
      kflag=0
c
c  save relerr and abserr for checking input on subsequent calls
c
      savre=relerr
      savae=abserr
c
c  restrict relative error tolerance to be at least as large as
c  2*eps+remin to avoid limiting precision difficulties arising
c  from impossible accuracy requests
c
      rer=twoeps+remin
      if (relerr .ge. rer) go to 55
c
c  relative error tolerance too small
c
      relerr=rer
      iflag=3
      kflag=3
      return

   55 dt=tout-t

      if (mflag .eq. 1) go to 60
      if (init .eq. 0) go to 65
      go to 80
c
c  initialization --
c  set initialization completion indicator,init
c  set indicator for too many output points,kop
c  evaluate initial derivatives
c  set counter for function evaluations,nfe
c  evaluate initial derivatives
c  set counter for function evaluations,nfe
c  estimate starting stepsize
c
   60 init=0
      kop=0

      a=t
      call f(a,y,yp)
      nfe=1
      if (t .ne. tout) go to 65
      iflag=2
      return

   65 init=1
      h=dabs(dt)
      toln=0.
      do k=1,neqn
        tol=relerr*dabs(y(k))+abserr
        if (tol .le. 0.) go to 70
        toln=tol
        ypk=dabs(yp(k))
        if (ypk*h**5 .gt. tol) h=(tol/ypk)**0.2d0
   70   continue
      end do
      if (toln .le. 0.0d0) h=0.0d0
      h=dmax1(h,u26*dmax1(dabs(t),dabs(dt)))
      jflag=isign(2,iflag)
c
c  set stepsize for integration in the direction from t to tout
c
   80 h=dsign(h,dt)
c
c  test to see if rkf45 is being severely impacted by too many
c  output points
c
      if (dabs(h) .ge. 2.0d0*dabs(dt)) kop=kop+1
      if (kop .ne. 100) go to 85
c
c  unnecessary frequency of output
c
      kop=0
      iflag=7
      return

   85 if (dabs(dt) .gt. u26*dabs(t)) go to 95
c
c  if too close to output point,extrapolate and return
c
      do k=1,neqn
        y(k)=y(k)+dt*yp(k)
      end do
      a=tout
      call f(a,y,yp)
      nfe=nfe+1
      go to 300
c
c  initialize output point indicator
c
   95 output= .false.
c
c  to avoid premature underflow in the error tolerance function,
c  scale the error tolerances
c
      scale=2.0d0/relerr
      ae=scale*abserr
c
c  step by step integration
c
  100 hfaild= .false.
c
c  set smallest allowable stepsize
c
      hmin=u26*dabs(t)
c
c     adjust stepsize if necessary to hit the output point.
c     look ahead two steps to avoid drastic changes in the stepsize and
c     thus lessen the impact of output points on the code.
c
      dt=tout-t
      if (dabs(dt) .ge. 2.0d0*dabs(h)) go to 200
      if (dabs(dt) .gt. dabs(h)) go to 150
c
c  the next successful step will complete the integration to the
c  output point
c
      output= .true.
      h=dt
      go to 200

  150 h=0.5d0*dt
c
c     core integrator for taking a single step
c
c     the tolerances have been scaled to avoid premature underflow in
c     computing the error tolerance function et.
c     to avoid problems with zero crossings,relative error is measured
c     using the average of the magnitudes of the solution at the
c     beginning and end of a step.
c     the error estimate formula has been grouped to control loss of
c     significance.
c     to distinguish the various arguments, h is not permitted
c     to become smaller than 26 units of roundoff in t.
c     practical limits on the change in the stepsize are enforced to
c     smooth the stepsize selection process and to avoid excessive
c     chattering on problems having discontinuities.
c     to prevent unnecessary failures, the code uses 9/10 the stepsize
c     it estimates will succeed.
c     after a step failure, the stepsize is not allowed to increase for
c     the next attempted step. this makes the code more efficient on
c     problems having discontinuities and more effective in general
c     since local extrapolation is being used and extra caution seems
c     warranted.
c
c
c     test number of derivative function evaluations.
c     if okay,try to advance the integration from t to t+h
c
  200 if (nfe .le. maxnfe) go to 220
c
c     too much work
      iflag=4
      kflag=4
      return
c
c     advance an approximate solution over one step of length h
c
  220 call r8_fehl(f,neqn,y,t,h,yp,f1,f2,f3,f4,f5,f1)
      nfe=nfe+5
c
c     compute and test allowable tolerances versus local error estimates
c     and remove scaling of tolerances. note that relative error is
c     measured with respect to the average of the magnitudes of the
c     solution at the beginning and end of the step.
c
      eeoet=0.0d0
      do k=1,neqn
        et=dabs(y(k))+dabs(f1(k))+ae
        if (et .gt. 0.0d0) go to 240
c
c       inappropriate error tolerance
        iflag=5
        return
c
  240   ee=dabs((-2090.0d0*yp(k)+(21970.0d0*f3(k)-15048.0d0*f4(k)))+
     1                        (22528.0d0*f2(k)-27360.0d0*f5(k)))
        eeoet=dmax1(eeoet,ee/et)
      end do
c
      esttol=dabs(h)*eeoet*scale/752400.0d0
c
      if (esttol .le. 1.0d0) go to 260
c
c
c     unsuccessful step
c                       reduce the stepsize , try again
c                       the decrease is limited to a factor of 1/10
c
      hfaild= .true.
      output= .false.
      s=0.1d0
      if (esttol .lt. 59049.0d0) s=0.9d0/esttol**0.2d0
      h=s*h
      if (dabs(h) .gt. hmin) go to 200
c
c     requested error unattainable at smallest allowable stepsize
      iflag=6
      kflag=6
      return
c
c
c     successful step
c                        store solution at t+h
c                        and evaluate derivatives there
c
  260 t=t+h
      do k=1,neqn
        y(k)=f1(k)
      end do
      a=t
      call f(a,y,yp)
      nfe=nfe+1
c
c
c                       choose next stepsize
c                       the increase is limited to a factor of 5
c                       if step failure has just occurred, next
c                          stepsize is not allowed to increase
c
      s=5.0d0
      if (esttol .gt. 1.889568d-4) s=0.9d0/esttol**0.2d0
      if (hfaild) s=dmin1(s,1.0d0)
      h=dsign(dmax1(s*dabs(h),hmin),h)
c
c     end of core integrator
c
c
c     should we take another step
c
      if (output) go to 300
      if (iflag .gt. 0) go to 100
c
c     integration successfully completed
c
c     one-step mode
      iflag=-2
      return
c
c     interval mode
  300 t=tout
      iflag=2
      return
      end
      subroutine r8_fehl ( f, neqn, y, t, h, yp, f1, f2, f3, f4, f5, s )

c*********************************************************************72
c
cc r8_fehl() fehlberg fourth-fifth order runge-kutta method
c
c    fehl integrates a system of neqn first order
c    ordinary differential equations of the form
c             dy(i)/dt=f(t,y(1),---,y(neqn))
c    where the initial values y(i) and the initial derivatives
c    yp(i) are specified at the starting point t. fehl advances
c    the solution over the fixed step h and returns
c    the fifth order (sixth order accurate locally) solution
c    approximation at t+h in array s(i).
c    f1,---,f5 are arrays of dimension neqn which are needed
c    for internal storage.
c    the formulas have been grouped to control loss of significance.
c    fehl should be called with an h not smaller than 13 units of
c    roundoff in t so that the various independent arguments can be
c    distinguished.
c
      integer neqn

      double precision  y(neqn),t,h,yp(neqn),f1(neqn),f2(neqn),
     1  f3(neqn),f4(neqn),f5(neqn),s(neqn)

      double precision  ch
      integer  k

      ch=h/4.0d0
      do k=1,neqn
        f5(k)=y(k)+ch*yp(k)
      end do
      call f(t+ch,f5,f1)

      ch=3.0d0*h/32.0d0
      do k=1,neqn
        f5(k)=y(k)+ch*(yp(k)+3.0d0*f1(k))
      end do
      call f(t+3.0d0*h/8.0d0,f5,f2)

      ch=h/2197.0d0
      do k=1,neqn
        f5(k)=y(k)+ch*(1932.0d0*yp(k)+(7296.0d0*f2(k)-7200.0d0*f1(k)))
      end do
      call f(t+12.0d0*h/13.0d0,f5,f3)

      ch=h/4104.0d0
      do k=1,neqn
        f5(k)=y(k)+ch*((8341.0d0*yp(k)-845.0d0*f3(k))+
     1                            (29440.0d0*f2(k)-32832.0d0*f1(k)))
      end do
      call f(t+h,f5,f4)

      ch=h/20520.0d0
      do k=1,neqn
        f1(k)=y(k)+ch*((-6080.0d0*yp(k)+(9295.0d0*f3(k)-
     1         5643.0d0*f4(k)))+(41040.0d0*f1(k)-28352.0d0*f2(k)))
      end do
      call f(t+h/2.0d0,f1,f5)
c
c     compute approximate solution at t+h
c
      ch = h / 7618050.0d0
      do k = 1, neqn
        s(k)=y(k)+ch*((902880.0d0*yp(k)+(3855735.0d0*f3(k)-
     &        1371249.0d0*f4(k)))+(3953664.0d0*f2(k)+
     &        277020.0d0*f5(k)))
      end do

      return
      end


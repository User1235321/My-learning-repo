c     random01.for
c     Random number routines.
c     ------------------------------------------------------
c
c     P. A. Jacobs
c     Department of Mechanical Engineering
c     University of Queensland
c
c     Revisions...
c     13-Feb-98 : translated back from C
c     17-Feb-98 : changed function name to flip_coin
c
c ----------------------------------------------------------

      function random01 (restart)
c        implicit undefined (a-z)
         integer restart
         real*8  random01

c        Purpose...
c        -------
c        Provide uniformly distributed random numbers between 0.0 and 1.0.
c
c        Input...
c        -----
c        restart  : Set negative to initialize or restart the sequence.
c                   The value is also used as part of the seed on the
c                   first call.
c
c        Output...
c        ------
c        random01() returns a double value as the next random number.
c
c        Reference...
c        ---------
c        Based on the translated code ran3() in
c        Press et al. Numerical Recipes in C (1989), Cambridge.
c        In turn, they used code from
c        D. E. Knuth Seminumerical Algorithms (1981), Addison-Wesley

c        Have used double-precision storage to maintain consistency
c        with the C version of the code.

         real*8    MBIG, MSEED, MZ, FAC
         parameter (MBIG = 1000000000)
         parameter (MSEED = 161803398)
         parameter (MZ = 0)
         parameter (FAC = 1.0 / MBIG)

c        The following quantities need to be remembered between calls.
c        iff indicates whether the routine has been called before.

         integer inext, inextp
         real*8  ma(55)
         integer iff

c        The following data are temporary

         real*8  mj, mk
         integer i, ii, k

         common /rand1/ inext, inextp, ma, iff
         data iff/0/

c        Initialization

         if (restart .lt. 0 .or. iff .eq. 0) then

c           Leave our mark so that we know that we have initialised
            iff = 1

c           Initialize ma(55) using the seed restart and the large
c           number MSEED.
            mj = MSEED - abs(restart)
            mj = mod(mj, MBIG)
            ma(55) = mj

c           Now, initialize the rest of the table, in a slightly
c           random order, but with numbers that are not especially
c           random.
   
            mk = 1
            do i = 1, 54
               ii = mod(21 * i, 55)
               ma(ii) = mk
               mk = mj - mk
               if (mk .lt. MZ) mk = mk + MBIG
               mj = ma(ii)
c              write (*,*) 'ma(', ii, ') = ', ma(ii)
            end do

c           Randomize the numbers.

            do k = 1, 4
               do i = 1, 55
                  ma(i) = ma(i) - ma(1 + mod((i+30), 55))
                  if (ma(i) .lt. MZ) ma(i) = ma(i) + MBIG
               end do
            end do

c           Prepare indices for the first random number.
c           Note that the value 31 is special.
    
            inext = 0
            inextp = 31
            restart = 1

c           End of initialization
         endif


c        ----------
c        Start here, except for initialization or restarting.
c        ----------

c        increment the indices, with wraparound

         inext = inext + 1
         if (inext .eq. 56) inext = 1
         inextp = inextp + 1
         if (inextp .eq. 56) inextp = 1

c        Generate a new random number subtractively, 
c        make sure it is in range and then store it.

         mj = ma(inext) - ma(inextp)
         if (mj .lt. MZ) mj = mj + MBIG
         ma(inext) = mj

c        Return the derived uniform deviate as a double value
c        between 0.0 and 1.0.

         random01 = mj * FAC
         return
      end

c --------------------------------------------------------------

      function flip_coin (p)
c        implicit undefined (a-z)
         real*8  p
         integer flip_coin

c        Purpose...
c        -------
c        Biased coin toss.
c
c        Input...
c        -----
c        p    : probability of returning a 1
c
c        Output...
c        ------
c        Return a 1 with probability p
c                 0 with probability (1.0 - p)

         real*8  rn
         real*8  random01
         integer restart

         if (p .ge. 1.0) then
            flip_coin = 1
            return
         endif

c        Call the portable random number generator.

         restart = 1
         rn = random01 (restart)

c        Return the result of the coin toss.

         if (rn .le. p) then
            flip_coin = 1
         else
            flip_coin = 0
         endif
         return

      end

c ------ end of file random01.for ------------


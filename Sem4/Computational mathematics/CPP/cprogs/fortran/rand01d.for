c     test_r01.for
c     Test the random number generator.

      program test_r01

c     implicit undefined (a-z)
      integer  seed, i, n
      real*8   rnd
      real*8   random01
      external random01

      n = 10
      seed = 3
      rnd = random01(seed)
      write (*,*) 'seed = ', seed

      do i = 0, n-1
         write (*,*) 'i = ', i, '; x = ', random01(seed)
      end do

      end

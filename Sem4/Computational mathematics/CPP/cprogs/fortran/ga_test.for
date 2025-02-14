c     test_ga.for
c     Test driver for ga_optimize() : Genetic Algorithm Optimizer.
c     ----------------------------------------------------------

      program test_ga
c        implicit undefined (a-z)

c        Data definitions etc are contained in the header file
         include 'ga_opt.fh'

         external objfunc1, objfunc2
         real*8   objfunc1, objfunc2

         record  /ga_data/ gad
         real*8  fbest, xbest(10)
         integer jx

c        go to 10

         write (*,*) 'Test 1 for ga_optimize()...'

c        Set parameters for the GA

c        size of populations
         gad.popsize = 100
c        number of parameters
         gad.nx      = 1
c        bits in a parameter
         gad.xbits   = 30
c        number of generations
         gad.maxgen  = 50
c        stop if no improvement in spin generations
         gad.spin    = 5

c        probability of crossover
         gad.pcross    = 0.6
c        probability of mutation
         gad.pmutation = 1.0 / gad.popsize
c        preserve best string
         gad.elitist   = 1

c        Seed for random numbers
         gad.iseed     = 1

c        =1: write reports 
         gad.report_flag = 0

         write(*,*) 'Call GA optimizer...'
         call ga_optimize (objfunc1, gad, xbest, fbest)

         write(*,*) 'return_flag = ', gad.return_flag
         if (gad.return_flag .ge. 0) then
            write(*,*) '------- Result --------'
            write(*,*) 'Objective = ', fbest
            do jx = 1, gad.nx
               write(*,*) 'x[', jx, '] = ', xbest(jx)
            end do
         else
            write(*,*) 'Error return'
         endif

 10      continue
         write(*,*)
         write(*,*) 'Test 2 for ga_optimize()...'

c        Set parameters for the GA

c        size of populations
         gad.popsize = 50
c        number of parameters
         gad.nx      = 2
c        bits in a parameter
         gad.xbits   = 22
c        number of generations
         gad.maxgen  = 8000 / gad.popsize
c        stop if no improvement in spin generations
         gad.spin    = 30

c        probability of crossover
         gad.pcross    = 0.6d0
c        probability of mutation
         gad.pmutation = 2.0d0 / gad.popsize
c        preserve best string
         gad.elitist   = 1

c        Seed for random numbers
         gad.iseed     = -1

c        =1: write reports 
         gad.report_flag = 0

         write(*,*) 'Call GA optimizer...'
         call ga_optimize (objfunc2, gad, xbest, fbest)

         write(*,*) 'return_flag = ', gad.return_flag
         if (gad.return_flag .ge. 0) then
            write(*,*) '------- Result --------'
            write(*,*) 'Objective = ', fbest
            do jx = 1, gad.nx
               write(*,*) 'x[', jx, '] = ', xbest(jx)
            end do
         else
            write(*,*) 'Error return'
         endif

      end

c ---------------------------------------------------------------

      real*8 function objfunc1 (n, x)
c        implicit undefined (a-z)
         integer n
         real*8  x(*)
c 
c        Purpose...
c        -------
c        Given the n parameter values, compute the objective
c        (or raw fitness) function.
c 
c        Input...
c        -----
c        n    : number of parameters
c        x(*) : array of parameters x(i), 0 < i <= n
c               Note that the optimizer only searches 
c               the range 0.0 <= x < 1.0
c
c        Output...
c        ------
c        objfun returns a double value
c 
         include 'ga_opt.fh'
         real*8  value

c        value = x(1) * x(1) * x(1) - 1.0d0

         real*8 xx, yy, temp, temp2

         yy = (x(1) - 0.5d0) * 200.0d0
         xx = (0.5d0 - 0.5d0) * 200.0d0

         temp = xx * xx + yy * yy
         temp2 = sqrt(temp)
         temp2 = sin(temp2)

         value = 0.5d0 + (temp2 * temp2 - 0.5d0) / 
     &           (1.0d0 + 0.001d0 * temp * temp)

c        The original paper searched for a minimum.
c        We are searching for the maximum.
         value = -value

         if (GA_DEBUG .ge. 4) then
            print *, 'objfunc1: n = ', n, ', x = ', x(1)
            print *,'objfunc1: value = ', value
         endif

c        Remember to set the return value...
         objfunc1 = value
         return
      end

c ---------------------------------------------------------------

      real*8 function objfunc2 (n, x)
c        implicit undefined (a-z)
         integer n
         real*8  x(*)
c 
c        Purpose...
c        -------
c        Given the n parameter values, compute the objective
c        (or raw fitness) function.
c 
c        Input...
c        -----
c        n    : number of parameters
c        x(*) : array of parameters x(i), 0 < i <= n
c               Note that the optimizer only searches 
c               the range 0.0 <= x < 1.0
c
c        Output...
c        ------
c        objfun returns a double value
c 
         include 'ga_opt.fh'

c        This objective function is labelled F6 in the paper
c        J. D. Schaffer, R. A. Caruana, L. J. Eshelman and R. Das
c        A study of control parameters affecting online performance
c        of genetic algorithms for function optimization.
c        Proceedings of the Third International Conference on 
c        Genetic Algorithms, Arlington, VA, 1989.

         real*8  value
         real*8 xx, yy, temp, temp2

         xx = (x(1) - 0.5d0) * 200.0d0
         yy = (x(2) - 0.5d0) * 200.0d0

         temp = xx * xx + yy * yy
         temp2 = sqrt(temp)
         temp2 = sin(temp2)

         value = 0.5d0 + (temp2 * temp2 - 0.5d0) / 
     &           (1.0d0 + 0.001d0 * temp * temp)

c        The original paper searched for a minimum.
c        We are searching for the maximum.
         value = -value

         if (GA_DEBUG .ge. 4) then
            print *, 'objfunc2: n = ', n, ', x(1),x(2) = ', x(1), x(2)
            print *,'objfunc2: value = ', value
         endif

c        Remember to set the return value...
         objfunc2 = value
         return
      end


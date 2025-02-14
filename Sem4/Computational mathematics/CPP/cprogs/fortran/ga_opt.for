c     ga_opt.for
c     Genetic Algorithm Optimizer.
c     --------------------------------------------------------
c 
c     Purpose...
c     -------
c     Given a user defined objective function and a set of encoded
c     parameters, use a Genetic Algorithm to optimize the values of
c     the parameters to give the highest value of the objective function.
c 
c     Written by...
c     ----------
c     P. A. Jacobs
c     Department of Mechanical Engineering
c     University of Queensland
c     Email: peterj@mech.uq.edu.au
c 
c     Version...
c     -------
c     1.00   : 21-Feb-93 : Inital C coding
c     2.00   : 15-Feb-98 : FORTRAN adaption
c 
c     References...
c     ----------
c     This minimizer is largely based on the description of the
c     "Simple Genetic Algorithm" given in
c 
c     D. E. Goldberg (1989)
c     Genetic Algorithms in Search, Optimization, and Machine Learning.
c     Addison-Wesley Publishing Company, Reading, MA
c 
c     ---------------------------------------------------------
c  

      subroutine ga_optimize (objfunc, gad, xbest, fbest)

c        implicit undefined (a-z)
         include 'ga_opt.fh'

         real*8   objfunc
         external objfunc
         record   /ga_data/ gad
         real*8   xbest(*)
         real*8   fbest

c        Purpose...
c        -------
c        The principle function providing the interface
c        to the Genetic Algorithm.
c 
c        Input...
c        -----
c        objfunc : pointer to the user supplied objective function
c        gad     : the global GA data
c 
c        Output...
c        ------
c        xbest(*) : the array of parameter values of the best individual
c        fbest    : the best objective value.
c
c        ga_optimize records the return status in gad.return_flag
c            0 for a normal return
c           -1 for an error return
c            1 for stalled search
 
         integer  jx, jid, jbit
         integer  itemp, gen_count
         real*8   temp, current_best
         real*8   xtemp(MAXPAR)
         external flip_coin
         integer  flip_coin
         external random01
         real*8   random01

c        ----------
c        Initialize...
c        ----------
         gad.return_flag = 0

c        length of chromosome
         gad.lchrom = gad.xbits * gad.nx

c        Scale to bring keep 0.0 <= x < 1.0
         gad.xscale = 2.0d0**(gad.xbits)

         if (GA_DEBUG .ge. 1) then
            write(*,*) 'ga_optimize: Genetic Algorithm Optimizer.'
            write(*,*) 'Initial Parameters...'
            write(*,*) 'Population size   = ', gad.popsize
            write(*,*) 'Phenotype length  = ', gad.nx
            write(*,*) 'Bits per parameter= ', gad.xbits
            write(*,*) 'Chromosome length = ', gad.lchrom
            write(*,*) 'Max generations   = ', gad.maxgen
            write(*,*) 'Crossover prob.   = ', gad.pcross
            write(*,*) 'Mutation prob.    = ', gad.pmutation
         endif

c        Some checking of inputs...

         if (GA_DEBUG .ge. 2) then
            write(*,*) 'ga_optimize: Check inputs'
         endif

         itemp = gad.popsize / 2
         if (itemp * 2 .ne. gad.popsize) then
            write(*,*) 'ga_optimize: Incorrect population size', 
     &         gad.popsize
            gad.return_flag = -1
            return
         endif

         if (gad.nx .gt. MAXPAR) then
            write(*,*) 'ga_optimize: Too many parameters, ', 
     &         gad.nx 
            gad.return_flag = -1
            return
         endif

         if (gad.popsize .gt. MAXPOP) then
            write(*,*) 'ga_optimize: Population too large, ', 
     &         gad.popsize 
            gad.return_flag = -1
            return
         endif

         if (gad.lchrom .gt. MAXSTRING) then
            write(*,*) 'ga_optimize: Chromosome too long, ', 
     &         gad.lchrom 
            gad.return_flag = -1
            return
         endif

c        Standard Random-number generator
c        srand(gad.iseed)

         temp = random01 ( gad.iseed )

c        Counters

         gad.gen       = 0
         gad.nmutation = 0
         gad.ncross    = 0

         gad.spin_counter = 0

c        Initialize the population with random values

         if (GA_DEBUG .ge. 2) then
            write(*,*) 'ga_optimize: Initialize population.'
         endif
         do jid = 1, gad.popsize
            do jbit = 1, gad.lchrom
c              Set each bit with a fair coin toss.
               gad.oldpop(jid).chrom(jbit) = flip_coin(0.5d0)
            end do

            call decode_chromosome( gad, gad.oldpop(jid) )
            do jx = 1, gad.nx
               xtemp(jx) = gad.oldpop(jid).x(jx)
            end do
            gad.oldpop(jid).objective = objfunc( gad.nx, xtemp )

            gad.oldpop(jid).parent1 = 0
            gad.oldpop(jid).parent2 = 0
            if (GA_DEBUG .ge. 4) then
               write(*,*) 'jid = ', jid
               call print_chromosome(gad, gad.oldpop(jid) )
            endif
         end do

         if (GA_DEBUG .ge. 2) then
            write(*,*) 'ga_optimize:',
     &        ' Compute initial population statistics'
         endif
         call ga_scale_fitness( gad, gad.oldpop )
         call ga_statistics( gad, gad.oldpop )

         if (GA_DEBUG .ge. 1) then
            write(*,*) 'Initial Population...'
            write(*,*) 'Maximum fitness  = ', gad.max
            write(*,*) 'Average fitness  = ', gad.avg
            write(*,*) 'Minimum fitness  = ', gad.min
            write(*,*) 'Sum of fitness   = ', gad.sumfitness
         endif

c        Record the phenotype of the best from the initial generation.

         call ga_copy_individual( gad, gad.oldpop(gad.best),
     &                                 gad.ultimate )

c        -----------------
c        The Main GA loop.
c        -----------------

         do gen_count = 1, gad.maxgen

            gad.gen = gad.gen + 1

            if (GA_DEBUG .ge. 2) then
               write(*,*) 'ga_optimize: Compute generation ', gad.gen
            endif
            call ga_generation( objfunc, gad )

            call ga_apply_elitist( gad, gad.newpop )

            call ga_scale_fitness( gad, gad.newpop )
            call ga_statistics( gad, gad.newpop )

            call ga_report( gad )

c           Copy the new generation over the old one.
            do jid = 1, gad.popsize
               call ga_copy_individual( gad, gad.newpop(jid),
     &                                  gad.oldpop(jid) )
            end do

c           Check the best result from the latest generation.
c           If it is the best ever then record its phenotype.

            gad.spin_counter = gad.spin_counter + 1
            current_best = gad.oldpop(gad.best).objective
            if ( current_best .gt. gad.ultimate.objective ) then
               call ga_copy_individual (gad, gad.oldpop(gad.best),
     &                                  gad.ultimate )
c              We are still improving so reset the counter
               gad.spin_counter = 0
            endif

            if (GA_DEBUG .ge. 2) then
               write(*,*) 'ga_optimize: End of generation.'
            endif

c           If we are not making progress then 
c           set the report flag to indicate a stalled optimization
c           and break the loop.

            if (gad.spin_counter .ge. gad.spin) then
               gad.return_flag = 1
               goto 10
            endif

         end do

c        If we have reached this point, we have cycled specified
c        number of generations without mishap.
         gad.return_flag = 0

 10      continue

c        Assume that the user is interested only in 
c        the parameter values of the the phenotype
c        (rather than the internal representation).

         fbest = gad.ultimate.objective
         do jx = 1, gad.nx
            xbest(jx) = gad.ultimate.x(jx)
         end do

         return
      end

c ---------------------------------------------------------

      subroutine ga_copy_individual (gad, original, target)

c        implicit undefined (a-z)
         include 'ga_opt.fh'

         record /ga_data/ gad
         record /individual/ original
         record /individual/ target

c        Purpose...
c        -------
c        Make a complete copy of an individual.
c 
c        Input...
c        -----
c        gad      : pointer to the global data structure
c        original : individual to copy
c 
c        Output...
c        ------
c        target   : the new individual

         integer jx, jbit

         if (GA_DEBUG .ge. 5) then
            write(*,*) 'ga_copy_individual: Begin.'
         endif

         target.fitness = original.fitness
         do jbit = 1, gad.lchrom
            target.chrom(jbit) = original.chrom(jbit)
         end do

         target.objective = original.objective
         do jx = 1, gad.nx
            target.x(jx) = original.x(jx)
         end do

         target.parent1   = original.parent1
         target.parent2   = original.parent2
         target.objective = original.objective

         if (GA_DEBUG .ge. 5) then
            write(*,*) 'ga_copy_individual: End.'
         endif

         return
      end

c ---------------------------------------------------------------

      subroutine decode_chromosome (gad, critter)

c        implicit undefined (a-z)
         include 'ga_opt.fh'

         record /ga_data/ gad
         record /individual/ critter

c        Purpose...
c        -------
c        Decode the binary chromosome's binary string to produce an
c        array of parameter values for the phenotype array.
c
c        Input...
c        -----
c        gad     : pointer to the global data structure
c        critter : pointer to a particular individual
c 
c        Output...
c        ------
c        The decoded parameter values are written to the parameter array
c        within the individual's data structure.

         integer jx, jbit
         real*8  powerof2, accum
         integer grey_bit, bit_index
         integer previous_bit, this_bit
         integer binary_string(MAXSTRING)

         if (GA_DEBUG .ge. 5) then
            write(*,*) 'decode_chromosome: Begin.'
         endif

         do jx = 1, gad.nx

c           Extract the substring of bits for one parameter
c           and decode from reflected grey code to binary

            previous_bit = 0
            do jbit = 1, gad.xbits
               bit_index = (jx-1) * gad.xbits + jbit
               grey_bit = critter.chrom(bit_index)

               if (grey_bit .eq. 1) then
                  if (previous_bit .eq. 0) then
                     this_bit = 1
                  else
                     this_bit = 0
                  endif
               else
                  this_bit = previous_bit
               endif

               previous_bit = this_bit
               binary_string(jbit) = this_bit
            end do

c           Convert the binary coding to a floating point value.

            powerof2 = 1.0d0
            accum = 0.0d0
            do jbit = 1, gad.xbits
               if (binary_string(jbit) .eq. 1) accum = accum + powerof2
               powerof2 = powerof2 * 2.0d0
            end do

c           Save the floating point value in the individual 
c           parameter list.
c           The scale ensures that 0.0 <= x < 1.0.

            critter.x(jx) = accum / gad.xscale
         end do

         if (GA_DEBUG .ge. 5) then
            write(*,*) 'decode_chromosome: End.'
         endif

         return
      end

c ---------------------------------------------------------------

      subroutine print_chromosome (gad, critter)

c        implicit undefined (a-z)
         include 'ga_opt.fh'

         record /ga_data/    gad
         record /individual/ critter

c        Purpose...
c        -------
c        Print a representation of the chromosome's binary string.
c
c        Input...
c        -----
c        gad     : pointer to the global data structure
c        critter : pointer to a particular individual

         integer jx, jbit
         integer bit_index, bit_value
         character*(MAXSTRING) chrom

         if (GA_DEBUG .ge. 5) then
            write(*,*) 'print_chromosome: Begin.'
         endif

         do jx = 1, gad.nx

c           Extract the substring of bits for one parameter

            do jbit = 1, gad.xbits
               bit_index = (jx-1) * gad.xbits + jbit
               bit_value = critter.chrom(bit_index)
               if (bit_value .eq. 1) then
                  chrom(bit_index:bit_index) = '1'
               else
                  chrom(bit_index:bit_index) = '0'
               endif
            end do

         end do

         write(*,*) '[', chrom, ']'
         do jx = 1, gad.nx
            write(*,*) 'x(', jx, ') = ', critter.x(jx)
         end do

         if (GA_DEBUG .ge. 5) then
            write(*,*) 'print_chromosome: End.'
         endif

         return
      end

c -----------------------------------------------------------

      subroutine ga_generation (objfunc, gad)

c        implicit undefined (a-z)
         include 'ga_opt.fh'

         real*8   objfunc
         external objfunc
         record   /ga_data/ gad

c        Purpose...
c        -------
c        Create a new generation through select, crossover and mutation.
c
c        Input...
c        -----
c        gad     : pointer to the global data structure.
c        objfunc : pointer to the objective function

         integer  mate1, mate2
         integer  child1, child2
         integer  ga_select
         external ga_select
         integer  jx
         real*8   xtemp(MAXPAR)

         if (GA_DEBUG .ge. 3) then
            write(*,*) 'ga_generation: Begin.'
         endif

c        Set up the choices array.
c        This is needed later by ga_select()
         call ga_preselect( gad, gad.oldpop )
         gad.nremain = gad.popsize

c        Repeat the process of selection, cross-over and mutation
c        until the new population is full.

         do child1 = 1, gad.popsize, 2

c           Create the new generation in pairs.
            child2 = child1 + 1

c           Select 2 parents.
            mate1 = ga_select( gad )
            mate2 = ga_select( gad )
            if (GA_DEBUG .ge. 4) then
               write(*,*) 'For children: ', child1, child2
               write(*,*) 'Selected parents are: ', mate1, mate2
            endif

c           Apply the Cross-over and mutation operator.
            call ga_cross_over ( gad, 
     &                           gad.oldpop(mate1), 
     &                           gad.oldpop(mate2),
     &                           gad.newpop(child1), 
     &                           gad.newpop(child2) )

            if (GA_DEBUG .ge. 3) then
               write(*,*) 'ga_generation: after ga_cross_over.'
               write(*,*) 'Parent 1:'
               call print_chromosome( gad, gad.oldpop(mate1) )
               write(*,*) 'Parent 2:'
               call print_chromosome( gad, gad.oldpop(mate2) )
            endif

c           Decode the chromosomes and evaluate the objective 
c           function for each child.

            call decode_chromosome ( gad, gad.newpop(child1) )
            do jx = 1, gad.nx
               xtemp(jx) = gad.newpop(child1).x(jx)
            end do
            gad.newpop(child1).objective = objfunc( gad.nx, xtemp )
            gad.newpop(child1).parent1 = mate1
            gad.newpop(child1).parent2 = mate2

            call decode_chromosome ( gad, gad.newpop(child2) )
            do jx = 1, gad.nx
               xtemp(jx) = gad.newpop(child2).x(jx)
            end do
            gad.newpop(child2).objective = objfunc( gad.nx, xtemp )
            gad.newpop(child2).parent1 = mate1
            gad.newpop(child2).parent2 = mate2

            if (GA_DEBUG .ge. 3) then
               write(*,*) 'ga_generation: after evaluating children.'
               write(*,*) 'child 1:'
               call print_chromosome( gad, gad.newpop(child1) )
               write(*,*) 'child 2:'
               call print_chromosome( gad, gad.newpop(child2) )
            endif

         end do

         if (GA_DEBUG .ge. 3) then
            write(*,*) 'ga_generation: End.'
         endif

         return
      end

c --------------------------------------------------------------

      subroutine ga_apply_elitist (gad, pop)

c        implicit undefined (a-z)
         include 'ga_opt.fh'

         record   /ga_data/    gad
         record   /individual/ pop(*)

c        Apply the elitist strategy by ensuring that the best-so-far
c        individual is present in the new population.
c
c        Input...
c        -----
c        gad   : pointer to the global data structure
c        pop   : the population we are checking.
c
c        Output...
c        ------
c        If anything is done, this routines writes directly to the
c        individual's data structures in pop.

         integer jbest, jworst, jid
         real*8  objbest, objworst, temp

         if (GA_DEBUG .ge. 3) then
            write(*,*) 'ga_apply_elitist: Begin.'
         endif

c        Search for the best and the worst objective functions in the
c        specified population.

         jbest    = 1
         jworst   = 1
         objbest  = pop(1).objective
         objworst = objbest

         do jid = 2, gad.popsize
            temp = pop(jid).objective
            if (temp .gt. objbest) then
               objbest = temp
               jbest   = jid
            endif
            if (temp .lt. objworst) then
               objworst = temp
               jworst   = jid
            endif
         end do

c        Apply the elitist strategy by copying the best-so-far into
c        the current population.

         if (gad.ultimate.objective .gt. objbest) then
            call ga_copy_individual (gad, gad.ultimate, pop(jworst) )
         endif

         if (GA_DEBUG .ge. 3) then
            write(*,*) 'ga_apply_elitist: End.'
         endif

         return
      end

c -------------------------------------------------------------

      subroutine ga_preselect (gad, pop)

c        implicit undefined (a-z)
         include 'ga_opt.fh'

         record   /ga_data/    gad
         record   /individual/ pop(*)

c        Purpose...
c        -------
c        Set up the array of mating choices for selection by the
c        stochastic remainder method.
c        Essentially, we are stacking the deck in favour of fitter
c        individuals.
c

         integer  j, k, winner, jassign
         real*8   fraction(MAXPOP), expected
         external flip_coin
         integer  flip_coin

         if (GA_DEBUG .ge. 4) then
            write(*,*) 'ga_preselect: Begin.'
         endif

c        Pass through the population once and 
c        assign (the integer part of) the expected number of copies.
c        This will ensure that the fitter individuals will be
c        more numerous in the parents pool.

         k = 1
         do j = 1, gad.popsize
            expected    = pop(j).fitness / gad.avg
            jassign     = int(expected)
            fraction(j) = expected - real(jassign)
            do while (jassign .gt. 0 .and. k .le. gad.popsize)
               gad.choices(k) = j
               k = k + 1
               jassign = jassign - 1
            end do
         end do

c        If necessary,
c        fill up the remainder of the choice array by stochastically
c        assigning individuals based on the fractional part of the
c        expected number of copies.
c
c        Don't fill up the last element this way.
c        Instead, set to the first individual.

         j = 1
         do while (k .le. (gad.popsize - 1))

c           Wrap around if necessary.
            if (j .gt. gad.popsize) j = 1

            if (fraction(j) .gt. 0.0d0) then
               winner = flip_coin( fraction(j) )
               if (winner .eq. 1) then
                  gad.choices(k) = j
                  fraction(j) = fraction(j) - 1.0d0
                  k = k + 1
               endif
            endif
            j = j + 1
         end do

         gad.choices(gad.popsize) = 1

         if (GA_DEBUG .ge. 4) then
            do k = 1, gad.popsize
               write(*,*) 'choice(', k, ') = ', gad.choices(k)
            end do
            write(*,*) 'ga_preselect: End.'
         endif

         return
      end

c -----------------------------------------------------------

      integer function ga_select (gad)

c        implicit undefined (a-z)
         include 'ga_opt.fh'

         record   /ga_data/    gad

c        Purpose...
c        -------
c        Select individuals via roulette wheel selection.
c        Selection is biased toward the fitter individuals
c        because the pool has been stacked with such individuals.
c        Subroutine ga_preselect arranged this.
c
c        Input...
c        -----
c        gad   : pointer to the global data structure
c
c        Output...
c        ------
c        ga_select() returns the index of the selected individual.

         integer  jid, jpick
         real*8   rnd, partsum
         real*8   random01
         external random01

         integer SELECT_OPTION
         parameter(SELECT_OPTION = 1)

         if (GA_DEBUG .ge. 4) then
            write(*,*) 'ga_select: Begin.'
         endif

         if (SELECT_OPTION .eq. 0) then

c           THIS SEEMS TO NEED FIXING
            write(*,*) 'Roulette-wheel selection'
            write(*,*) 'We should not reach this point.'
            write(*,*) 'Stopping.'
            stop

c           Roulette-wheel selection.
c
c           Initialize counter and partial sum.
            jid = 1
            partsum = gad.oldpop(jid).fitness

c           Compute a random point on the wheel.
            rnd = random01(gad.iseed)
            rnd = rnd * gad.sumfitness

c           Find the wheel slot.
            do while (partsum .lt. rnd)
               jid = jid + 1
               partsum = partsum + gad.oldpop(jid).fitness
            end do

         else

c           Stochastic remainder selection.
c           For early selections, we randomly pick from the whole
c           of the choices table.  Later, we restrict the choice
c           to the earlier part of the table where we are most likely
c           to find fitter individuals.
c           Remember that nremain needed to be initialised
c           before any selectionsare made using this function.

            rnd = random01(gad.iseed)
            rnd = rnd * real(gad.nremain)
            jpick = int(rnd) + 1
            if (jpick .gt. gad.nremain) jpick = gad.nremain

            jid = gad.choices(jpick)

            gad.nremain = gad.nremain - 1

            if (GA_DEBUG .ge. 5) then
               write(*,*) 'ga_select: rnd = ', rnd
               write(*,*) '           jpick = ', jpick
               write(*,*) '             jid = ', jid
               write(*,*) '         nremain = ', gad.nremain
            endif

         endif

         if (GA_DEBUG .ge. 4) then
            write(*,*) 'ga_select: End.'
         endif

         ga_select = jid
         return 
      end

c -----------------------------------------------------------

      subroutine ga_cross_over (gad, parent1, parent2, child1, child2)

c        implicit undefined (a-z)
         include 'ga_opt.fh'

         record   /ga_data/    gad
         record   /individual/ parent1
         record   /individual/ parent2
         record   /individual/ child1
         record   /individual/ child2

c        Purpose...
c        -------
c        Given two parents generate two children by crossing-over 
c        segments of their chromosomes.
c
c        Input...
c        -----
c        gad     : pointer to the global data structure
c        parent1 : pointer to parent 1
c        parent2 :
c        child1  : pointer to child 1
c        child2  :
c
c        Output...
c        ------
c        ga_cross_over() writes its results directly to the child data
c        structures.

         integer  jbit, jcross1, jcross2, jtemp
         integer  flip_result
         real*8   rnd1, rnd2, rtemp
         real*8   random01
         external random01
         external flip_coin, ga_mutation
         integer  flip_coin, ga_mutation

         if (GA_DEBUG .ge. 4) then
            write(*,*) 'ga_cross_over: Begin.'
         endif

         flip_result = flip_coin(gad.pcross)
         if ( flip_result .eq. 1 ) then

c           Do cross-over with probability pcross.

            gad.ncross = gad.ncross + 1

c           Select a cross-over site along the chromosome.

            rnd1 = random01(gad.iseed)
            jcross1 = int(rnd1 * real(gad.lchrom)) + 1
            if (jcross1 .gt. gad.lchrom) jcross1 = gad.lchrom

            rnd2 = random01(gad.iseed)
            jcross2 = int(rnd2 * real(gad.lchrom)) + 1
            if (jcross2 .gt. gad.lchrom) jcross2 = gad.lchrom

c           Ensure that jcross1 <= jcross2.
            if (jcross1 .gt. jcross2) then
               jtemp   = jcross1
               jcross1 = jcross2
               jcross2 = jtemp
               rtemp   = rnd1
               rnd1    = rnd2
               rnd2    = rtemp
            endif

            if (GA_DEBUG .ge. 4) then
               write(*,*) 'rnd1 = ', rnd1, ', rnd2 = ', rnd2
               write(*,*) 'jcross1 = ', jcross1, ', jcross2 = ', jcross2
            endif

         else

c           Set the cross-over points so that nothing happens.

            jcross1 = gad.lchrom
            jcross2 = jcross1
         endif


c        Direct copy first parts of chromosomes with possible mutation.

         do jbit = 1, jcross1
            child1.chrom(jbit) = ga_mutation( gad, parent1.chrom(jbit) )
            child2.chrom(jbit) = ga_mutation( gad, parent2.chrom(jbit) )
         end do

c        Do the cross-over with possible mutation.
c        If jcross1 == jcross2, nothing is done here.

         do jbit = jcross1 + 1, jcross2
            child1.chrom(jbit) = ga_mutation( gad, parent2.chrom(jbit) )
            child2.chrom(jbit) = ga_mutation( gad, parent1.chrom(jbit) )
         end do

c        Now, copy the remainder of the chromosome with possible mutation.
c        If jcross2 == lchrom, nothing is done here.

         do jbit = jcross2 + 1, gad.lchrom
            child1.chrom(jbit) = ga_mutation( gad, parent1.chrom(jbit) )
            child2.chrom(jbit) = ga_mutation( gad, parent2.chrom(jbit) )
         end do

         if (GA_DEBUG .ge. 5) then
            write(*,*) 'parent1'
            call print_chromosome( gad, parent1 )
            write(*,*) 'parent2'
            call print_chromosome( gad, parent2 )
            write(*,*) 'child1'
            call print_chromosome( gad, child1 )
            write(*,*) 'child2'
            call print_chromosome( gad, child2 )
            write(*,*) 'ga_cross_over: End.'
         endif

         return
      end

c -------------------------------------------------------

      integer function ga_mutation (gad, received_value)

c        implicit undefined (a-z)
         include 'ga_opt.fh'

         record  /ga_data/ gad
         integer received_value

c        Purpose...
c        -------
c        Randomly change a value (flip a bit) with probability
c        pmutation.
c        Remember that FORTRAN passes by reference so we must
c        protect the incoming value (otherwise the parents' 
c        chromosomes get buggered).
c
c        Input...
c        -----
c        gad             : pointer to the global data structure
c        received_value  : current value of the bit
c
c        Output...
c        ------
c        ga_mutation() returns the new value (be it changed or no)
c        The routine also updates the global mutation counter.

         integer  mutate, return_value
         external flip_coin
         integer  flip_coin

c        Default action is a direct copy.
         return_value = received_value

c        For a mutation, flip the bit.
         mutate = flip_coin(gad.pmutation)
         if (mutate .eq. 1) then
            gad.nmutation = gad.nmutation + 1
            if (received_value .eq. 1) then
               return_value = 0
            else
               return_value = 1
            endif
         endif

         ga_mutation = return_value
         return
      end

c ------------------------------------------------------------

      subroutine ga_scale_fitness (gad, pop)

c        implicit undefined (a-z)
         include 'ga_opt.fh'

         record  /ga_data/    gad
         record  /individual/ pop(*)

c        Purpose...
c        -------
c        Apply sigma-truncation followed by linear scaling to convert 
c        from raw fitness to a scaled fitness measure.  
c
c        Input...
c        -----
c        gad   : pointer to the global data structure
c        pop   : pointer to the population to be scaled
c
c        Output...
c        ------
c        The individual objective function values are mapped to scaled
c        fitness values and stored in the individual data structures.

         integer jid
         real*8  a, b, delta, temp
         real*8  umax, umin, uavg, uvar, sigma

         if (GA_DEBUG .ge. 2) then
            write(*,*) 'ga_scale_fitness: Begin'
            write(*,*) 'map objective function to fitness'
         endif

c        Fine the minimum value of the objective function.

         umin = pop(1).objective
         do jid = 2, gad.popsize
            temp = pop(jid).objective
            if (temp .lt. umin) umin = temp
         end do

         if (umin .lt. 0.0d0) then
c           Add an offset to ensure that all fitness
c           values are positive.
            temp = 0.01d0 - umin
            do jid = 1, gad.popsize
               pop(jid).fitness = pop(jid).objective + temp
            end do
         else
c           Simply copy the objective function values.
            do jid = 1, gad.popsize
               pop(jid).fitness = pop(jid).objective
            end do
         endif

         if (GA_DEBUG .ge. 3) then
            write(*,*) 'After offset...'
            write(*,*) 'jid, objective, fitness'
            do jid = 1, gad.popsize
               write(*,*) jid, pop(jid).objective, pop(jid).fitness
            end do
         endif

c        Apply sigma truncation.
c
c        First, compute raw fitness mean and variance.

         temp = pop(1).fitness
         uavg = temp
         do jid = 2, gad.popsize
            uavg = uavg + pop(jid).fitness
         end do
         uavg = uavg / real(gad.popsize)

c        Variance and standard deviation
         temp = pop(1).fitness - uavg
         uvar = temp * temp
         do jid = 2, gad.popsize
            temp = uavg - pop(jid).fitness
            uvar = uvar + temp * temp
         end do
         uvar = uvar / real(gad.popsize - 1)
         sigma = sqrt(uvar)

c        Apply sigma truncation.

         do jid = 1, gad.popsize
            temp = pop(jid).fitness
            temp = temp - 2.0d0 * sigma
            if (temp .le. 0.0d0) then
               pop(jid).fitness = 0.0d0
            else
               pop(jid).fitness= temp
            end if
         end do

         if (GA_DEBUG .ge. 3) then
            write(*,*) 'After sigma truncation...'
            write(*,*) 'jid, objective, fitness'
            do jid = 1, gad.popsize
               write(*,*) jid, pop(jid).objective, pop(jid).fitness
            end do
         endif

c        Stage 1 of linear scaling.
c        Compute mean, maximum and minimum.

         temp = pop(1).fitness
         uavg = temp
         umin = temp
         umax = temp
         do jid = 2, gad.popsize
            temp = pop(jid).fitness
            uavg = uavg + temp
            if (temp .gt. umax) umax = temp
            if (temp .lt. umin) umin = temp
         end do
         uavg = uavg / real(gad.popsize)

         if (GA_DEBUG .ge. 3) then
            write(*,*) 'ga_scale_fitness: linear-scaling parameters'
         endif

c        Now, compute the normal linear-scaling parameters
c        as per Goldberg (1989).
c
c        The parameters are selected such that the average scaled
c        fitness is equal to the average objective value and the
c        maximum scaled fitness is FMULTIPLE times the average
c        scaled fitness.

         delta = umax - umin
         if (delta .le. 0.0d0) then
            write(*,*) 'ga_scale_fitness: No variation in population.'
            stop
         endif

         a = uavg * (FMULTIPLE - 1.0d0) / delta
         b = (1.0d0 - a) * uavg

c        Check the scaled minimum value to see if the scaling
c        needs to be adjusted.

         if ( (a * umin + b) .lt. 0.0d0 ) then
c           Adjust the scaling parameters such that the minimum
c           scaled fitness is zero.
            a = uavg / delta
            b = -1.0d0 * a * umin
         endif

c        Now, do the linear scaling.

         if (GA_DEBUG .ge. 3) then
            write(*,*) 'ga_scale_fitness: linear-scaling proper'
         endif
         do jid = 1, gad.popsize
            pop(jid).fitness = a * pop(jid).fitness + b
         end do

         if (GA_DEBUG .ge. 3) then
            write(*,*) 'After linear scaling...'
            write(*,*) 'jid, objective, fitness'
            do jid = 1, gad.popsize
               write(*,*) jid, pop(jid).objective, pop(jid).fitness
            end do
         endif

         return
      end

c ----------------------------------------------------------------

      subroutine ga_statistics (gad, pop)

c        implicit undefined (a-z)
         include 'ga_opt.fh'

         record  /ga_data/    gad
         record  /individual/ pop(*)

c        Purpose...
c        -------
c        Calculate the population statistics of the given population.
c        These statistics are the minimum, maximum and average fitness of
c        the individuals within the population.
c
c        Input...
c        -----
c        gad   : pointer to the global data structure
c        pop   : pointer to the current population (array) of individuals
c
c        Output...
c        ------
c        Statistics are stored in the global data structure.

         integer jid
         real*8  temp

         if (GA_DEBUG .ge. 3) then
            write(*,*) 'ga_statistics: Begin.'
         endif

         temp = pop(1).fitness
         gad.best       = 1
         gad.sumfitness = temp
         gad.min        = temp
         gad.max        = temp

         do jid = 2, gad.popsize
            temp = pop(jid).fitness
            gad.sumfitness = gad.sumfitness + temp
            if (temp .gt. gad.max) then
               gad.max = temp
               gad.best = jid
            end if
            if (temp .lt. gad.min) gad.min = temp
         end do

         gad.avg = gad.sumfitness / real(gad.popsize)

         if (GA_DEBUG .ge. 3) then
            write(*,*) 'ga_statistics: End.'
         endif

         return
      end

c --------------------------------------------------------------

      subroutine ga_report (gad)

c        implicit undefined (a-z)
         include 'ga_opt.fh'

         record  /ga_data/    gad

c        Purpose...
c        -------
c        Print a report on the state of both the old and new populations.
c        Just what is reported is regulated by the GA_DEBUG macro defined
c        in ga_opt.fh.
c
c        Input...
c        -----
c        gad  : pointer to the global data structure
c        pop  : pointer to the current population (array) of individuals
c

         integer j, jid, jbest

         if (gad.report_flag .eq. 1 .or. GA_DEBUG .ge. 1) then
            write(*,*) '-------- Population Report for Generation ',
     &                  gad.gen, ' -------'
            write(*,*) 'Maximum Fitness = ', gad.max
            write(*,*) 'Minimum Fitness = ', gad.min
            write(*,*) 'Average Fitness = ', gad.avg
            write(*,*) 'Sum of Fitness  = ', gad.sumfitness

            jbest = gad.best
            write(*,*) 'Chromosome with Maximum fitness is ', jbest
            if (GA_DEBUG .ge. 3) then
               call print_chromosome( gad, gad.newpop(jbest) )
            endif
            write(*,*) 'Phenotype = '
            do j = 1, gad.nx
               write(*,*) gad.newpop(jbest).x(j)
            end do
            write(*,*) 'Objective = ', gad.newpop(jbest).objective
            write(*,*) 'Fitness   = ', gad.newpop(jbest).fitness
         endif

         if (GA_DEBUG .ge. 3) then
            write(*,*) 'Old population...'
            do jid = 1, gad.popsize
               call print_chromosome( gad, gad.oldpop(jid) )
            end do
            write(*,*) 'New population...'
            do jid = 1, gad.popsize
               call print_chromosome( gad, gad.newpop(jid) )
            end do
         endif

         return
      end

c ---------- end of file ga_opt.for ---------


/* ga_opt.c
 * Genetic Algorithm Optimizer.
 * ---------------------------------------------------------------
 *
 * Purpose...
 * -------
 * Given a user defined objective function and a set of encoded
 * parameters, use a Genetic Algorithm to optimize the values of
 * the parameters to give the highest value of the objective function.
 *
 * Written by...
 * ----------
 * P. A. Jacobs
 * Department of Mechanical Engineering
 * University of Queensland
 * Email: peterj@sun.mech.uq.oz.au
 *
 * Version...
 * -------
 * 1.00   : 21-Feb-93 : Inital coding
 * 1.01   : 22-Feb-93 : array of parameters
 * 1.02   : 23-Feb-93 : scaled fitness
 * 1.10   : 24-feb-93 : enforce positive fitness function
 *                      add objective function to the calling
 *                      arguments of ga_optimize()
 *                      remember the best individual ever
 *                      reflected-gray code added
 * 1.11   : 25-Feb-93 : Sigma truncation before linear scaling
 *                      elitist strategy added
 *                      stopping criteria added
 * 1.12   : 26-Feb-93 : Selection by stochasitc remainder method
 * 1.13   : 01-Mar-93 : two-point cross-over
 *                      add the seed to the global data structure
 * 1.20   : 03-Mar-93 : Moved random number routines to a new file.
 * 1.21   : 18-Feb-98 : minor changes to match the FORTRAN version
 *
 *
 * References...
 * ----------
 * This minimizer is largely based on the description of the
 * "Simple Genetic Algorithm" given in
 *
 * D. E. Goldberg (1989)
 * Genetic Algorithms in Search, Optimization, and Machine Learning.
 * Addison-Wesley Publishing Company, Reading, MA
 *
 * -------------------------------------------------------------------
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "ga_opt.h"
#include "random.h"

/*--------------------------------------------------------------------*/

int ga_optimize (double (*objfunc) (int n, double x[]),
                 struct ga_data *gad, 
                 double xbest[], double *fbest) {
   /*
    * Purpose...
    * -------
    * The principle function providing the interface
    * to the Genetic Algorithm.
    *
    * Input...
    * -----
    * *gad       : pointer to the global GA data
    * (*objfunc) : pointer to the user supplied objective function
    *
    * Output...
    * ------
    * xbest[] : points to the array of parameter values of the best
    *           individual
    * *fbest  : points to the best objective value.
    *
    */
   int    jx, jid, jbit;
   int    itemp;
   double temp, current_best;

   /*
    * ----------
    * Initialize...
    * ----------
    */

   gad->objfunc = objfunc;             /* Remember this function name */

   gad->lchrom = gad->xbits * gad->nx;        /* length of chromosome */

   /*
    * Scale to bring keep 0.0 <= x < 1.0
    * xscale = 2**xbits
    */
   temp = 1.0;
   for (jbit = 0; jbit < gad->xbits; ++jbit) temp *= 2.0;
   gad->xscale = temp;


#  if GA_DEBUG >= 1
      printf ("\nga_optimize: Genetic Algorithm Optimizer.\n");
      printf ("\nInitial Parameters...\n");
      printf ("Population size   = %d\n", gad->popsize);
      printf ("Phenotype length  = %d\n", gad->nx);
      printf ("Bits per parameter= %d\n", gad->xbits);
      printf ("Chromosome length = %d\n", gad->lchrom);
      printf ("Max generations   = %d\n", gad->maxgen);
      printf ("Crossover prob.   = %f\n", gad->pcross);
      printf ("Mutation prob.    = %f\n", gad->pmutation);
#  endif

   /*
    * Some checking of inputs...
    */
#  if GA_DEBUG >= 2
      printf ("ga_optimize: Check inputs\n");
#  endif

   itemp = gad->popsize / 2;
   if (itemp * 2 != gad->popsize) {
      printf ("ga_optimize: Incorrect population size, %d\n",
              gad->popsize);
      printf ("Press <RETURN>..."); getchar();
      exit (-1);
   } /* end if */

   if (gad->nx > MAXPAR) {
      printf ("ga_optimize: Too many parameters, %d\n", gad->nx);
      printf ("Press <RETURN>..."); getchar();
      exit (-1);
   } /* end if */

   if (gad->popsize > MAXPOP) {
      printf ("ga_optimize: Population too large, %d\n", gad->popsize);
      printf ("Press <RETURN>..."); getchar();
      exit (-1);
   } /* end if */

   if (gad->lchrom > MAXSTRING) {
      printf ("ga_optimize: Chromosome too long, %d\n", gad->lchrom);
      printf ("Press <RETURN>..."); getchar();
      exit (-1);
   } /* end if */

   /*
    * Allocate memory for the populations.
    */
#  if GA_DEBUG >= 2
      printf ("ga_optimize: Allocate memory\n");
#  endif

   gad->oldpop = (struct individual *)
                 malloc (MAXPOP * sizeof(struct individual));
   if (gad->oldpop == NULL) {
      printf ("gamin : could not allocate oldpop memory\n");
      printf ("Press <RETURN>..."); getchar();
      exit (-1);
   } /* end if */
   gad->newpop = (struct individual *)
                 malloc (MAXPOP * sizeof(struct individual));
   if (gad->newpop == NULL) {
      printf ("gamin : could not allocate newpop memory\n");
      printf ("Press <RETURN>..."); getchar();
      exit (-1);
   } /* end if */

   /*
    * Standard Random-number generator
    * srand ( gad->iseed );
    */
   temp = random01 ( &(gad->iseed) );

   /*
    * Counters
    */
   gad->gen       = 0;
   gad->nmutation = 0;
   gad->ncross    = 0;

   gad->spin_counter = 0;

   /*
    * Initialize the population at random
    */
#  if GA_DEBUG >= 2
      printf ("ga_optimize: Initialize population.\n");
#  endif
   for (jid = 0; jid < gad->popsize; ++jid) {
      for (jbit = 0; jbit < gad->lchrom; ++jbit) {
         /* Set each bit with a fair coin toss. */
         gad->oldpop[jid].chrom[jbit] = flip(0.5);
      } /* end for */

      decode_chromosome ( gad, &(gad->oldpop[jid]) );
      gad->oldpop[jid].objective =
         (*(gad->objfunc)) ( gad->nx, gad->oldpop[jid].x );

      gad->oldpop[jid].parent1 = 0;
      gad->oldpop[jid].parent2 = 0;

#     if GA_DEBUG >= 4
         printf("jid=%d, ", jid);
         print_chromosome(gad, &(gad->oldpop[jid]) );
#     endif
   }  /* end for (jid... */

#  if GA_DEBUG >= 2
      printf ("ga_optimize: Compute initial population statistics.\n");
#  endif
   ga_scale_fitness (gad, gad->oldpop);
   ga_statistics (gad, gad->oldpop);

#  if GA_DEBUG >= 1
      printf ("\nInitial Population...\n");
      printf ("Maximum fitness  = %f\n", gad->max);
      printf ("Average fitness  = %f\n", gad->avg);
      printf ("Minimum fitness  = %f\n", gad->min);
      printf ("Sum of fitness   = %f\n", gad->sumfitness);
#  endif

   /*
    * Record the phenotype of the best from the initial generation.
    */
   ga_copy_individual (gad, &(gad->oldpop[gad->best]),
                       &(gad->ultimate) );


   /*
    * -----------------
    * The Main GA loop.
    * -----------------
    */
   do {
      ++(gad->gen);

#     if GA_DEBUG >= 2
         printf ("ga_optimize: Compute generation %d.\n", gad->gen);
#     endif
      ga_generation (gad);

      ga_apply_elitist (gad, gad->newpop);

      ga_scale_fitness (gad, gad->newpop);
      ga_statistics (gad, gad->newpop);

      ga_report (gad);

      /*
       * Advance the generation by swapping their pointers.
       */
      gad->temp_pop = gad->oldpop;
      gad->oldpop   = gad->newpop;
      gad->newpop   = gad->temp_pop;

      /*
       * Check the best result from the latest generation.
       * If it is the best ever then record its phenotype.
       */
      ++(gad->spin_counter);
      current_best = gad->oldpop[gad->best].objective;
      if ( current_best > gad->ultimate.objective ) {
         ga_copy_individual (gad, &(gad->oldpop[gad->best]),
                             &(gad->ultimate) );
         gad->spin_counter = 0;    /* We are still improving. */
      } /* end if */

#     if GA_DEBUG >= 2
         printf ("ga_optimize: End of generation.\n");
#     endif

   } while (gad->gen < gad->maxgen &&
            gad->spin_counter < gad->spin);

   /*
    * The user is probably interested in the parameter values
    * of the the phenotype
    */
   *fbest = gad->ultimate.objective;
   for (jx = 0; jx < gad->nx; ++jx) xbest[jx] = gad->ultimate.x[jx];

   /*
    * Clean up and return...
    */
   free (gad->newpop);
   free (gad->oldpop);

   return 0;
}  /* end of ga_optimize() */

/*--------------------------------------------------------------------*/

int ga_copy_individual (struct ga_data *gad,
                        struct individual *original,
                        struct individual *target) {
   /*
    * Purpose...
    * -------
    * Make a complete copy of an individual.
    *
    * Input...
    * -----
    * *gad      : pointer to the global data structure
    * *original : individual to copy
    *
    * Output...
    * ------
    * *target   : the new individual
    *
    */
   int jx, jbit;

   target->fitness = original->fitness;
   for (jbit = 0; jbit < gad->lchrom; ++jbit)
      target->chrom[jbit] = original->chrom[jbit];

   target->objective = original->objective;
   for (jx = 0; jx < gad->nx; ++jx) 
      target->x[jx] = original->x[jx];

   target->parent1   = original->parent1;
   target->parent2   = original->parent2;
   target->objective = original->objective;

   return 0;
}  /* end of ga_copy_individual() */

/*--------------------------------------------------------------------*/

int decode_chromosome (struct ga_data *gad, 
                       struct individual *critter) {
   /*
    * Purpose...
    * -------
    * Decode the binary chromosome's binary string to produce an
    * array of parameter values for the phenotype array.
    *
    * Input...
    * -----
    * *gad     : pointer to the global data structure
    * *critter : pointer to a particular individual
    *
    * Output...
    * ------
    * The decoded parameter values are written to the parameter array
    * within the individual's data structure.
    *
    */
   int    jx, jbit;
   double powerof2, accum;
   int    gray_bit;
   int    previous_bit, binary_string[MAXSTRING];

   for (jx = 0; jx < gad->nx; ++jx) {
      /*
       * Extract the substring of bits for one parameter
       * and decode from reflected gray code to binary
       *
       * For a good story on Gray codes...
       * Carla Savage (1997) A survey of combinatorial Gray codes.
       * SIAM Review Vol 39 No 4 pp 605--629.
       */
      previous_bit = 0;
      for (jbit = 0; jbit < gad->xbits; ++jbit) {
         gray_bit = critter->chrom[jx * gad->xbits + jbit];

         if (gray_bit == 1)
            binary_string[jbit] = !previous_bit;
         else
            binary_string[jbit] = previous_bit;

         previous_bit = binary_string[jbit];
      } /* end for jbit */

      /*
       * Convert the binary coding to a floating point value.
       */
      powerof2 = 1.0;
      accum = 0.0;
      for (jbit = 0; jbit < gad->xbits; ++jbit) {
         if (binary_string[jbit] == 1) accum += powerof2;
         powerof2 *= 2.0;
      } /* end for jbit */

      /*
       * Save the floating point value in the individual 
       * parameter list.
       * The scale ensures that 0.0 <= x < 1.0.
       */
      critter->x[jx] = accum / gad->xscale;
   }  /* end for (jx... */

   return 0;
} /* end function decode_chromosome() */

/*--------------------------------------------------------------------*/

int print_chromosome (struct ga_data *gad, struct individual *critter) {
   /*
    * Purpose...
    * -------
    * Print a string representation of the individual.
    *
    * Input...
    * -----
    * *gad     : pointer to the global data structure
    * *critter : pointer to a particular individual
    *
    */
   int    jx, jbit;

   for (jbit = 0; jbit < gad->lchrom; ++jbit) {
      printf ( "%1d", critter->chrom[jbit] );
   } /* end for */
   printf (", ");

   for (jx = 0; jx < gad->nx; ++jx) {
      printf ( " %f", critter->x[jx] );
   }  /* for (jx... */

   printf (", f=%f, o=%f\n", critter->fitness, critter->objective );

   return 0;
} /* end function print_chromosome() */

/*--------------------------------------------------------------------*/

int ga_generation (struct ga_data *gad) {
   /*
    * Purpose...
    * -------
    * Create a new generation through select, crossover and mutation.
    *
    * Input...
    * -----
    * *gad  : pointer to the global data structure.
    *
    */
   int   mate1, mate2;
   int   child1, child2;

   /*
    * Set up the choices array.
    */
   ga_preselect (gad, gad->oldpop);
   gad->nremain = gad->popsize;

   /*
    * Repeat the process of selection, cross-over and mutation
    * until the new population is full.
    */
   child1 = 0;
   do {
      /*
       * Create the new generation in pairs.
       */
      child2 = child1 + 1;

      /*
       * Select 2 parents.
       */
      mate1 = ga_select (gad);
      mate2 = ga_select (gad);

#     if GA_DEBUG >= 4
         printf("ga_generation: selected parents are %d, %d\n", 
                 mate1, mate2);
         print_chromosome(gad, &(gad->oldpop[mate1]) );
         print_chromosome(gad, &(gad->oldpop[mate2]) );
#     endif

      /*
       * Apply the Cross-over and mutation operator.
       */
      ga_cross_over ( gad, &(gad->oldpop[mate1]), &(gad->oldpop[mate2]),
                      &(gad->newpop[child1]), &(gad->newpop[child2]) );

      /*
       * Decode the chromosomes and evaluate the ojbective function
       * for each child.
       */
      decode_chromosome ( gad, &(gad->newpop[child1]) );
      gad->newpop[child1].objective =
         (*(gad->objfunc)) ( gad->nx, gad->newpop[child1].x );
      gad->newpop[child1].parent1 = mate1;
      gad->newpop[child1].parent2 = mate2;

      decode_chromosome ( gad, &(gad->newpop[child2]) );
      gad->newpop[child2].objective =
         (*(gad->objfunc)) ( gad->nx, gad->newpop[child2].x );
      gad->newpop[child2].parent1 = mate1;
      gad->newpop[child2].parent2 = mate2;

#     if GA_DEBUG >= 4
         printf("ga_generation: children are %d, %d\n", child1, child2);
         print_chromosome(gad, &(gad->newpop[child1]) );
         print_chromosome(gad, &(gad->newpop[child2]) );
#     endif

      /* Increment the counter to point at the next 2 children. */
      child1 += 2;
   } while (child1 < gad->popsize);

   return 0;
}  /* end function ga_generation() */

/*--------------------------------------------------------------------*/

int ga_apply_elitist (struct ga_data *gad,
                      struct individual *pop) {
   /*
    * Apply the elitist strategy by ensuring that the best-so-far
    * individual is present in the new population.
    *
    * Input...
    * -----
    * *gad   : pointer to the global data structure
    * *pop   : the population we are checking.
    *
    * Output...
    * ------
    * If anything is done, this routines writes directly to the
    * individual's data structures in pop.
    *
    */
   int    jbest, jworst, jid;
   double objbest, objworst, temp;

   /*
    * Search for the best and the worst objective functions in the
    * specified population.
    */
   jbest    = 0;
   jworst   = 0;
   objbest  = pop[0].objective;
   objworst = objbest;

   for (jid = 1; jid < gad->popsize; ++jid) {
      temp = pop[jid].objective;
      if (temp > objbest)  { objbest  = temp; jbest  = jid; }
      if (temp < objworst) { objworst = temp; jworst = jid; }
   } /* end for */

   /*
    * Apply the elitist strategy by copying the best-so-far into
    * the current population.
    */
   if (gad->ultimate.objective > objbest)
      ga_copy_individual (gad, &(gad->ultimate), &(pop[jworst]) );

   return 0;
} /* end function ga_apply_elitist() */

/*--------------------------------------------------------------------*/

int ga_preselect (struct ga_data *gad, struct individual *pop) {
   /*
    * Purpose...
    * -------
    * Set up the array of mating choices for selection by the
    * stochastic remainder method.
    *
    */
   int    j, k, winner, jassign;
   double fraction[MAXPOP], expected;

   /*
    * Pass through the population once and assign
    * (the integer part) of the expected number of copies.
    * This will ensure that fitter individuals will be more
    * numerous in the parents pool.
    */
   j = 0;
   k = 0;
   do {
      expected    = pop[j].fitness / gad->avg;
      jassign     = (int) expected;
      fraction[j] = expected - jassign;
      while (jassign > 0) { gad->choices[k] = j; ++k; --jassign; }
      ++j;
   } while (j < gad->popsize && k < gad->popsize);

   /*
    * At this point, the choice array has been filled if k == gad->popsize.
    *
    * If necessary,
    * fill up the remainder of the choice array by stochastically
    * assigning individuals based on the fractional part of the
    * expected number of copies.
    *
    * Don't fill the last element this way. 
    * Instead, set to the first individual.
    */
   j = 0;
   while (k < gad->popsize - 1) {
      /* 
       * Continue to loop through the population 
       * until we find one to fill the slot 
       */
      if (j >= gad->popsize) j = 0;  /* wrap around */
      if (fraction[j] > 0.0) {
         winner = flip ( fraction[j] );
         if (winner) { gad->choices[k] = j; fraction[j] -= 1.0; ++k; }
      } /* end if */
      ++j;
   }  /* while (k... */

   /* Don't leave the last element to chance. */
   gad->choices[gad->popsize - 1] = 0;

#  if GA_DEBUG >= 3
      printf("After preselection...\n");
      for (k = 0; k < gad->popsize; ++k) {
         printf("choice[%d] = %d\n", k, gad->choices[k]);
      } /* end for */
      printf("ga_preselect(): End\n");
#  endif

   return 0;
}  /* End function ga_preselect() */

/*--------------------------------------------------------------------*/

int ga_select (struct ga_data *gad) {
   /*
    * Purpose...
    * -------
    * Select individuals via roulette wheel selection.
    * Selection is biased toward the fitter individuals
    * because the pool has been stacked with such individuals.
    * Function ga_preselect() has arranged this.
    *
    * Input...
    * -----
    * *gad   : pointer to the global data structure
    *
    * Output...
    * ------
    * ga_select() returns the index of the selected individual.
    *
    */
   int    jid, jpick;
   double rnd, partsum;

#  define  SELECT_OPTION  1

#  if SELECT_OPTION == 0
      /*
       * Roulette-wheel selection.  THIS SEEMS TO NEED FIXING
       *
       * Initialize counter and partial sum.
       */
      jid = 0;
      partsum = gad->oldpop[jid].fitness;

      /*
       * Compute a random point on the wheel.
       */
      rnd = random01 ( &(gad->iseed) );
      rnd *= gad->sumfitness;

      /*
       * Find the wheel slot.
       */
      while (partsum < rnd) {
         ++jid;
         partsum += gad->oldpop[jid].fitness;
      } /* end while */

#  else
      /*
       * Stochastic remainder selection.
       * For early selections, we randomly pick from the whole
       * of the table.  Later, we restrict selection to fitter 
       * individuals that were inserted into the early part of 
       * the choice array.
       */
      rnd = random01 ( &(gad->iseed) );
      rnd *= gad->nremain;
      jpick = (int) rnd;
      if (jpick >= gad->nremain) jpick = gad->nremain - 1;

      jid = gad->choices[jpick];

      --(gad->nremain);
      /* Remember to set nremain before any selections. */

#  endif

   return jid;
}  /* end function ga_select() */

/*--------------------------------------------------------------------*/

int ga_cross_over ( struct ga_data *gad,
                    struct individual *parent1,
                    struct individual *parent2,
                    struct individual *child1,
                    struct individual *child2 ) {
   /*
    * Purpose...
    * -------
    * Given two parents generate two children by crossing-over segments
    * of their chromosomes.
    *
    * Input...
    * -----
    * *gad     : pointer to the global data structure
    * *parent1 : pointer to parent 1
    * *parent2 :
    * *child1  : pointer to child 1
    * *child2  :
    *
    * Output...
    * ------
    * ga_cross_over() writes its results directly to the child data
    * structures.
    *
    */
   int    jbit, jcross1, jcross2, jtemp;
   double rnd;

   if ( flip(gad->pcross) == 1 ) {
      /*
       * Do cross-over with probability pcross.
       */
      ++gad->ncross;
      /*
       * Select a cross-over site along the chromosome.
       * rnd = ( (double) rand() ) / ( (double) RAND_MAX + 1.0 );
       */
      rnd = random01 ( &(gad->iseed) );
      jcross1 = rnd * gad->lchrom;
      rnd = random01 ( &(gad->iseed) );
      jcross2 = rnd * gad->lchrom;
      if (jcross1 > jcross2) {
         /* Ensure that jcross1 <= jcross2. */
         jtemp   = jcross1;
         jcross1 = jcross2;
         jcross2 = jtemp;
      } /* end if */

   } else {
      /*
       * Set the cross-over points so that nothing happens.
       */
      jcross1 = gad->lchrom;
      jcross2 = jcross1;
   } /* end if */

#  if GA_DEBUG >= 4
     printf("ga_cross_over: jcross1 = %d, jcross2 = %d\n", 
            jcross1, jcross2);
#  endif

   /*
    * Direct copy first parts of chromosomes with possible mutation.
    */
   for (jbit = 0; jbit < jcross1; ++jbit) {
      child1->chrom[jbit] = ga_mutation ( gad, parent1->chrom[jbit] );
      child2->chrom[jbit] = ga_mutation ( gad, parent2->chrom[jbit] );
   } /* end for */

   /*
    * Do the cross-over with possible mutation.
    * If jcross1 == jcross2, nothing is done here.
    */
   for (jbit = jcross1; jbit < jcross2; ++jbit) {
      child1->chrom[jbit] = ga_mutation ( gad, parent2->chrom[jbit] );
      child2->chrom[jbit] = ga_mutation ( gad, parent1->chrom[jbit] );
   } /* end for */

   /*
    * Now, copy the remainder of the chromosome with possible mutation.
    * If jcross2 == lchrom, nothing is done here.
    */
   for (jbit = jcross2; jbit < gad->lchrom; ++jbit) {
      child1->chrom[jbit] = ga_mutation ( gad, parent1->chrom[jbit] );
      child2->chrom[jbit] = ga_mutation ( gad, parent2->chrom[jbit] );
   } /* end for */

   return 0;
}  /* end function ga_cross_over() */

/*--------------------------------------------------------------------*/

int ga_mutation ( struct ga_data *gad, allele value ) {
   /*
    * Purpose...
    * -------
    * Randomly change a value (flip a bit) with probability
    * pmutation.
    *
    * Input...
    * -----
    * *gad   : pointer to the global data structure
    * value  : current value of the bit
    *
    * Output...
    * ------
    * ga_mutation() returns the new value (be it changed or no)
    * The routine also updates the global mutation counter.
    *
    */
   int  mutate;

   mutate = flip (gad->pmutation);

   if (mutate == 1) {
      ++gad->nmutation;
      return !value;        /* flip the bit */
   } else {
      return value;         /* return the bit unchanged */
   } /* end if */

} /* end function ga_mutation() */

/*--------------------------------------------------------------------*/

int ga_scale_fitness (struct ga_data *gad, struct individual *pop) {
   /*
    * Purpose...
    * -------
    * Apply sigma-truncation followed by linear scaling to convert 
    * from raw fitness to a scaled fitness measure.  
    *
    * Input...
    * -----
    * *gad   : pointer to the global data structure
    * *pop   : pointer to the population to be scaled
    *
    * Output...
    * ------
    * The individual objective function values are mapped to scaled
    * fitness values and stored in the individual data structures.
    *
    */
   int    jid;
   double a, b, delta, temp;
   double umax, umin, uavg, uvar, sigma;

#  if GA_DEBUG >= 2
      printf ("ga_scale_fitness: map objective function to fitness\n");
#  endif

   /* 
    * Fine the minimum value of the objective function.
    */
   umin = pop[0].objective;
   for (jid = 1; jid < gad->popsize; ++jid) {
      temp = pop[jid].objective;
      if (temp < umin) umin = temp;
   } /* end for */

   if (umin < 0.0) {
      /* 
       * Add an offset to ensure that all fitness
       * values are positive.
       */
      temp = 0.01 - umin;
      for (jid = 0; jid < gad->popsize; ++jid)
         pop[jid].fitness = pop[jid].objective + temp;
   } else {
      /* 
       * Simply copy the objective function values.
       */
      for (jid = 0; jid < gad->popsize; ++jid)
         pop[jid].fitness = pop[jid].objective;
   } /* end if */

#  if GA_DEBUG >= 3
      printf ("After offset...\n");
      printf ("jid, objective, fitness\n");
      for (jid = 0; jid < gad->popsize; ++jid)
         printf ("%d, %f %f\n", jid, pop[jid].objective, pop[jid].fitness);
#  endif

   /*
    * Apply sigma truncation.
    *
    * First, compute raw fitness mean and variance.
    */
   temp = pop[0].fitness;
   uavg = temp;
   for (jid = 1; jid < gad->popsize; ++jid) {
      uavg += pop[jid].fitness;
   } /* end for */
   uavg = uavg / gad->popsize;

   /* Variance and standard deviation */
   temp = pop[0].fitness - uavg;
   uvar = temp * temp;
   for (jid = 1; jid < gad->popsize; ++jid) {
      temp = uavg - pop[jid].fitness;
      uvar += temp * temp;
   } /* end for */
   uvar = uvar / (gad->popsize - 1);
   sigma = sqrt(uvar);

   /*
    * Apply sigma truncation.
    */
   for (jid = 0; jid < gad->popsize; ++jid) {
      temp = pop[jid].fitness;
      temp -= 2.0 * sigma;
      if (temp <= 0.0)
         pop[jid].fitness = 0.0;
      else
         pop[jid].fitness= temp;
   } /* end for */

#  if GA_DEBUG >= 3
      printf ("After sigma truncation...\n");
      printf ("jid, objective, fitness\n");
      for (jid = 0; jid < gad->popsize; ++jid)
         printf ("%d, %f %f\n", jid, pop[jid].objective, pop[jid].fitness);
#  endif

   /*
    * Stage 1 of linear scaling.
    * Compute mean, maximum and minimum.
    */
   temp = pop[0].fitness;
   uavg = temp;
   umin = temp;
   umax = temp;

   for (jid = 1; jid < gad->popsize; ++jid) {
      temp = pop[jid].fitness;
      uavg += temp;
      if (temp > umax) umax = temp;
      if (temp < umin) umin = temp;
   } /* end for */

   uavg = uavg / gad->popsize;

#  if GA_DEBUG >= 2
      printf ("ga_scale_fitness: linear-scaling parameters\n");
#  endif

/*
 * Now, compute the normal linear-scaling parameters
 * as per Goldberg (1989).
 *
 * The parameters are selected such that the average scaled
 * fitness is equal to the average objective value and the
 * maximum scaled fitness is FMULTIPLE times the average
 * scaled fitness.
 */

   delta = umax - umin;
   if (delta <= 0.0) {
      printf ("ga_scale_fitness: No variation in population.");
      printf ("Press <RETURN>..."); getchar();
      exit (-1);
   } /* end for */

   a = uavg * (FMULTIPLE - 1.0) / delta;
   b = (1.0 - a) * uavg;

   /*
    * Check the scaled minimum value to see if the scaling
    * needs to be adjusted.
    */
   if ( (a * umin + b) < 0.0 ) {
      /*
       * Adjust the scaling parameters such that the minimum
       * scaled fitness is zero.
       */
      a = uavg / delta;
      b = -1.0 * a * umin;
   } /* end if */

   /*
    * Now, do the linear scaling.
    */
#  if GA_DEBUG >= 2
      printf ("ga_scale_fitness: linear-scaling proper\n");
#  endif

   for (jid = 0; jid < gad->popsize; ++jid) {
      pop[jid].fitness = a * pop[jid].fitness + b;
   } /* end for */

#  if GA_DEBUG >= 3
      printf ("After linear scaling...\n");
      printf ("jid, objective, fitness\n");
      for (jid = 0; jid < gad->popsize; ++jid)
         printf ("%d, %f %f\n", jid, pop[jid].objective, pop[jid].fitness);
#  endif

   return 0;
} /* end function ga_scale_fitness() */

/*--------------------------------------------------------------------*/

int ga_statistics (struct ga_data *gad, struct individual *pop) {
   /*
    * Purpose...
    * -------
    * Calculate the population statistics of the given population.
    * These statistics are the minimum, maximum and average fitness of
    * the individuals within the population.
    *
    * Input...
    * -----
    * *gad   : pointer to the global data structure
    * *pop   : pointer to the current population (array) of individuals
    *
    * Output...
    * ------
    * Statistics are stored in the global data structure.
    *
    */
   int    jid;
   double temp;

   temp = pop[0].fitness;
   gad->best       = 0;
   gad->sumfitness = temp;
   gad->min        = temp;
   gad->max        = temp;

   for (jid = 1; jid < gad->popsize; ++jid) {
      temp = pop[jid].fitness;
      gad->sumfitness += temp;
      if (temp > gad->max) { gad->max = temp; gad->best = jid; }
      if (temp < gad->min) gad->min = pop[jid].fitness;
   } /* end for */

   gad->avg = gad->sumfitness / gad->popsize;

   return 0;
}  /* end function ga_statistics() */

/*--------------------------------------------------------------------*/

int ga_report (struct ga_data *gad) {
   /*
    * Purpose...
    * -------
    * Print a report on the state of both the old and new populations.
    * Just what is reported is regulated by the GA_DEBUG macro defined
    * in ga_opt.h.
    *
    * Input...
    * -----
    * *gad  : pointer to the global data structure
    * *pop  : pointer to the current population (array) of individuals
    *
    */
   int j, jbit, jid, jbest;

   if (gad->report_flag == 1 || GA_DEBUG >= 1) {
      printf ("\n-------- Population Report for Generation %d -------\n",
              gad->gen);
      printf ("Maximum Fitness = %f, Minimum Fitness = %f\n",
              gad->max, gad->min);
      printf ("Average Fitness = %f, Sum of Fitness  = %f\n",
              gad->avg, gad->sumfitness);

      jbest = gad->best;
      printf ("Chromosome with Maximum fitness is %d\n", gad->best);
      print_chromosome(gad, &(gad->newpop[jbest]) );
   } /* end if */

#  if GA_DEBUG >= 3
      printf ("Old population...\n");
      for (jid = 0; jid < gad->popsize; ++jid) {
         printf ("%d :", jid);
         print_chromosome(gad, &(gad->oldpop[jid]) );
      }  /* end for (jid... */

      printf ("Press return..."); getchar();

      printf ("New population...\n");
      printf ("jid   chromosome   phenotype   fitness  objective\n");
      for (jid = 0; jid < gad->popsize; ++jid) {
         printf ("%d :", jid);
         print_chromosome(gad, &(gad->newpop[jid]) );
      } /* end for (jid... */

      printf ("Press return..."); getchar();
#  endif

   return 0;
}  /* end of ga_report() */

/*----------------- end of file ga_opt.c -----------------*/

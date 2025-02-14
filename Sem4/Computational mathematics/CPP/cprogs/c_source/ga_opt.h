/* ga_opt.h
 * Header file for the Genetic Algorithm Optimizer.
 * -------------------------------------------------------
 */

/*
 * ---------
 * Constants
 * ---------
 */

/*
 * Debug levels 0: no messages printed
 *              1: summary results each generation
 *              2: subroutine tracing
 *              3: intermediate results printed
 *              4: right down to the bit level
 */
#define  GA_DEBUG   0

/*
 * Dimensions for the work arrays
 * MAXPOP    : number of individuals in a population
 * MAXSTRING : number of bits in a chromosome string
 * MAXPAR    : number of parameters encoded in a string
 */
#define  MAXPOP     200
#define  MAXSTRING  400
#define  MAXPAR      40

/*
 * Fitness scaling constant.
 * For small populations of between 50 and 100,
 * values of 1.2 to 2.0 can be used.
 */
#define  FMULTIPLE  2.0


/*
 * -------------------------
 * Types and Data Structures
 * -------------------------
 */

/*
 * Allele = bit position
 */
typedef int allele;

/*
 * Data structure for an individual
 */
struct individual {
   allele chrom[MAXSTRING];      /* Chromosome = string of bits  */
   double x[MAXPAR];             /* Phenotype  = parameter value */
   double fitness;               /* scaled fitness measure       */
   double objective;             /* value of objective function  */
   int    parent1, parent2;      /* ids of my parents            */
}; /* end struct individual */

/*
 * -----------
 * Global data
 * -----------
 */
struct ga_data {
   double (*objfunc) (int n, double x[]);  /* objective function  */

   struct individual *oldpop;    /* non-overlapping populations   */
   struct individual *newpop;    /* Note that these are pointers  */
   struct individual *temp_pop;  /* only, memory to be allocated  */

   int    popsize;               /* number of individuals in pop  */
                                 /* popsize is assumed even       */
   int    lchrom;                /* string length                 */
   int    nx;                    /* number of perameters in the   */
                                 /* phenotype                     */
   int    xbits;                 /* bits per parameter            */
   double xscale;                /* scale to keep 0.0 <= x < 1.0  */

   int    gen;                   /* generation counter            */
   int    maxgen;                /* maximum number of generations */
   int    spin;                  /* max generations with no       */
                                 /* improvement                   */
   int    spin_counter;          /* reset each time a new best    */
                                 /* is found                      */

   int    iseed;                 /* Seed for random numbers       */

   double pcross;                /* probability of cross-over     */
   double pmutation;             /* probability of mutation       */

   double sumfitness;            /* sum of population fittness    */
   int    nmutation;             /* running count of mutations    */
   int    ncross;                /* running count of cross-overs  */

   double a_scale, b_scale;      /* fitness scaling parameters    */

   double avg;                   /* Average fitness of population */
   double max;                   /* Maximum fitness of population */
   double min;                   /* Minimum fitness of population */

   int    choices[MAXPOP];       /* array of choices              */
   int    nremain;               /* remaining choices             */

   int    best;                  /* index of the individual with  */
                                 /* maximum fitness this generatn.*/
   struct individual ultimate;   /* the best individual in all    */
                                 /* previous generations          */
   int    elitist;               /* =1, apply elitist strategy    */
                                 /* =0, completely replace pop    */

   int    report_flag;           /* =1, write fitness report for  */
                                 /*     each generation           */
                                 /* =0, don't write anything      */
}; /* end struct gad */

/*
 * ---------------------
 * Function Declarations
 * ---------------------
 */

int     ga_optimize       (double (*objfunc) (int n, double x[]),
                           struct ga_data *gad,
                           double xbest[], double *fbest);

int    ga_copy_individual (struct ga_data *gad,
                           struct individual *original,
                           struct individual *target);

int    decode_chromosome  (struct ga_data *gad,
                           struct individual *critter);

int    print_chromosome   (struct ga_data *gad, 
                           struct individual *critter);

int    ga_generation      (struct ga_data *gad);

int    ga_apply_elitist   (struct ga_data *gad,
                           struct individual *pop);

int    ga_scale_fitness   (struct ga_data *gad,
                           struct individual *pop);

int    ga_statistics      (struct ga_data *gad,
                           struct individual *pop);

int    ga_report          (struct ga_data *gad);

int    ga_preselect       (struct ga_data *gad,
                           struct individual *pop);

int    ga_select          (struct ga_data *gad);

int    ga_cross_over      (struct ga_data *gad,
                           struct individual *parent1,
                           struct individual *parent2,
                           struct individual *child1,
                           struct individual *child2);

int    ga_mutation        (struct ga_data *gad,
                           allele value);

/*---------------------------------------------------------------*/

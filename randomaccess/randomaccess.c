/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; -*- */

/*
 * This code has been contributed by the DARPA HPCS program.  Contact
 * David Koester <dkoester@mitre.org> or Bob Lucas <rflucas@isi.edu>
 * if you have questions.
 *
 * GUPS (Giga UPdates per Second) is a measurement that profiles the memory
 * architecture of a system and is a measure of performance similar to MFLOPS.
 * The HPCS HPCchallenge RandomAccess benchmark is intended to exercise the
 * GUPS capability of a system, much like the LINPACK benchmark is intended to
 * exercise the MFLOPS capability of a computer.  In each case, we would
 * expect these benchmarks to achieve close to the "peak" capability of the
 * memory system. The extent of the similarities between RandomAccess and
 * LINPACK are limited to both benchmarks attempting to calculate a peak system
 * capability.
 *
 * GUPS is calculated by identifying the number of memory locations that can be
 * randomly updated in one second, divided by 1 billion (1e9). The term "randomly"
 * means that there is little relationship between one address to be updated and
 * the next, except that they occur in the space of one half the total system
 * memory.  An update is a read-modify-write operation on a table of 64-bit words.
 * An address is generated, the value at that address read from memory, modified
 * by an integer operation (add, and, or, xor) with a literal value, and that
 * new value is written back to memory.
 *
 * We are interested in knowing the GUPS performance of both entire systems and
 * system subcomponents --- e.g., the GUPS rating of a distributed memory
 * multiprocessor the GUPS rating of an SMP node, and the GUPS rating of a
 * single processor.  While there is typically a scaling of FLOPS with processor
 * count, a similar phenomenon may not always occur for GUPS.
 *
 * For additional information on the GUPS metric, the HPCchallenge RandomAccess
 * Benchmark,and the rules to run RandomAccess or modify it to optimize
 * performance -- see http://icl.cs.utk.edu/hpcc/
 *
 */

/*
 * This file contains the computational core of the single cpu version
 * of GUPS.  The inner loop should easily be vectorized by compilers
 * with such support.
 *
 * This core is used by both the single_cpu and star_single_cpu tests.
 */

//#include <hpcc.h>
#include <math.h>
#include <time.h>
#include "randomaccess.h"
#include "utility.h"
#include "PerfCounts.h"

/* Number of updates to table (suggested: 4x number of table entries) */
#define NUPDATE (4 * TableSize)
u64Int ran[128];              /* Current random numbers */

static void
RandomAccessUpdate(u64Int TableSize, u64Int *Table) {
  u64Int i;
  int j;

  /* Perform updates to main table.  The scalar equivalent is:
   *
   *     u64Int ran;
   *     ran = 1;
   *     for (i=0; i<NUPDATE; i++) {
   *       ran = (ran << 1) ^ (((s64Int) ran < 0) ? POLY : 0);
   *       table[ran & (TableSize-1)] ^= ran;
   *     }
   */

  for (i=0; i<NUPDATE/128; i++) {
/* #pragma ivdep */
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (j=0; j<128; j++) {
      ran[j] = (ran[j] << 1) ^ ((s64Int) ran[j] < 0 ? POLY : 0);
      Table[ran[j] & (TableSize-1)] ^= ran[j];
    }
  }
}

int
HPCC_RandomAccess(HPCC_Params *params, int doIO, double *GUPs, double *cpuTime, double *realTime, int *failure, u64Int *tableSizeFinal, uint64_t results[]) {
  u64Int i,j;
  u64Int temp;
  double cputime;               /* CPU time to update table */
  double realtime;              /* Real time to update table */
  double totalMem;
  unsigned int numIterations = 0;
  u64Int *Table;
  u64Int logTableSize, TableSize;
  FILE *outFile = NULL;

  if (doIO) {
    outFile = fopen( params->outFname, "a" );
    if (! outFile) {
      outFile = stderr;
      fprintf( outFile, "Cannot open output file.\n" );
      return 1;
    }
  }

  /* calculate local memory per node for the update table */
  totalMem = params->HPLMaxProcMem;
  totalMem /= sizeof(u64Int);

  /* calculate the size of update array (must be a power of 2) */
  for (totalMem *= 0.5, logTableSize = 0, TableSize = 1;
       totalMem >= 1.0;
       totalMem *= 0.5, logTableSize++, TableSize <<= 1)
    ; /* EMPTY */

  Table = HPCC_XMALLOC( u64Int, TableSize );
  if (! Table) {
    if (doIO) {
      fprintf( outFile, "Failed to allocate memory for the update table (" FSTR64 ").\n", TableSize);
      fclose( outFile );
    }
    printf("Failed to allocate memory for the update table (" FSTR64 ").\n", TableSize);
    return 1;
  }
  params->RandomAccess_N = (s64Int)TableSize;

  /* Print parameters for run */
  if (doIO) {
  fprintf( outFile, "Main table size   = 2^" FSTR64 " = " FSTR64 " words\n", logTableSize,TableSize);
  fprintf( outFile, "Number of updates = " FSTR64 "\n", NUPDATE);
  }
  printf("Main table size   = 2^" FSTR64 " = " FSTR64 " words\n", logTableSize,TableSize);
  printf("Number of updates = " FSTR64 "\n", NUPDATE);

  /* Initialize main table */
  for (i=0; i<TableSize; i++) Table[i] = i;
  
  /* Initialize Random numbers */
  for (j=0; j<128; j++)
    ran[j] = HPCC_starts ((NUPDATE/128) * j);

  /* Begin timing here */
  cputime = -HPL_timer_cputime();
  realtime = -RTSEC();
  
  /* begin performance counters here */
  
/*  printf("start cputime %f realtime %f\n",cputime,realtime);*/

  RandomAccessUpdate( TableSize, Table );

  /* End timed section */
  cputime += HPL_timer_cputime();
  realtime += RTSEC();
/*  printf("end cputime %f realtime %f\n",cputime,realtime);*/

  /* make sure no division by zero */
  *GUPs = (realtime > 0.0 ? 1.0 / realtime : -1.0);
  *GUPs *= 1e-9*NUPDATE;
  /* Print timing results */
  if (doIO) {
  fprintf( outFile, "CPU time used  = %.6f seconds\n", cputime);
  fprintf( outFile, "Real time used = %.6f seconds\n", realtime);
  fprintf( outFile, "%.9f Billion(10^9) Updates    per second [GUP/s]\n", *GUPs );
  }
  printf("CPU time used  = %.6f seconds\n", cputime);
  printf("Real time used = %.6f seconds\n", realtime);
  printf("%.9f Billion(10^9) Updates    per second [GUP/s]\n", *GUPs );

  /* Verification of results (in serial or "safe" mode; optional) */
  temp = 0x1;
  for (i=0; i<NUPDATE; i++) {
    temp = (temp << 1) ^ (((s64Int) temp < 0) ? POLY : 0);
    Table[temp & (TableSize-1)] ^= temp;
  }

  temp = 0;
  for (i=0; i<TableSize; i++)
    if (Table[i] != i)
      temp++;

  if (doIO) {
  fprintf( outFile, "Found " FSTR64 " errors in " FSTR64 " locations (%s).\n",
           temp, TableSize, (temp <= 0.01*TableSize) ? "passed" : "failed");
  }
  if (temp <= 0.01*TableSize) *failure = 0;
  else *failure = 1;
  
  // testing complete run for at least 7 seconds
  numIterations = (int) ((7 / realtime) + .5);
  numIterations = (numIterations > 0 ? numIterations : 1);
  
  // timing start
  cputime = -HPL_timer_cputime();
  realtime = -RTSEC();
  
  setupCounters();
  resetCounters();
  enableCounters();
  
  for (i=0; i<numIterations; i++) {
    RandomAccessUpdate( TableSize, Table );
  }
  // counter teardown
  disableCounters();
  readCounters(results);
  closeCounters();
  
  cputime += HPL_timer_cputime();
  realtime += RTSEC();

  // timing end
  
  /* make sure no division by zero */
  *GUPs = (realtime > 0.0 ? 1.0 / (realtime / numIterations) : -1.0);
  *GUPs *= 1e-9*NUPDATE;
  
  printf("%d iterations\n", numIterations);
  printf("CPU time used  = %.6f seconds\n", cputime);
  printf("Real time used = %.6f seconds\n", realtime);
  printf("%.9f Billion(10^9) Updates per second [GUP/s]\n", *GUPs );
  *tableSizeFinal = TableSize;
  *cpuTime = cputime;
  *realTime = realtime;
  
  HPCC_free( Table );

  if (doIO) {
    fflush( outFile );
    fclose( outFile );
  }

  return 0;
}

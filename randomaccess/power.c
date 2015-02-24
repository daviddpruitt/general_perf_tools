#include <math.h>
#include <time.h>
#include "randomaccess.h"
#include "utility.h"
#include "PerfCounts.h"
#include "globals.h"


// make life easier with a function pointer
void (*taskPtr)(u64Int TableSize, u64Int *Table);

// number of ops performed
static u64Int numCurrOps;
extern u64Int numOpsTot;

extern void instrOnly(u64Int TableSize, u64Int *Table);

/* turns out nop is a pretty universal assembly instruction :) */
#define NOP "nop\n\t"
#define FIVE_NOP NOP NOP NOP NOP NOP
#define TEN_NOP FIVE_NOP FIVE_NOP
#define FIFTY_NOP TEN_NOP TEN_NOP TEN_NOP TEN_NOP TEN_NOP
#define HUNDRED_NOP FIFTY_NOP FIFTY_NOP
#define FIVEHUNDRED_NOP HUNDRED_NOP HUNDRED_NOP HUNDRED_NOP HUNDRED_NOP HUNDRED_NOP
#define THOUSAND_NOP FIVEHUNDRED_NOP FIVEHUNDRED_NOP
#define INSTRUNITS (1000 * 1000)
#define NOPUNITS (5000 * 2000)
/*
 * run only nops
 */
static void
nop(u64Int TableSize, u64Int *Table)
{
	TableSize = TableSize;
	Table = Table;
	int i;
	
	for (i = 0; i < 5000; i++)
		asm volatile (THOUSAND_NOP THOUSAND_NOP);
}

#define ONE_MEM j = Table[j];
#define FIVE_MEM ONE_MEM ONE_MEM ONE_MEM ONE_MEM ONE_MEM
#define TEN_MEM FIVE_MEM FIVE_MEM
#define FIFTY_MEM TEN_MEM TEN_MEM TEN_MEM TEN_MEM TEN_MEM
#define HUNDRED_MEM FIFTY_MEM FIFTY_MEM
#define FIVEHUNDRED_MEM HUNDRED_MEM HUNDRED_MEM HUNDRED_MEM HUNDRED_MEM HUNDRED_MEM
#define THOUSAND_MEM FIVEHUNDRED_MEM FIVEHUNDRED_MEM


void
memOnly(u64Int TableSize, u64Int *Table)
{
	TableSize = TableSize;
	register volatile u64Int j,x, dummy;
	
	j = 0;
	
	for (x = 0; x < 1000; x++) {
		THOUSAND_MEM;
	}
	dummy += j;
	
}

u64Int
numOps(void)
{
	return numCurrOps;
}

int
PowerMeas(HPCC_Params *params, int doIO, double *GUPs, double *cpuTime, double *realTime, u64Int *tableSizeFinal, uint64_t results[])
{
  u64Int i ,j;
  //u64Int temp;
  //u64Int tableSize = 0;
  double cputime;               /* CPU time to update table */
  double realtime;              /* Real time to update table */
  double totalMem;
  unsigned int numIterations = 0;
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
  
  /* initialization of table */
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
/*  if (doIO) {*/
/*  fprintf( outFile, "Main table size   = 2^" FSTR64 " = " FSTR64 " words\n", logTableSize,TableSize);*/
/*  fprintf( outFile, "Number of updates = " FSTR64 "\n", NUPDATE);*/
/*  }*/
/*  printf("Main table size   = 2^" FSTR64 " = " FSTR64 " words\n", logTableSize,TableSize);*/
/*  printf("Number of updates = " FSTR64 "\n", NUPDATE);*/

  /* Initialize main table */
  for (i=0; i<TableSize; i++) Table[i] = rand() % TableSize;
  
  /* Initialize Random numbers */
  for (j=0; j<128; j++)
    ran[j] = HPCC_starts ((NUPDATE/128) * j);
  
  /* Initialization complete */
   /* Begin timing here */
  cputime = -HPL_timer_cputime();
  realtime = -RTSEC();

  for (i=0; i<10;i++)
  	taskPtr(TableSize, Table);

  /* End timed section */
  cputime += HPL_timer_cputime();
  realtime += RTSEC();
  
  /* make sure no division by zero */
  *GUPs = (realtime > 0.0 ? 1.0 / realtime : -1.0);
  *GUPs *= 1e-6*numCurrOps;
  
  /* Print timing results */
  if (doIO) {
  	fprintf( outFile, "CPU time used  = %.6f seconds\n", cputime);
  	fprintf( outFile, "Real time used = %.6f seconds\n", realtime);
  	fprintf( outFile, "%.9f Million(10^6) Work Units per second [BUP/s]\n", *GUPs );
  }
  printf("CPU time used  = %.6f seconds\n", cputime);
  printf("Real time used = %.6f seconds\n", realtime);
  printf("%.9f Million(10^6) Work Units per second [BUP/s]\n", *GUPs );
  printf(""FSTR64" operations per call\n", numCurrOps);
  
  // testing complete run for at least 7 seconds
  numIterations = (int) ((150 / realtime) + .5);
  numIterations = (numIterations > 0 ? numIterations : 1);
  
  // timing start
  cputime = -HPL_timer_cputime();
  realtime = -RTSEC();
  
  setupCounters();
  resetCounters();
  enableCounters();
  
  for (i=0; i<numIterations; i++) {
    taskPtr(TableSize, Table);
  }
  // counter teardown
  disableCounters();
  readCounters(results);
  closeCounters();
  
  cputime += HPL_timer_cputime();
  realtime += RTSEC();
  
  /* make sure no division by zero */
  *GUPs = (cputime > 0.0 ? 1.0 / cputime : -1.0);
  *GUPs *= 1e-6*numCurrOps*numIterations;
  //*GUPS = (numCurrOps*numIterations)/(cputime*1e6);
  
  printf("%d iterations\n", numIterations);
  printf("CPU time used  = %.6f seconds\n", cputime);
  printf("Real time used = %.6f seconds\n", realtime);
  printf("%.9f Million(10^6) Work Units per second [BUP/s]\n", *GUPs );
  printf("%.9f Million(10^6) Work Units per second [BUP/s]\n",  (numCurrOps*numIterations)/(cputime*1e6) );
  *tableSizeFinal = 0;
  *cpuTime = cputime;
  *realTime = realtime;

  if (doIO) {
    fflush( outFile );
    fclose( outFile );
  }
  return 0;
}

int
PowerMeasNop(HPCC_Params *params, int doIO, double *GUPs, double *cpuTime, double *realTime, u64Int *tableSizeFinal, uint64_t results[])
{
  taskPtr = nop;
  numCurrOps = 1000;
  return PowerMeas(params, doIO, GUPs, cpuTime, realTime, tableSizeFinal, results);
}

int
PowerMeasInstr(HPCC_Params *params, int doIO, double *GUPs, double *cpuTime, double *realTime, u64Int *tableSizeFinal, uint64_t results[])
{
	taskPtr = instrOnly;
	numCurrOps = numOpsTot;
	return PowerMeas(params, doIO, GUPs, cpuTime, realTime, tableSizeFinal, results);
}

int
PowerMeasMem(HPCC_Params *params, int doIO, double *GUPs, double *cpuTime, double *realTime, u64Int *tableSizeFinal, uint64_t results[])
{
	taskPtr = memOnly;
	numCurrOps = NOPUNITS;
	return PowerMeas(params, doIO, GUPs, cpuTime, realTime, tableSizeFinal, results);
}

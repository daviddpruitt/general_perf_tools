/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; -*- */

#ifndef RANDOM_ACCESS_H
#define RANDOM_ACCESS_H

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

/* Random number generator */
#ifdef LONG_IS_64BITS
#define POLY 0x0000000000000007UL
#define PERIOD 1317624576693539401L
#else
#define POLY 0x0000000000000007ULL
#define PERIOD 1317624576693539401LL
#endif

/* Macros for timing */
#define CPUSEC() (HPL_timer_cputime())
#define RTSEC() (HPL_timer_walltime())

/*#define LONG_IS_64BITS*/

#define XMALLOC(t,s) ((t*)malloc(sizeof(t)*(s)))
#define HPCC_XMALLOC(t,s) XMALLOC(t,s)
#define HPCC_free free

/* Define 64-bit types and corresponding format strings for printf() and constants */
#ifdef LONG_IS_64BITS
/*typedef unsigned long u64Int;*/
/*typedef long s64Int;*/
/*#define FSTR64 "%ld"*/
/*#define FSTRU64 "%lu"*/
typedef int64_t s64Int;
typedef uint64_t u64Int;
#define FSTR64 "%"PRId64
#define FSTRU64 "%"PRIu64
#define ZERO64B 0L
#else
/*typedef int32_t s64Int;*/
/*typedef uint32_t u64Int*/
/*#define FSTR64 PRId32*/
/*#define FSTRU64 PRIu32*/
typedef int64_t s64Int;
typedef uint64_t u64Int;
#define FSTR64 "%"PRId64
#define FSTRU64 "%"PRIu64
/*typedef unsigned long long u64Int;*/
/*typedef long long s64Int;*/
/*#define FSTR64 "%lld"*/
/*#define FSTRU64 "%llu"*/
#define ZERO64B 0LL
#endif

extern u64Int HPCC_starts (s64Int);
extern u64Int HPCC_starts_LCG (s64Int);

#define WANT_MPI2_TEST 0


#define HPCC_TRUE 1
#define HPCC_FALSE 0
#define HPCC_DONE 0

#define FINISHED_TAG 1
#define UPDATE_TAG   2
#define USE_NONBLOCKING_SEND 1

#define MAX_TOTAL_PENDING_UPDATES 1024
#define LOCAL_BUFFER_SIZE MAX_TOTAL_PENDING_UPDATES

#define USE_MULTIPLE_RECV 1

#ifdef USE_MULTIPLE_RECV
#define MAX_RECV					16
#else
#define MAX_RECV					1
#endif

#define LCG_MUL64 ((u64Int)6364136223846793005ULL)
#define LCG_ADD64 1

typedef struct HPCC_RandomAccess_tabparams_s {
  s64Int LocalTableSize; /* local size of the table may be rounded up >= MinLocalTableSize */
  s64Int ProcNumUpdates; /* usually 4 times the local size except for time-bound runs */

  u64Int logTableSize;   /* it is an unsigned 64-bit value to type-promote expressions */
  u64Int TableSize;      /* always power of 2 */
  u64Int MinLocalTableSize; /* TableSize/NumProcs */
  u64Int GlobalStartMyProc; /* first global index of the global table stored locally */
  u64Int Top; /* global indices below 'Top' are asigned in MinLocalTableSize+1 blocks;
                 above 'Top' -- in MinLocalTableSize blocks */

/*  MPI_Datatype dtype64;*/
//  MPI_Status *finish_statuses; /* storage for 'NumProcs' worth of statuses */
//  MPI_Request *finish_req;     /* storage for 'NumProcs' worth of requests */

  int logNumProcs, NumProcs, MyProc;

  int Remainder; /* TableSize % NumProcs */
} HPCC_RandomAccess_tabparams_t;

/* parameters of execution */
typedef struct {
  /* HPL section */
/*   HPL_T_test                 test;*/
/*   int                        nval  [HPL_MAX_PARAM],*/
/*                              nbval [HPL_MAX_PARAM],*/
/*                              pval  [HPL_MAX_PARAM],*/
/*                              qval  [HPL_MAX_PARAM],*/
/*                              nbmval[HPL_MAX_PARAM],*/
/*                              ndvval[HPL_MAX_PARAM],*/
/*                              ndhval[HPL_MAX_PARAM];*/
/*   HPL_T_ORDER                porder;*/
/*   HPL_T_FACT                 pfaval[HPL_MAX_PARAM],*/
/*                              rfaval[HPL_MAX_PARAM];*/
/*   HPL_T_TOP                  topval[HPL_MAX_PARAM];*/
/*   HPL_T_FACT                 rpfa;*/
/*   HPL_T_SWAP                 fswap;*/
   int ns, nbs, npqs, npfs, nbms, ndvs, nrfs, ntps, ndhs, tswap, L1notran, Unotran, equil, align;

  /* HPCC section */
  char inFname[256 + 1], outFname[256 + 1];
/*  int PTRANSns, PTRANSnval[2 * HPL_MAX_PARAM];*/
/*  int PTRANSnbs, PTRANSnbval[2 * HPL_MAX_PARAM];*/
/*  int PTRANSnpqs, PTRANSpval[2 * HPL_MAX_PARAM], PTRANSqval[2 * HPL_MAX_PARAM];*/
  double MPIRandomAccess_LCG_GUPs, MPIRandomAccess_GUPs, Star_LCG_GUPs, Single_LCG_GUPs, StarGUPs, SingleGUPs,
    MPIRandomAccess_ErrorsFraction, MPIRandomAccess_time, MPIRandomAccess_CheckTime,
    MPIRandomAccess_TimeBound,
    MPIRandomAccess_LCG_ErrorsFraction, MPIRandomAccess_LCG_time, MPIRandomAccess_LCG_CheckTime,
    MPIRandomAccess_LCG_TimeBound,
    StarStreamCopyGBs, StarStreamScaleGBs,
    StarStreamAddGBs, StarStreamTriadGBs, SingleStreamCopyGBs, SingleStreamScaleGBs,
    SingleStreamAddGBs, SingleStreamTriadGBs, StarDGEMMGflops, SingleDGEMMGflops;
  double StarFFTGflops, SingleFFTGflops, MPIFFTGflops, MPIFFT_maxErr;
  double MaxPingPongLatency, RandomlyOrderedRingLatency, MinPingPongBandwidth,
    NaturallyOrderedRingBandwidth, RandomlyOrderedRingBandwidth,
    MinPingPongLatency, AvgPingPongLatency, MaxPingPongBandwidth, AvgPingPongBandwidth,
    NaturallyOrderedRingLatency;
  int DGEMM_N;
  int StreamThreads, StreamVectorSize;
  int FFT_N;
  int MPIFFT_Procs;
  int MPIRandomAccess_LCG_Algorithm, MPIRandomAccess_Algorithm;

/*  HPL_RuntimeData HPLrdata;*/
/*  PTRANS_RuntimeData PTRANSrdata;*/

  int Failure; /* over all failure of the benchmark */

/*  double MPIFFTtimingsForward[MPIFFT_TIMING_COUNT], MPIFFTtimingsBackward[MPIFFT_TIMING_COUNT];*/

  size_t HPLMaxProcMem;
  int HPLMaxProc, HPLMinProc;
  int RunHPL, RunStarDGEMM, RunSingleDGEMM,
    RunPTRANS, RunStarStream, RunSingleStream,
    RunMPIRandomAccess_LCG, RunStarRandomAccess_LCG, RunSingleRandomAccess_LCG,
    RunMPIRandomAccess, RunStarRandomAccess, RunSingleRandomAccess,
    RunStarFFT, RunSingleFFT, RunMPIFFT,
    RunLatencyBandwidth;

  int FFTEnblk, FFTEnp, FFTEl2size;
  s64Int RandomAccess_LCG_N, RandomAccess_N, MPIRandomAccess_LCG_ExeUpdates, MPIRandomAccess_ExeUpdates,
    MPIRandomAccess_LCG_N, MPIRandomAccess_N, MPIRandomAccess_LCG_Errors, MPIRandomAccess_Errors, MPIFFT_N;
} HPCC_Params;

/*static void RandomAccessUpdate(u64Int TableSize, u64Int *Table);*/
int PowerMeas(HPCC_Params *params, int doIO, double *GUPs, double *cpuTime, double *realTime, u64Int *tableSizeFinal, uint64_t results[]);
int PowerMeasNop(HPCC_Params *params, int doIO, double *GUPs, double *cpuTime, double *realTime, u64Int *tableSizeFinal, uint64_t results[]);
int PowerMeasInstr(HPCC_Params *params, int doIO, double *GUPs, double *cpuTime, double *realTime, u64Int *tableSizeFinal, uint64_t results[]);
int PowerMeasMem(HPCC_Params *params, int doIO, double *GUPs, double *cpuTime, double *realTime, u64Int *tableSizeFinal, uint64_t results[]);



#if defined( RA_SANDIA_NOPT )
#define HPCC_RA_ALGORITHM 1
#elif defined( RA_SANDIA_OPT2 )
#define HPCC_RA_ALGORITHM 2
#else
#define HPCC_RA_STDALG 1
#define HPCC_RA_ALGORITHM 0
#endif

#endif // RANDOM_ACCESS_H

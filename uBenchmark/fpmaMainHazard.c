#include <stdio.h>
#include "utility.h"
#include "PerfCounts.h"

#ifndef BTYPE
# define BTYPE double
#endif

// parameter definitions
#define f(x) f##x
#define pdef(n) BTYPE f(n)
#define paramDefs pdef(1),pdef(2),pdef(3),pdef(4),pdef(5),pdef(6),pdef(7),pdef(8)//,pdef(9),pdef(10),pdef(11),pdef(12),pdef(13),pdef(14),pdef(15)

// autovar defs
#define r(x) r##x
#define rdef(n) BTYPE r(n)
#define regDefs rdef(1);rdef(2);rdef(3);rdef(4);rdef(5);rdef(6);rdef(7);rdef(8);//rdef(9);rdef(10);rdef(11);rdef(12);rdef(13);rdef(14);rdef(15);

// op type
#if OP == 1			/* muladd */
# define m(x,y) r(x) += f(x) * f(y)
# define a(x,y) res[x] += src[y] * src[y]
#elif OP == 2			/* add */
# define m(x,y) r(x) += f(y)
# define a(x,y) res[x] += src[y]
#elif OP == 3			/* mul */
# define m(x,y) r(x) *= f(y)
# define a(x,y) res[x] *= src[y]
#elif OP == 4			/* muladd */
# define m(x,y) r(x) += f(x) * f(y)
# define a(x,y) res[x] += src[y] * src[y]
#else
#error ERROR - OP OUT OF RANGE
#endif

//#define ARRAYSIZE (10 * 1024 * 1024)
#define DO_FETCH f(15) += array[i]; i = (i + stride) % (size); stride += STRIDE_ADJ;

#if LENGTH == 16
//# define do16  m(2,1);m(3,2);m(4,3);m(5,4);m(6,5);m(7,6);m(8,7);m(1,8);m(2,1);m(3,2);m(4,3);m(5,4);m(6,5);m(7,6);m(8,7);m(1,8);
# define do16 {for (i = 0; i < 8; ++i) {a(i,i);}};{for (i = 0; i < 8; ++i) {a(i,i);}};
#elif LENGTH == 1
//# define do16  m(1,1);m(1,1);m(1,1);m(1,1);m(1,1);m(1,1);m(1,1);m(1,1);m(1,1);m(1,1);m(1,1);m(1,1);m(1,1);m(1,1);m(1,1);m(1,1);
#elif LENGTH == 4
//# define do16  m(2,1);m(3,2);m(4,3);m(1,4);
#else
#error ERROR! - LENGTH is out of range should be 16 1 or 4
#endif


#if OP == 4
#define do32 do16 do16
#define do64 do32 do32
#define do192 do64 do64 do64
#define do576 do192 do192 do192
#define do2304 do576 do576 do576 do576 DO_FETCH
#define do11520 do2304 do2304 do2304 do2304 do2304 
#else
#define do32 do16 do16
#define do64 do32 do32
#define do192 do64 do64 do64
#define do576 do192 do192 do192
#define do2304 do576 do576 do576 do576
#define do11520 do2304 do2304 do2304 do2304 do2304 
#endif

BTYPE *array;
unsigned stride;
unsigned size;

BTYPE
do10800Fn(paramDefs)
{
/*  regDefs;*/
/*  r1=r2=r3=r4=r5=r6=r7=r8=(BTYPE)0.0;//=r9=r10=r11=r12=r13=r14=r15=(BTYPE)0.0;*/
  BTYPE res[8] = {0,0,0,0,0,0,0,0};
  BTYPE src[8] = {f1,f2,f3,f4,f5,f6,f7,f8};
	long i;
	
	do11520;

#if LENGTH == 16
	return res[0]+res[1]+res[2]+res[3]+res[4]+res[5]+res[6]+res[7];
#elif LENGTH == 4
  return res[0]+res[1]+res[2];
#elif LENGTH == 1
  return res[0];
#endif
/*	#endif*/
}

long long
doMany(long long numIters)
{
	long long count = 0;
	BTYPE x = 0;
	while (numIters--) {
			x +=do10800Fn(.0000001, 
					.0000002, 
					.0000003, 
					.0000004,
					.0000005,
					.0000006,
					.0000007,
					.0000008);
/*					.0000009,*/
/*					.00000010,*/
/*					.00000011,*/
/*					.00000012,*/
/*					.00000013,*/
/*					.00000014,*/
/*					.00000015);*/
					// add count based on op type
#if (OP == 1) || (OP == 4)
		count += 23040;
#else
		count += 11520;
#endif
	}				
	//TableSize = TableSize;
	//Table[0] = Table[0];
	//Table = Table;
	if (x == 0.0)
		count += 1;
	return count;
}

extern long long doManyExternal(long long numIters);

int
main() // int argc, char *argv[])
{
	double cputime = 0.0;
	long long numIters = 10;
	long long ops;
	uint64_t *counts;
	
	counts = (uint64_t*)malloc(sizeof(uint64_t) * 10);
	// if we're doing fp and arithmetic
	#if OP == 4
	size = ARRAYSIZE / 8;
	array = (double*)malloc(sizeof(double) * size);
	printf("Array pointer %p\n", array);
	printf("x=%f size=%d stride=%d stride_Adj=%d\n",x,size,STRIDE,STRIDE_ADJ);
	long i;
	stride = STRIDE;
	fflush(stdout);
	for (i = 0; i < size; i++) {
		array[i] = (BTYPE) 0.00000000001 * i;
	}
	#endif
	
	printf("After Loop\n");
	fflush(stdout);
	
	setupCounters();
	resetCounters();

	do {
		numIters *= 10;
		resetCounters();
		enableCounters();
		cputime = -HPL_timer_cputime();
		ops = doManyExternal(numIters);
		cputime += HPL_timer_cputime();
		disableCounters();
		readCounters(counts);
		printf("%g seconds, %g ops, %g ops/sec \n", cputime, (double)ops, ((double) ops) / cputime);
		printf("%"PRIu64" instr %"PRIu64" fp instr %"PRIu64" cycles %"PRIu64" dcache stalls \n" ,counts[1], counts[6], counts[0], counts[4]);
	} while (cputime < 15.0);	

	closeCounters();
	return 0;
}


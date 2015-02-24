#include <stdio.h>
#include "utility.h"
#include "PerfCounts.h"

#define f(x) f##x 

#define pdef(n) double f(n)
#define paramDefs pdef(1),pdef(2),pdef(3),pdef(4),pdef(5),pdef(6),pdef(7),pdef(8),pdef(9),pdef(10),pdef(11),pdef(12),pdef(13),pdef(14),pdef(15)
//paramDefs
//#define OP 2
#if OP == 1			/* muladd */
# define m(x) f(x) += f(x) * f(x)
#elif OP == 2			/* add */
# define m(x) f(x) += f(x)
#elif OP == 3			/* mul */
# define m(x) f(x) *= f(x)
#elif OP == 4			/* muladd */
# define m(x) f(x) += f(x) * f(x)
#else
ERROR - OP OUT OF RANGE
#endif

//#define ARRAYSIZE (10 * 1024 * 1024)
#define DO_FETCH f(15) += array[i]; i = (i + stride) % (size); stride += STRIDE_ADJ;

#define do15  m(1);m(2);m(3);m(4);m(5);m(6);m(7);m(8);m(9);m(10);m(11);m(12);m(13);m(14);m(15);

#if OP == 4
#define do30 do15 do15
#define do60 do30 do30
#define do180 do60 do60 do60
#define do540 do180 do180 do180
#define do2160 do540 do540 do540 do540 DO_FETCH
#define do10800 do2160 do2160 do2160 do2160 do2160 
#else
#define do30 do15 do15
#define do90 do30 do30 do30
#define do270 do90 do90 do90
#define do540 do270 do270 
#define do2160 do540 do540 do540 do540 
#define do10800 do2160 do2160 do2160 do2160 do2160 
#endif

u64Int numOpsTot = 1080000;
double *array;
unsigned stride;
unsigned size;

double
do10800Fn(paramDefs)
{
	//ninety;
	//five_forty;
	//two_thou_one_sixty;
	long i = 0;
	i = i;
	do10800;
/*	#if OP == 4*/
/*	return f1+f2+f3+f4+f5+f6+f7+f8+f9+f10+f11+f12+f13+f14+f15;*/
/*	#else*/
	return f1+f2+f3+f4+f5+f6+f7+f8+f9+f10+f11+f12+f13+f14+f15;
/*	#endif*/
}

long long
doMany(long long numIters)
{
	long long count = 0;
	double x = 0;
	while (numIters--) {
			x +=do10800Fn(.0000001, 
					.0000002, 
					.0000003, 
					.0000004,
					.0000005,
					.0000006,
					.0000007,
					.0000008,
					.0000009,
					.00000010,
					.00000011,
					.00000012,
					.00000013,
					.00000014,
					.00000015);
		count += 10800;
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
	double x = 0, cputime = 0.0;
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
		array[i] = (double) 0.00000000001 * i;
	}
	#endif
	
	printf("After Loop\n");
	fflush(stdout);
	
	setupCounters();
	resetCounters();
	
	printf("x %f\n",x);
	//for (i = 0; i < 1000000; i++) {
	//	x += instrOnly();
	//}

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
	printf("x %f\n",x);
	closeCounters();
	return 0;
}


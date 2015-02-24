#include "stdio.h"
#include "string.h"
#include "utility.h"

#define f(x) f##x 

#define pdef(n) double f(n)
#define paramDefs pdef(1),pdef(2),pdef(3),pdef(4),pdef(5),pdef(6),pdef(7),pdef(8),pdef(9),pdef(10),pdef(11),pdef(12),pdef(13),pdef(14),pdef(15)
//paramDefs

#if OP == 1			/* muladd */
# define m(x) f(x) += f(x) * f(x)
#elif OP == 2			/* add */
# define m(x) f(x) += f(x)
#elif OP == 3			/* mul */
# define m(x) f(x) *= f(x)
#endif



#define do15  m(1);m(2);m(3);m(4);m(5);m(6);m(7);m(8);m(9);m(10);m(11);m(12);m(13);m(14);m(15);
#define do30 do15 do15
#define do90 do30 do30 do30
#define do270 do90 do90 do90
#define do540 do270 do270 
#define do2160 do540 do540 do540 do540 
#define do10800 do2160 do2160 do2160 do2160 do2160 

#define out_start 0
#define in_start 0 
#define SIZETOCOPY SIZE
#define ARRAYSIZE (10 * 1024 * 1024)

u64Int numOpsTot = 1080000;
static size_t start = 0;

long long
doFn(size_t arrayIn[], size_t arrayOut[])
{
	
	memcpy(arrayOut + start, arrayIn + start, SIZETOCOPY);
	start = (start + STRIDE) % MAXSIZE; 
	
	return SIZETOCOPY;
}

extern long long doManyExternal(long long numIters, size_t *arrayIn, size_t *arrayOut);

int
main() // int argc, char *argv[])
{
	double x = 0, cputime = 0.0;
	long long numIters = 10;
	long long ops;

	// 4 Mb arrays to make sure we get outside L2 and into main memory
	size_t *arrayIn, *arrayOut;
	
	printf("x=%f sizetocopy=%d stride=%d\n",x,SIZETOCOPY,STRIDE);
	fflush(stdout);
	
	arrayIn = (size_t*)malloc(sizeof(arrayIn) * ARRAYSIZE);
	arrayOut = (size_t*)malloc(sizeof(arrayIn) * ARRAYSIZE);
	
	// cache warmup
	start = 0;
	doManyExternal(1000000, arrayIn, arrayOut);
	
	do {
		numIters *= 10;
		start = 0;
		cputime = -HPL_timer_cputime();
		ops = doManyExternal(numIters, arrayIn, arrayOut);
		cputime += HPL_timer_cputime();
		printf("%g seconds, %g ops, %g ops/sec \n", cputime, (double)ops, ((double) ops) / cputime);
	} while (cputime < 15.0);	
	printf("x %f\n",x);
	return 0;
}


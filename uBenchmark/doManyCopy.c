#include "string.h"
#define f(x) f##x 
#define pdef(n) double f(n)
#define paramDefs pdef(1),pdef(2),pdef(3),pdef(4),pdef(5),pdef(6),pdef(7),pdef(8),pdef(9),pdef(10),pdef(11),pdef(12),pdef(13),pdef(14),pdef(15)
long long doFn(size_t arrayIn[], size_t arrayOut[]);

long long 
doManyExternal(long long numIters, size_t *arrayIn, size_t *arrayOut)
{
	long long count = 0;
	//double x = 0;
	while (numIters--) {
		count += doFn(arrayIn, arrayOut);
	}				
	//TableSize = TableSize;
	//Table[0] = Table[0];
	//Table = Table;
/*	if (x == 0.0)*/
/*		count += 1;*/
	return count;
}

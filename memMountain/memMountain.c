#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include "randomutils.h"
#include "runTest.h"

#ifndef NUM_ELEMS
#define NUM_ELEMS 4096U
#endif
#ifndef STRIDE
#define STRIDE 1U
#endif
#ifndef RANDOM_ACCESS
#define RANDOM_ACCESS 1
#endif
#ifndef LOAD_WIDTH
#define LOAD_WIDTH 8
#endif
#ifndef NUM_ITERS
#define NUM_ITERS 1000000000U
#endif

#define THOUS   1000
#define MILLION 1000000
#define BILLION 1000000000

// this is totally a hack because some compilers
// happily ignore the long part of the number and
// we have bits truncated
uint64_t numElems = NUM_ELEMS;
uint64_t stride = STRIDE;
uint64_t numIters = NUM_ITERS;
uint64_t loadWidth = LOAD_WIDTH;
uint64_t randomAccess = RANDOM_ACCESS;

void fillArray(double *array, uint64_t numElems)
{
  uint64_t index;
  for (index = 0; index < numElems; index++) {
    if (randomAccess) {
      array[index] = getRand(0, numElems);
    } else {
      array[index] = 1.00; //(index + loadWidth * stride) % numElems;
    }
  }
}

// from http://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html
/* Subtract the struct timespec values X and Y,
   storing the result in RESULT.
   Return 1 if the difference is negative, otherwise 0. */
int
timespec_subtract (struct timespec *result, struct timespec *x, struct timespec *y)
{
  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_nsec < y->tv_nsec) {
    int nsec = (y->tv_nsec - x->tv_nsec) / BILLION + 1;
    y->tv_nsec -= BILLION * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_nsec - y->tv_nsec > BILLION) {
    int nsec = (x->tv_nsec - y->tv_nsec) / BILLION;
    y->tv_nsec += BILLION * nsec;
    y->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_nsec = x->tv_nsec - y->tv_nsec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}

void init(void)
{
  srand(timeSeed());
}

int main(int argc, char **argv)
{
  double array[NUM_ELEMS], retval;
  //array = (double*)malloc(NUM_ELEMS * sizeof(double));
  uint64_t numLoads = (numElems / stride) * numIters;
  //printf("NUM_ELEMS/STRIDE %" PRIu64 "\n", (numElems / stride));
  //printf("sizeof(numLoads) %zu \n", sizeof(numLoads));
  printf("Num Elems %" PRIu64 " stride %" PRIu64 " iters %" PRIu64 " loads %" PRIu64 "\n",
	 numElems, stride, numIters, numLoads);
  uint64_t numBytes = numLoads * sizeof(uint64_t);
  struct timespec startTime;
  struct timespec stopTime;
  struct timespec diffTime;
  double elapsed, latency;
  double gBytesPerSec, opsPerSec;
  int index_0 = 0, index_1 = 1, index_2 = 2, index_3 = 3;
  int index_4 = 4, index_5 = 5, index_6 = 6, index_7 = 7;

  fillArray(array, numElems);

  // cache warmup
  //retval = runTest(1, array, numElems,
  //		   index_0, index_1, index_2, index_3, index_4, index_5, index_6, index_7);
  fillArray(array, numElems);


  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &startTime);
  retval = runTest(numIters, array, numElems,
  		   index_0, index_1, index_2, index_3, index_4, index_5, index_6, index_7);
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &stopTime);

  timespec_subtract(&diffTime, &stopTime, &startTime);
  elapsed = (diffTime.tv_sec * 1.0) + (diffTime.tv_nsec * 1.0) / BILLION;
  latency = (elapsed / (numLoads * 1.0)) * BILLION;
  gBytesPerSec = numBytes / (elapsed * BILLION);
  opsPerSec = (numLoads * 1.0) / elapsed;
  printf("Retval %f\n", retval);
  printf("Elapsed: %f Bytes: %" PRIu64 " gB/sec %f latency %f ops/sec %f\n",
	 elapsed,
	 numBytes,
	 gBytesPerSec,
	 latency,
	 opsPerSec);

  return 0;
}

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
#define NUM_ITERS 100000000U
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

uint64_t fillArray(uint64_t *array, uint64_t numElems)
{
  uint64_t index;
  for (index = 0; index < numElems; index++) {
    if (randomAccess) {
      array[index] = getRand(0, numElems);
    } else {
      array[index] = (index + loadWidth * stride) % numElems;
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

int main(int argc, char **argv)
{
  uint64_t array[NUM_ELEMS];
  uint64_t numLoads = (numElems / stride) * numIters;
  printf("NUM_ELEMS/STRIDE %" PRIu64 "\n", (numElems / stride));
  printf("sizeof(numLoads) %zu \n", sizeof(numLoads));
  printf("size %" PRIu64 " stride %" PRIu64 " iters %" PRIu64 " loads %" PRIu64 "\n",
	 numElems, stride, numIters, numLoads);
  uint64_t numBytes = numLoads * sizeof(uint64_t);
  struct timespec startTime;
  struct timespec stopTime;
  struct timespec diffTime;
  double elapsed;
  double gBytesPerSec;
  double latency;
  int index_0 = 0, index_1 = 0, index_2 = 0, index_3 = 0;
  int index_4 = 0, index_5 = 0, index_6 = 0, index_7 = 0;

  uint64_t retval;
  srand(timeSeed());

  fillArray(array, numElems);

  // cache warmup
  retval = runTest(1, array, numElems,
		   index_0, index_1, index_2, index_3, index_4, index_5, index_6, index_7);

  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &startTime);
  retval = runTest(NUM_ITERS, array, numElems,
		   index_0, index_1, index_2, index_3, index_4, index_5, index_6, index_7);
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &stopTime);

  timespec_subtract(&diffTime, &stopTime, &startTime);
  elapsed = (diffTime.tv_sec * 1.0) + (diffTime.tv_nsec * 1.0) / BILLION;
  gBytesPerSec = numBytes / (elapsed * BILLION);
  latency = (elapsed / (numLoads * 1.0)) * BILLION;
  printf("Retval %" PRIu64 "\n", retval);
  printf("Elapsed: %f Bytes: %" PRIu64 " gB/sec %f latency %f ops/sec %f\n",
	 elapsed,
	 numBytes,
	 gBytesPerSec,
	 latency,
	 numLoads);
}

#include "runTest.h"

uint64_t
runTest(uint64_t numIters, uint64_t *testArray, uint64_t testArraySize,
	uint64_t index_0, uint64_t index_1, uint64_t index_2, uint64_t index_3,
	uint64_t index_4, uint64_t index_5, uint64_t index_6, uint64_t index_7)
{
  uint64_t dest;
  uint64_t iter;
  uint64_t chunkNum;
  // indexes for loading arrays in a single loop iteration
  //volatile int index_0, index_1, index_2, index_3, index_4, index_5, index_6, index_7;
  //volatile int index_8, index_9, index_10, index_11, index_12, index_13, index_14, index_15;

  // init the indexes in case they're needed
  index_0 = stride * 0; index_1 = stride * 1; index_2 = stride * 2; index_3 = stride * 3;
  index_4 = stride * 4; index_5 = stride * 5; index_6 = stride * 6; index_7 = stride * 7;
  //index_8 = stride * 8; index_9 = stride * 9; index_10 = stride * 10; index_11 = stride * 11;
  //index_12 = stride * 12; index_13 = stride * 13; index_14 = stride * 14; index_15 = stride * 15;

  dest = 0;
  for (iter = 0; iter < numIters; iter++) {
    #include "access.h" 
  }

  dest = index_0 + index_1 + index_2 + index_3 + index_4 + index_5 + index_6 + index_7;
  //dest = index_8 + index_9 + index_10 + index_11 + index_12 + index_13 + index_14 + index_15;
  return dest;
}

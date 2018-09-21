#include <stdio.h>
#include <time.h>
#include "iacaMarks.h"
#include "runTest.hpp"

// allow fused multiply add
#pragma STDC FP_CONTRACT ON

uint64_t bogusArray[16];

void quickTest(double *array, double seed)
{
  double *arrayVals = array;
  seed += array[0];

  runTest<double>(10, array, arrayVals, 10000,0,1,2,3,4,5,6,7);
}

template <typename arrayType>
arrayType runTest(uint64_t numIters, arrayType *testArray, arrayType *seedValue, uint64_t testArraySize,
		  uint64_t index_0, uint64_t index_1, uint64_t index_2, uint64_t index_3,
		  uint64_t index_4, uint64_t index_5, uint64_t index_6, uint64_t index_7)
{
  arrayType dest_0, dest_1, dest_2, dest_3, dest_4, dest_5, dest_6, dest_7;
  uint64_t iter;

  // initialize destinations
  dest_0 = seedValue[0]; dest_1  = seedValue[1]; dest_2 = seedValue[2]; dest_3  = seedValue[3];
  dest_4 = seedValue[4]; dest_5  = seedValue[5]; dest_6 = seedValue[6]; dest_7  = seedValue[7];

  for (iter = 0; iter < (numIters); iter++) {
    // keeps the inner loop from having lots of optimizatiions
    // applied to the ops
    // as a bonus every arch has nops
    asm volatile ("nop");
    //IACA_START;
    #include "access.h"
    //IACA_END;
    asm volatile("nop");
  }

  testArray[0] = dest_0 + dest_1 + dest_2 + dest_3  + dest_4  + dest_5  + dest_6  + dest_7;

  return testArray[0];
}

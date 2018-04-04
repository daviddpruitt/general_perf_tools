#include <stdio.h>
#include "iacaMarks.h"
#include "runTest.h"
#include "archSpecific.h"

#define SCALING_FACTOR 0.0000000000000000001

uint64_t bogusArray[16];
double
runTest(uint64_t numIters, double *inputArray, uint64_t testArraySize,
	uint64_t index_0, uint64_t index_1, uint64_t index_2, uint64_t index_3,
	uint64_t index_4, uint64_t index_5, uint64_t index_6, uint64_t index_7)
{
  register double dest_0 asm (FP_PREFIX"0");   register double dest_8  asm (FP_PREFIX"8");
  register double dest_1 asm (FP_PREFIX"1");   register double dest_9  asm (FP_PREFIX"9");
  register double dest_2 asm (FP_PREFIX"2");   register double dest_10 asm (FP_PREFIX"10"); 
  register double dest_3 asm (FP_PREFIX"3");   register double dest_11 asm (FP_PREFIX"11");
  register double dest_4 asm (FP_PREFIX"4");   register double dest_12 asm (FP_PREFIX"12"); 
  register double dest_5 asm (FP_PREFIX"5");   register double dest_13 asm (FP_PREFIX"13");
  register double dest_6 asm (FP_PREFIX"6");   register double dest_14 asm (FP_PREFIX"14"); 
  register double dest_7 asm (FP_PREFIX"7");   register double dest_15 asm (FP_PREFIX"15");

  register double *testArray asm(BASE_REG);
  
  //double dest_1 = index_1, dest_2 = index_2, dest_3 = index_3, 
  //       dest_4 = index_4, dest_5 = index_5, dest_6 = index_6, dest_7 = index_7;
  uint64_t iter;
  //uint64_t chunkNum;
  // indexes for loading arrays in a single loop iteration

  // initialize destinations 
  dest_0  = 1 - SCALING_FACTOR; dest_1  = 1 + 2 * SCALING_FACTOR; 
  dest_2  = 1 + SCALING_FACTOR; dest_3  = 1 - 2 * SCALING_FACTOR; 
  dest_4  = 1 - SCALING_FACTOR; dest_5  = 1 + 2 * SCALING_FACTOR; 
  dest_6  = 1 + SCALING_FACTOR; dest_7  = 1 - 2 * SCALING_FACTOR;
  dest_8  = 1 - SCALING_FACTOR; dest_9  = 1 + 2 * SCALING_FACTOR; 
  dest_10 = 1 + SCALING_FACTOR; dest_11 = 1 - 2 * SCALING_FACTOR; 
  dest_12 = 1 - SCALING_FACTOR; dest_13 = 1 + 2 * SCALING_FACTOR; 
  dest_14 = 1 + SCALING_FACTOR; dest_15 = 1 - 2 * SCALING_FACTOR;

  // init the indexes in case they're needed
  //index_0 = stride * 0; index_1 = stride * 1; index_2 = stride * 2; index_3 = stride * 3;
  //index_4 = stride * 4; index_5 = stride * 5; index_6 = stride * 6; index_7 = stride * 7;
  //index_8 = stride * 8; index_9 = stride * 9; index_10 = stride * 10; index_11 = stride * 11;
  //index_12 = stride * 12; index_13 = stride * 13; index_14 = stride * 14; index_15 = stride * 15;
 
  //testArray = inputArray;
  asm("movq %0, %%rbx"::"g" (inputArray));
  for (iter = 0; iter < numIters; iter++) {
    #include "access.h"
  }

  // hack to keep compiler from optimizing away previous loop since ops aren't used
  // note to self need extra registers
  inputArray[0]  = dest_0;  inputArray[1]  = dest_1;  inputArray[2]  = dest_2;  inputArray[3]  = dest_3;
  inputArray[4]  = dest_4;  inputArray[5]  = dest_5;  inputArray[6]  = dest_6;  inputArray[7]  = dest_7;
  inputArray[8]  = dest_8;  inputArray[9]  = dest_9;  inputArray[10] = dest_10; inputArray[11] = dest_11;
  inputArray[12] = dest_12; inputArray[13] = dest_13; inputArray[14] = dest_14; inputArray[15] = dest_15;

 
  testArray[0] = dest_0 + dest_1 + dest_2 + dest_3  + dest_4  + dest_5  + dest_6  + dest_7;
  testArray[0] = dest_8 + dest_9 + dest_10 + dest_11 + dest_12 + dest_13 + dest_14 + dest_15 + testArray[0];
 
  return testArray[0];
}

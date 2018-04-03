#include <stdio.h>
#include "iacaMarks.h"
#include "runTest.h"

#define SCALING_FACTOR 0.0001

double
runTest(uint64_t numIters, double *inputArray, uint64_t testArraySize,
	uint64_t index_0, uint64_t index_1, uint64_t index_2, uint64_t index_3,
	uint64_t index_4, uint64_t index_5, uint64_t index_6, uint64_t index_7)
{
  register double dest_0 asm ("xmm0"); dest_0 = index_0 *  1.0;
  register double dest_1 asm ("xmm1"); dest_1 = index_1 *  0.9;
  register double dest_2 asm ("xmm2"); dest_2 = index_2 *  1.0;
  register double dest_3 asm ("xmm3"); dest_3 = index_3 *  0.9;
  register double dest_4 asm ("xmm4"); dest_4 = index_4 *  1.0;
  register double dest_5 asm ("xmm5"); dest_5 = index_5 *  0.9;
  register double dest_6 asm ("xmm6"); dest_6 = index_6 *  1.0;
  register double dest_7 asm ("xmm7"); dest_7 = index_7 *  0.9;
  register volatile double *testArray asm("rbx");
  
  //double dest_1 = index_1, dest_2 = index_2, dest_3 = index_3, 
  //       dest_4 = index_4, dest_5 = index_5, dest_6 = index_6, dest_7 = index_7;
  uint64_t iter;
  uint64_t chunkNum;
  // indexes for loading arrays in a single loop iteration

  /* int i; */
  /* for (i = 0; i < testArraySize; i++) { */
  /*   printf("%g ", inputArray[i]); */
  /* } */
  /* printf("\n"); */
  /* // printf("testarray[32] %f inputarray[32] %f\n", testArray[31], inputArray[31]); */
  /* printf("dest_0 %g dest_1 %g, dest_2 %g dest_3 %g dest_4 %g, dest_5 %g dest_6 %g, dest_7 %g\n", dest_0, dest_1, dest_2, dest_3, dest_4, dest_5, dest_6, dest_7); */
  /* printf("array %f \n", inputArray[0]); */
 
  //printf("testArray[32] %p inputarray[32] %p\n", &testArray[512], &inputArray[512]);
  dest_0 = 1 - SCALING_FACTOR; dest_1 = 1 + 2 * SCALING_FACTOR; 
  dest_2 = 1 + SCALING_FACTOR; dest_3 = 1 - 2 * SCALING_FACTOR; 
  dest_4 = 1 - SCALING_FACTOR; dest_5 = 1 + 2 * SCALING_FACTOR; 
  dest_6 = 1 + SCALING_FACTOR; dest_7 = 1 - 2 * SCALING_FACTOR;

  // init the indexes in case they're needed
  //index_0 = stride * 0; index_1 = stride * 1; index_2 = stride * 2; index_3 = stride * 3;
  //index_4 = stride * 4; index_5 = stride * 5; index_6 = stride * 6; index_7 = stride * 7;
  //index_8 = stride * 8; index_9 = stride * 9; index_10 = stride * 10; index_11 = stride * 11;
  //index_12 = stride * 12; index_13 = stride * 13; index_14 = stride * 14; index_15 = stride * 15;
 
  testArray = inputArray;
  //asm("movq %0, %%rbx"::"g" (inputArray));
  for (iter = 0; iter < numIters; iter++) {
    #include "access.h"
    /* printf("dest_0 %g dest_1 %g, dest_2 %g dest_3 %g dest_4 %g, dest_5 %g dest_6 %g, dest_7 %g\n", dest_0, dest_1, dest_2, dest_3, dest_4, dest_5, dest_6, dest_7); */
    /* printf("array %f \n", inputArray[0]); */
  }

  /* printf("dest_0 %g dest_1 %g, dest_2 %g dest_3 %g dest_4 %g, dest_5 %g dest_6 %g, dest_7 %g\n", dest_0, dest_1, dest_2, dest_3, dest_4, dest_5, dest_6, dest_7); */
 
  dest_0 = dest_0 + dest_1 + dest_2 + dest_3 + dest_4 + dest_5 + dest_6 + dest_7;
 
  return dest_0;
}

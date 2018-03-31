#include <stdio.h>
#include "iacaMarks.h"
#include "runTest.h"

#define SCALING_FACTOR 0.0000001

double
runTest(uint64_t numIters, double * testArray, uint64_t testArraySize,
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
  
  //double dest_1 = index_1, dest_2 = index_2, dest_3 = index_3, 
  //       dest_4 = index_4, dest_5 = index_5, dest_6 = index_6, dest_7 = index_7;
  uint64_t iter;
  uint64_t chunkNum;
  // indexes for loading arrays in a single loop iteration
  //volatile int index_0, index_1, index_2, index_3, index_4, index_5, index_6, index_7;
  //volatile int index_8, index_9, index_10, index_11, index_12, index_13, index_14, index_15;

  
  dest_0 = 1 * SCALING_FACTOR; dest_1 = 2 * SCALING_FACTOR; 
  dest_2 = 3 * SCALING_FACTOR; dest_3 = 4 * SCALING_FACTOR; 
  dest_4 = 5 * SCALING_FACTOR; dest_5 = 6 * SCALING_FACTOR; 
  dest_6 = 9 * SCALING_FACTOR; dest_7 = 8 * SCALING_FACTOR;

  //printf("dest_0 %f\t dest_1 %f, dest_2 %f\n", dest_0, dest_1, dest_2);

  //#include "assignRegs.h"
  //#include "assignDest.h"

  printf("dest_0 %f\t dest_1 %f, dest_2 %f\n", dest_0, dest_1, dest_2);

  // init the indexes in case they're needed
  index_0 = stride * 0; index_1 = stride * 1; index_2 = stride * 2; index_3 = stride * 3;
  index_4 = stride * 4; index_5 = stride * 5; index_6 = stride * 6; index_7 = stride * 7;
  //index_8 = stride * 8; index_9 = stride * 9; index_10 = stride * 10; index_11 = stride * 11;
  //index_12 = stride * 12; index_13 = stride * 13; index_14 = stride * 14; index_15 = stride * 15;

  for (iter = 0; iter < numIters; iter++) {
    printf("%f \t", dest_1);
    #include "access.h"
    printf("%f \n", dest_1);
  }
 
  //dest = index_0 + index_1 + index_2 + index_3 + index_4 + index_5 + index_6 + index_7;
  //dest = index_8 + index_9 + index_10 + index_11 + index_12 + index_13 + index_14 + index_15;

  //#include "assignDest.h"
  //#include "assignDest.h"

  printf("dest_0 %f\t dest_1 %f, dest_2 %f\n", dest_0, dest_1, dest_2);

  dest_0 = dest_0 + dest_1 + dest_2 + dest_3 + dest_4 + dest_5 + dest_6 + dest_7;
 
  return dest_0;
}

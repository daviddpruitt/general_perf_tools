#include <stdio.h>
#include <time.h>
#include "iacaMarks.h"
#include "runTest.h"
#include "archSpecific.h"

uint64_t bogusArray[16];

double
runTest(uint64_t numIters, double *inputArray, double seedValue, uint64_t testArraySize,
	uint64_t index_0, uint64_t index_1, uint64_t index_2, uint64_t index_3,
	uint64_t index_4, uint64_t index_5, uint64_t index_6, uint64_t index_7)
{
  register double dest_0  asm (FP_PREFIX"0" ); register double dest_1  asm (FP_PREFIX"1" );
  register double dest_2  asm (FP_PREFIX"2" ); register double dest_3  asm (FP_PREFIX"3" );
  register double dest_4  asm (FP_PREFIX"4" ); register double dest_5  asm (FP_PREFIX"5" ); 
  register double dest_6  asm (FP_PREFIX"6" ); register double dest_7  asm (FP_PREFIX"7" );
  register double dest_8  asm (FP_PREFIX"8" ); register double dest_9  asm (FP_PREFIX"9" ); 
  register double dest_10 asm (FP_PREFIX"10"); register double dest_11 asm (FP_PREFIX"11");
  register double dest_12 asm (FP_PREFIX"12"); register double dest_13 asm (FP_PREFIX"13"); 
  register double dest_14 asm (FP_PREFIX"14"); 

  // Create an index into the array using a register
  register double *testArray asm(BASE_REG);asm volatile("":: "g"(testArray));
  
  uint64_t iter;
  //uint64_t chunkNum;
  
  // initialize destinations 
  dest_0  = 1 -     seedValue; dest_1  = 1 + 1.2 * seedValue; 
  dest_2  = 1 +     seedValue; dest_3  = 1 - 1.2 * seedValue; 
  dest_4  = 1 - 1.3 * seedValue; dest_5  = 1 + 1.4 * seedValue; 
  dest_6  = 1 + 1.3 * seedValue; dest_7  = 1 - 1.4 * seedValue;
  dest_8  = 1 - 1.5 * seedValue; dest_9  = 1 + 1.6 * seedValue; 
  dest_10 = 1 + 1.5 * seedValue; dest_11 = 1 - 1.6 * seedValue; 
  dest_12 = 1 - 1.6 * seedValue; dest_13 = 1 + 1.7 * seedValue; 
  dest_14 = 1 - .1  * seedValue; 

  __asm__ __volatile__("" :: "g" (dest_0));  __asm__ __volatile__("" :: "g" (dest_1));
  __asm__ __volatile__("" :: "g" (dest_2));  __asm__ __volatile__("" :: "g" (dest_3));
  __asm__ __volatile__("" :: "g" (dest_4));  __asm__ __volatile__("" :: "g" (dest_5));
  __asm__ __volatile__("" :: "g" (dest_6));  __asm__ __volatile__("" :: "g" (dest_7));
  __asm__ __volatile__("" :: "g" (dest_8));  __asm__ __volatile__("" :: "g" (dest_9));
  __asm__ __volatile__("" :: "g" (dest_10)); __asm__ __volatile__("" :: "g" (dest_11));
  __asm__ __volatile__("" :: "g" (dest_12)); __asm__ __volatile__("" :: "g" (dest_13));
  __asm__ __volatile__("" :: "g" (dest_14)); //__asm__ __volatile__("" :: "g" (dest_15));

  // init the indexes in case they're needed
  //index_0 = stride * 0; index_1 = stride * 1; index_2 = stride * 2; index_3 = stride * 3;
  //index_4 = stride * 4; index_5 = stride * 5; index_6 = stride * 6; index_7 = stride * 7;
  //index_8 = stride * 8; index_9 = stride * 9; index_10 = stride * 10; index_11 = stride * 11;
  //index_12 = stride * 12; index_13 = stride * 13; index_14 = stride * 14; index_15 = stride * 15;
 
  //int printed = 0;
  //testArray = inputArray;
  asm("movq %0, %%rbx"::"g" (inputArray));
  for (iter = 0; iter < numIters; iter++) {
    #include "access.h"
    /*if (iter % 100000 == 0) {printf("dest_0 %g dest_1 %g dest_2 %g dest_3 %g "
			      "dest_4 %g dest_5 %g dest_6 %g dest_7 %g "
			      "dest_8 %g dest_9 %g dest_10 %g dest_11 %g "
			      "dest_12 %g dest_13 %g dest_14 %g iter %lu\n", 
			      dest_0, dest_1, dest_2, dest_3, dest_4,
			      dest_5, dest_6, dest_7, dest_8, dest_9,
			      dest_10, dest_11, dest_12, dest_13, dest_14, iter);
			      printf("%lu %lu\n", iter, time(NULL));fflush(stdout);}*/
  }

  // hack to keep compiler from optimizing away previous loop since ops aren't used
  /* bogusArray[0]  = dest_0;  bogusArray[1]  = dest_1;  bogusArray[2]  = dest_2;  bogusArray[3]  = dest_3; */
  /* bogusArray[4]  = dest_4;  bogusArray[5]  = dest_5;  bogusArray[6]  = dest_6;  bogusArray[7]  = dest_7; */
  /* bogusArray[8]  = dest_8;  bogusArray[9]  = dest_9;  bogusArray[10] = dest_10; bogusArray[11] = dest_11; */
  /* bogusArray[12] = dest_12; bogusArray[13] = dest_13; bogusArray[14] = dest_14; bogusArray[15] = dest_15; */

 
  testArray[0] = dest_0 + dest_1 + dest_2 + dest_3  + dest_4  + dest_5  + dest_6  + dest_7;
  testArray[0] = dest_8 + dest_9 + dest_10 + dest_11 + dest_12 + dest_13;// + testArray[0];//+ dest_14 + dest_15 + testArray[0];
 
  return testArray[0];
}

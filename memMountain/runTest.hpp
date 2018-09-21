#include <cstdint>
#include <cinttypes>

//uint64_t
//runTest(uint64_t numIters, uint64_t *testArray, uint64_t testArraySize);

template <typename arrayType>
arrayType runTest(uint64_t numIters, arrayType *testArray, arrayType *seedValue, uint64_t testArraySize,
		  uint64_t index_0, uint64_t index_1, uint64_t index_2, uint64_t index_3,
		  uint64_t index_4, uint64_t index_5, uint64_t index_6, uint64_t index_7);

template double
runTest<double>(uint64_t numIters, double *testArray, double *seedValue, uint64_t testArraySize,
		uint64_t index_0, uint64_t index_1, uint64_t index_2, uint64_t index_3,
		uint64_t index_4, uint64_t index_5, uint64_t index_6, uint64_t index_7);

template float
runTest<float>(uint64_t numIters, float *testArray, float *seedValue, uint64_t testArraySize,
	       uint64_t index_0, uint64_t index_1, uint64_t index_2, uint64_t index_3,
	       uint64_t index_4, uint64_t index_5, uint64_t index_6, uint64_t index_7);

#ifndef ARRAY_UTIL_HPP
#define ARRAY_UTIL_HPP

#include <random>
#include <iostream>

template <typename arrayType>
void fillArrayForAddition(arrayType arrayToFill, size_t arrayLength);

template <typename arrayType>
void fillArrayMultiplication(arrayType arrayToFill, size_t arrayLength);

void fillArrayPointerChase(size_t *arrayToFill, size_t arrayLength, size_t stride);

#endif // ARRAY_UTIL_HPP

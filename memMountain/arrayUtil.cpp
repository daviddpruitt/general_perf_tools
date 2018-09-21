#include "arrayUtil.hpp"

template <typename arrayType>
void fillArrayForAddition(arrayType arrayToFill, size_t arrayLength)
{
  size_t currElem;

  std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(-1.0, 1.0);

  for (currElem = 0; currElem < arrayLength; currElem++) {
    arrayToFill[currElem] = dis(gen);
  }
}

template <typename arrayType>
void fillArrayMultiplication(arrayType arrayToFill, size_t arrayLength)
{
  size_t currElem;

  std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(0.5, 2.0);
  std::uniform_int_distribution<> tOrF(0, 1);

  for (currElem = 0; currElem < arrayLength; currElem++) {
    if (tOrF)
      arrayToFill[currElem] = dis(gen);
    else
      arrayToFill[currElem] = -1.0 * dis(gen);      
  }
}

void fillArrayPointerChase(size_t *arrayToFill, size_t arrayLength, size_t stride = 1)
{
  size_t currElem;
  size_t index = 0, prevIndex;

  for (currElem = 0; currElem < arrayLength; currElem++) {
    prevIndex = index;
    index = (index + stride) % arrayLength;
    arrayToFill[prevIndex] = index;
  }
}

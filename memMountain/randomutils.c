#include <inttypes.h>
#include <time.h>

size_t 
timeSeed(void)
{
  time_t current = time(0);
  char* timeTPtr = (char*)&current;
  size_t hash, fnv_prime, fnv_offset;
  int index;

  if (sizeof(size_t) == 8) {
    fnv_offset = 14695981039346656037U;
    fnv_prime = 1099511628211U;
  }
  else {
    fnv_offset = 2166136261;
    fnv_prime = 16777619;
  }

  hash = fnv_offset;
  for (index = 0; index < sizeof(time_t); index++) {
    hash ^= timeTPtr[index];
    hash *= fnv_prime;
  } 

  return hash;
}

// Get a random number from min to max inclusive
int
getRand(int min, int max)
{
  int result;
  do {
    result = rand();
  } while (min >= result || result >= max);
  return result;
}
  

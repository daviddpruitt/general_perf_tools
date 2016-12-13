#ifndef MEMFUNCS_H
#define MEMFUNCS_H
#include <unistd.h>  // for size_t
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  // for uint_t
#include <errno.h>   // for errno

#define NUM_REGIONS 5
#define NUM_TYPES 4
#define NUM_RANDOM 513
#define regionSize(s) (size / 5)
#define numElements(s) (s / sizeof(uint64_t))
#define dbg_printf if(0) printf

#define KiB (1 << 10)
#define MiB (1 << 20)
#define GiB (1 << 30)

extern const uint64_t rndData[];
extern const uint64_t full;
extern const uint64_t empty;
extern const uint64_t fence1;
extern const uint64_t fence2;
extern uint64_t types[4];

size_t allocMem(size_t totalSize, uint64_t volatile **region);
size_t fillMem(size_t totalSize, uint64_t volatile *region);

#endif // MEMFUNCS_H

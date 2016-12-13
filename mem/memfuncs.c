#include "memfuncs.h"
#include "data.h"

size_t allocMem(size_t totalSize, uint64_t volatile **region)
{
  // alloc memory
  uint64_t *locReg = (uint64_t*)malloc(totalSize);
  int retval;

  if (!locReg) {
    printf("Error unable to allocate memory\nExiting...\n");
    exit(1);
  }

  fprintf(stderr, "Allocation succeeded attempting to lock...");
  retval = mlock(locReg, totalSize);
  if (retval) {
    printf("Error unable to lock memory: %s\n", strerror(errno));
    free(locReg);
    exit(1);
  }
  fprintf(stderr, "\tlocking succeeded\n");
  *region = locReg;
  return 0;
}
size_t fillMem(size_t totalSize, uint64_t volatile *region)
{
  // elems in a sub region
  size_t subRegionElems = totalSize / (10 * sizeof(uint64_t));
  size_t currentSubLoc;// current location in subregion
  size_t volatile *currentLoc = region;
  int num; int regionNum;

  fprintf(stderr, "subRegionElems %zu %zu\n", 
	  subRegionElems, 
	  (10 * sizeof(uint64_t)) );
  // fill as two halfs
  for (num = 0; num < 2; num++) {
    //fprintf(stderr, "Half %d\n", num);
    for (regionNum = 0; regionNum < NUM_TYPES; regionNum++) {
      //fprintf(stderr, "Region %d\n", regionNum);
      // copy the normal fill types
      for (currentSubLoc = 0; currentSubLoc < subRegionElems; currentSubLoc++) {
	currentLoc[currentSubLoc] = types[regionNum];
      }
      currentLoc += subRegionElems;
      //fprintf(stderr,"current loc %p\n", currentLoc);
    }
    //fprintf(stderr,"Filling random \n");
    // copy the random fill region to the end
    for (currentSubLoc = 0; currentSubLoc < subRegionElems;) {
      int rndLoc;
      for (rndLoc = 0; rndLoc < NUM_RANDOM && currentSubLoc < subRegionElems
	     ; rndLoc++) {
	currentLoc[currentSubLoc] = rndData[rndLoc];
	currentSubLoc++;
      }
    }
    currentLoc += subRegionElems;
    //fprintf(stderr,"current loc %p\n", currentLoc);
  }
}

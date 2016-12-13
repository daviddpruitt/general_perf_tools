#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  // for uint_t
#include <stdarg.h>  // for getopts
#include <errno.h>   // for errno
#include <sys/resource.h> // for rlimit
#include "memfuncs.h"

uint64_t types[4];

size_t parseSize(char *size)
{
  size_t base = 0, totalSize = 0;
  char *multStr = size;
//base = strtoumax(size, NULL, 10);
//fprintf(stderr, "%z %s\n",base, multStr);

  base = (size_t)(size[0] - '0');
  switch (multStr[1]) {
  case 'G':
    totalSize = base * GiB; 
    printf("Using %zu Gib\n", base);
    break;
  case 'M':
    totalSize = base * MiB; 
    printf("Using %zu Gib\n", base);
    break;
  case 'K':
    totalSize = base * KiB; 
    printf("Using %zu Kib\n", base);
    break;
  default:
    printf("No valid size detected: Using %z bytes, input: %s\n", base ,multStr);
  }  
  return totalSize;
}

size_t parseOpts(int argc, char **argv)
{
  int aflag = 0;
  int bflag = 0;
  char *cvalue = NULL;
  int index;
  int c;
  size_t retval = 0;
  
  opterr = 0;
  while ((c = getopt (argc, argv, "s:")) != -1)
    switch (c)
      {
      case 's':
	cvalue = optarg;
	retval = parseSize(cvalue);
	break;
      case '?':
	if (optopt == 's')
	  fprintf (stderr, "Option -%s requires a size argument.\n", optopt);
	else if (isprint (optopt))
	  fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	else
	  fprintf (stderr,
		   "Unknown option character `\\x%x'.\n",
		   optopt);
	retval = 1;
      default:
	abort ();
      }
  /* deb_printf ("aflag = %d, bflag = %d, cvalue = %s\n", */
  /* 	  aflag, bflag, cvalue); */
  
  /* for (index = optind; index < argc; index++) */
  /*   printf ("Non-option argument %s\n", argv[index]); */
  return retval;
}

int main(int argc, char **argv)
{
  /* printf("size of array %d sizeof uint64 %d sizeof array / uint64 %d\n", */
  /* 	 sizeof(rndData),sizeof(uint64_t), sizeof(rndData)/sizeof(uint64_t)); */

  types[0] = full;
  types[1] = empty;
  types[2] = fence1;
  types[3] = fence2;


  uint64_t volatile *region;
  size_t testSize = parseOpts(argc, argv);

  struct rlimit rlim;
  getrlimit(RLIMIT_MEMLOCK, &rlim);
  printf("Lock soft limit: %ld\nLock hard limit: %ld\n", 
	 rlim.rlim_cur,
	 rlim.rlim_max);

  printf("%zu total bytes being allocated\n", testSize);
  allocMem(testSize, &region);
  fprintf(stderr, "Memory allocated\n");
  fprintf(stderr,"Filling memory\n");
  fillMem(testSize, region);
  sleep(120);
  return;
}

#include <stdio.h>
#include <stdlib.h>
#include "randomutils.h"

int
main(int argc, char **argv)
{
  int count;

  // seed test
  printf("Seed test:");
  for (count = 0; count < 10; count++) {
    sleep(2);
    printf("%zx ", timeSeed());
  }
  printf("\n");

  // random test
  printf("Random 100 to 1000 test\n");
  for (count = 0; count < 100; count++) {
    printf("%zd ", getRand(100, 1000));
  }
  printf("\n");
}


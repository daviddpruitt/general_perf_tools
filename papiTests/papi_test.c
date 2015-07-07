#include <stdlib.h>
#include <stdio.h>
#include "papi_counters.h"

int
main (int argc, char *argv[])
{
  double x = 1.001, y = 2.001;
  long long values[10];
  char **descriptions;
  int retval;
  int i;
  
  /* setup descriptions */
  descriptions = (char **)malloc(sizeof(char *) * 10);
  for (i = 0; i < 10; ++i) {
    descriptions[i] = (char *)malloc(sizeof(char) * 20);
  }
  
  /* init counters and get descriptions */
  if (init() != PAPI_OK) {
    printf("Failed to init\n");fflush(stdout);
  }
  
  if (get_desc(descriptions) != PAPI_OK) {
    printf("Failed to get descriptions\n");fflush(stdout);
  }
  
  if (reset_counters() != PAPI_OK) {
    printf("Failed to reset\n");fflush(stdout);
  }
    
  if (start_counters() != PAPI_OK) {
    printf("Failed to start_counters\n");fflush(stdout);
  }
    
  for (i = 0; i < 1000000000; ++i) {
    x += y * 1.0001;
  }  
  
  printf("x %f y %f\n", x, y);
  y = x + 1;  
  read_counters(values);
  printf("counter %lld", values[0]);
  if (stop_counters(&values[0]) != PAPI_OK) {
    printf("Failed to stop_counters\n");fflush(stdout);
  }
  
  printf("\nEvent counts:\n");
  for (i = 0; i < num_events; i++) {
    printf("%s\t",descriptions[i]);
  }
  printf("\n");
  for (i = 0; i < num_events; i++) {
    printf("%lld\t", values[i]);
  } 
  
  printf("\n\n");
  return (int) y;
}

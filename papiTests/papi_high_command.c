#include <papi.h>
#include <stdio.h>

#define NUM_FLOPS 10000
#define NUM_EVENTS 3

double c = 0.11;
void do_flops(int n)
{
 int i;
 double a = 0.5;
 double b = 6.2;

 for (i=0; i < n; i++)
 c += a * b;
 return;
}

main(int argc, char *argv[])	
{
  int i;
  for (i = 0; i < argc; ++i) {
	  printf("%s \n",argv[i]);
  }
	
  int Events[NUM_EVENTS] = {PAPI_TOT_INS, PAPI_FP_OPS, PAPI_L1_DCA};
  long_long values[NUM_EVENTS];

  /* Start counting events */
  if (PAPI_start_counters(Events, NUM_EVENTS) != PAPI_OK)
	  handle_error(1);

  if (argc > 1) {
	  system(argv[1]);
  } else {
	/* Defined in tests/do_loops.c in the PAPI source distribution */
	  do_flops(NUM_FLOPS);
  }

  /* Read the counters */
  if (PAPI_read_counters(values, NUM_EVENTS) != PAPI_OK)
	  handle_error(1);

  printf("After reading the counters: %lld %lld %lld\n",values[0], values[1], values[2]);

  if (argc > 1) {
	  system(argv[1]);
  } else {
	  do_flops(NUM_FLOPS);
  }

  /* Add the counters */
  if (PAPI_accum_counters(values, NUM_EVENTS) != PAPI_OK)
	  handle_error(1);
  printf("After reading the counters: %lld %lld %lld\n", values[0], values[1], values[2]);

  if (argc > 1) {
	  system(argv[1]);
  } else {
	  do_flops(NUM_FLOPS);
  }

  /* Stop counting events */
  if (PAPI_stop_counters(values, NUM_EVENTS) != PAPI_OK)
	  handle_error(1);

  printf("After stopping the counters: %lld %lld %lld\n", values[0], values[1], values[2]);
} 

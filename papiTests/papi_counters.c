#include "papi_counters.h"
#include <stdlib.h> 

int EventSet = PAPI_NULL;
int num_events;

struct event_struct {
  char name[32];
  int event;
  int index;
};

struct event_struct event_list[7];
 
/*
 * initializes the performance counters
 */
int init(void)
{
  int retval;
  
  /* Initialize the library */
  retval = PAPI_library_init(PAPI_VER_CURRENT);
  if (retval != PAPI_VER_CURRENT) {
    return retval;
  }  
  
/*fprintf(stdout, "PAPI Version Number\n");*/
/*fprintf(stdout, "MAJOR:    %d\n", PAPI_VERSION_MAJOR(retval));*/
/*fprintf(stdout, "MINOR:    %d\n", PAPI_VERSION_MINOR(retval));*/
/*fprintf(stdout, "REVISION: %d\n", PAPI_VERSION_REVISION(retval));*/
  
    /* enable multiplexing */
/*  retval = PAPI_multiplex_init();*/
/*  if (retval != PAPI_OK) {*/
/*    fprintf (stderr,"Can't multiplex.\n");*/
/*  } */
  
  /* Create an EventSet */
  retval = PAPI_create_eventset(&EventSet);
  if (retval != PAPI_OK) {
    return retval;
  }
  
  num_events = 0;
  /* add total cycles to event set */
/*  retval = PAPI_query_event (PAPI_TOT_CYC);*/
/*  if (retval != PAPI_OK) {*/
/*    fprintf (stderr,"No cycle counter.\n");*/
/*  } else {*/
    retval = PAPI_add_event(EventSet, PAPI_TOT_CYC);
    if (retval != PAPI_OK) {
/*      return retval;*/
    } else {
    strncpy(event_list[num_events].name, "cycles", 7);
    event_list[num_events].event = PAPI_TOT_CYC;
    event_list[num_events].index = num_events;
    ++num_events;
  }
  
  /* multiplex the event set this is done after adding an
   * event counter so that it's bound to a component first
   */
/*  retval = PAPI_set_multiplex(EventSet);*/
/*  if (retval != PAPI_OK) {*/
/*    printf("Can't multiplex event set\n");*/
/*    return retval;*/
/*  } */
  
  /* add total instructions to event set */
/*  if (PAPI_query_event (PAPI_TOT_INS) != PAPI_OK) {*/
/*    fprintf (stderr,"No instruction counter.\n");*/
/*  } else { */
    retval = PAPI_add_event(EventSet, PAPI_TOT_INS);
    if (retval != PAPI_OK) {
/*      return retval;*/
    } else {
    strncpy(event_list[num_events].name, "instr", 6);
    event_list[num_events].event = PAPI_TOT_INS;
    event_list[num_events].index = num_events;
    ++num_events;
  }
  
   /* add total stalled to event set */
/*  if (PAPI_query_event (PAPI_RES_STL) != PAPI_OK) {*/
/*    fprintf (stderr,"No cycle counter.\n");*/
/*  } else { */
    retval = PAPI_add_event(EventSet, PAPI_RES_STL);
    if (retval != PAPI_OK) {
/*      return retval;*/
    } else {
    strncpy(event_list[num_events].name, "cycles stalled", 15);
    event_list[num_events].event = PAPI_RES_STL;
    event_list[num_events].index = num_events;
    ++num_events;
  }
  
   /* add total stalled on memory to event set */
/*  if (PAPI_query_event (PAPI_MEM_RCY) != PAPI_OK) {*/
/*    fprintf (stderr,"No memory stalled cycles.\n");*/
/*  } else { */
    retval = PAPI_add_event(EventSet, PAPI_MEM_RCY);
    if (retval != PAPI_OK) {
/*      return retval;*/
    } else {
    strncpy(event_list[num_events].name, "cycles mem stalled", 19);
    event_list[num_events].event = PAPI_MEM_RCY;
    event_list[num_events].index = num_events;
    ++num_events;
  }
  
   /* add total fp stall to event set */
/*  if (PAPI_query_event (PAPI_FP_STAL) != PAPI_OK) {*/
/*    fprintf (stderr,"No fpu cycles stalled counter.\n");*/
/*  } else { */
    retval = PAPI_add_event(EventSet, PAPI_FP_STAL);
    if (retval != PAPI_OK) {
/*      return retval;*/
    } else {
    strncpy(event_list[num_events].name, "fpu cycles stalled", 19);
    event_list[num_events].event = PAPI_FP_STAL;
    event_list[num_events].index = num_events;
    ++num_events;
  }  
  
  /* add total l1 access to event set */
/*  if (PAPI_query_event (PAPI_L1_TCA) != PAPI_OK) {*/
/*    fprintf (stderr,"No L1 access counter.\n");*/
/*  } else { */
    retval = PAPI_add_event(EventSet, PAPI_L1_TCA);
    if (retval != PAPI_OK) {
      handle_error(retval);
/*      return retval;*/
    } else {
    strncpy(event_list[num_events].name, "L1 accesses", 12);
    event_list[num_events].event = PAPI_L1_TCA;
    event_list[num_events].index = num_events;
    ++num_events;
  }  
    
  /* add total l1 miss to event set */
/*  if (PAPI_query_event (PAPI_L1_TCM) != PAPI_OK) {*/
/*    fprintf (stderr,"No L1 miss counter.\n");*/
/*  } else { */
/*    retval = PAPI_add_event(EventSet, PAPI_L1_TCM);*/
/*    if (retval != PAPI_OK) {*/
/*      handle_error(retval);*/
/*      return retval;*/
/*    }*/
/*    strncpy(event_list[num_events].name, "L1 misses", 10);*/
/*    event_list[num_events].event = PAPI_L1_TCM;*/
/*    event_list[num_events].index = num_events;*/
/*    ++num_events;*/
/*  } */
  
  papi_init = 1;
  return PAPI_OK;
}

/*
 * Starts the event counters
 */
int start_counters(void) 
{
  if (!papi_init) {
    return -1;
  }
  printf("Starting counters\n");
  int retval = PAPI_start(EventSet);
  if (retval != PAPI_OK) {
    return retval;
  }
}

/*
 * Stops the counters and returns the counts
 */
int stop_counters(long long *counts) 
{
  long long *values;
  values = (long long*)malloc(sizeof(long long) * num_events);
  if (!papi_init) {
    return -1;
  }
  int retval = PAPI_stop(EventSet, values);
  int i = 0;
  for (i = 0; i < num_events; ++i) {
    counts[i] = values[i];
    printf("values %lld\n", values[i]);
  }
  if (retval != PAPI_OK) {
    return retval;
  }
  return 0;
}

/*
 *  Resets the counters
 */
int reset_counters(void)
{
  if (!papi_init) {
    return -1;
  }
  int retval = PAPI_reset(EventSet);
  if (retval != PAPI_OK) {
    return retval;
  }
  return 0;
}

/*
 * Reads the counters
 */
int read_counters(long long *counts)
{
  if (!papi_init) {
    return -1;
  }
  int retval = PAPI_read(EventSet, counts);
  if (retval != PAPI_OK) {
    return retval;
  } 
  return 0;
}

/*
 * Reads event strings of the event in the event set
 */
int get_desc(char **descriptions) 
{
  int i;
  
  if (!papi_init) {
    return -1;
  }
  
  for (i = 0; i < num_events; ++i) {
    strncpy(descriptions[i], event_list[i].name, 32);
  }
  return PAPI_OK;
}
 
int papi_init = 0;

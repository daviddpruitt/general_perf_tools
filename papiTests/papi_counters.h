#include <papi.h>
#include <stdio.h>
#include <string.h>

extern int papi_init;
extern int num_events;

int init(void);
int start_counters(void); 
int stop_counters(long long *counts);
int reset_counters(void);
int read_counters(long long *counts);
int get_desc(char **descriptions);

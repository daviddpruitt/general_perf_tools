#include <papi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#include <papi.h>
#include <stdio.h>

#include <papi.h>
#include <stdio.h>

main()
{
int EventSet = PAPI_NULL;
int retval;

/* Initialize the PAPI library */
retval = PAPI_library_init(PAPI_VER_CURRENT);

if (retval != PAPI_VER_CURRENT) {
  fprintf(stderr, "PAPI library init error!\n");
  exit(1); 
}

/* Create an EventSet */ 
if (PAPI_create_eventset(&EventSet) != PAPI_OK)
  handle_error(1);

/* Add Total Instructions Executed to our EventSet */
if (PAPI_add_event(EventSet, PAPI_L1_TCM) != PAPI_OK)
  handle_error(1);
}

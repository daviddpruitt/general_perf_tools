#include <papi.h>

int EventSet = PAPI_NULL;

int init(void)
{
  int retval;
  
  /* Initialize the library */
  retval = PAPI_library_init(PAPI_VER_CURRENT);
  if (retval != PAPI_VER_CURRENT) {
    return retval;
  }  
  
  /* Create an EventSet */
  retval = PAPI_create_eventset(&EventSet);
  if (retval != PAPI_OK) {
    return retval;
  }
  
  /* Add it to the eventset */
  retval = PAPI_add_event(EventSet, native);
  if (retval != PAPI_OK) {
    return retval;
  } 
  
  
}

int papi_init = 0;

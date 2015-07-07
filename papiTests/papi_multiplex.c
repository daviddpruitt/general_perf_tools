 
#include <papi.h>

int retval, i, EventSet = PAPI_NULL, max_to_add = 6, j = 0;
long_long *values;
const PAPI_preset_info_t *pset;

main()
{
  /* Initialize the PAPI library */
  retval = PAPI_library_init(PAPI_VER_CURRENT);
  if (retval != PAPI_VER_CURRENT)
    handle_error(1);

  /* Enable and initialize multiplex support */
  if (PAPI_multiplex_init() != PAPI_OK)
    handle_error(1);
 
 /* Create an'' EventSet ''*/
  if (PAPI_create_eventset(&EventSet) != PAPI_OK)
    handle_error(1);

 /* Convert the ''EventSet'' to a multiplexed event set */
  if (PAPI_set_multiplex(EventSet) != PAPI_OK)
    handle_error(1);

  for (i=0;i<PAPI_MAX_PRESET_EVENTS;i++)
  {
    if ((PAPI_query_event (i | PAPI_PRESET) == PAPI_OK)
    && ((i | PAPI_PRESET) != PAPI_TOT_CYC))
     {
     	if (PAPI_add_event(&EventSet, pset->event_code != PAPI_OK)
            handle_error(1);
           
        if (++j >= max_to_add)
            break;
     }
  }

  values = (long_long *)malloc(max_to_add*sizeof(long_long));
  if (values == NULL)
    handle_error(1);

  /* Start counting events */
  if (PAPI_start(EventSet) != PAPI_OK)
    handle_error(1);
}

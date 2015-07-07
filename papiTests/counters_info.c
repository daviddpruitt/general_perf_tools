 
#include <papi.h>	
#include <stdio.h>

main()
{
const PAPI_substrate_info_t *subinfo = NULL;
        	
if (PAPI_library_init(PAPI_VER_CURRENT) != PAPI_VER_CURRENT)
  exit(1);

if ((subinfo = PAPI_get_substrate_info()) == NULL)
  exit(1);

 

printf("num_preset_events: %d\n",subinfo->num_preset_events);

printf("num_native_events: %d\n",subinfo->num_native_events);

 
 }


#include <papi.h>
#include <stdio.h>
 
main()
{
int Events[2] = { PAPI_TOT_CYC, PAPI_TOT_INS };
int num_hwcntrs = 0;

/* Initialize the PAPI library and get the number of counters available */
if ((num_hwcntrs = PAPI_num_counters()) <= PAPI_OK)  
    handle_error(1);

printf("This system has %d available counters.", num_hwcntrs);

if (num_hwcntrs > 2)
    num_hwcntrs = 2;

     /* Start counting events */
     if (PAPI_start_counters(Events, num_hwcntrs) != PAPI_OK)
         handle_error(1);
}

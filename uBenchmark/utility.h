#ifndef UTILITY_H
#define UTILITY_H

#include <sys/time.h>
#include <sys/resource.h>
#include "randomaccess.h"

u64Int HPCC_starts(s64Int n);
u64Int HPCC_starts_LCG(s64Int n);
double HPL_timer_cputime(void);
double HPL_timer_walltime( void );

#endif // UTILITY_H

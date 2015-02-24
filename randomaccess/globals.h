#ifndef GLOBALS_H
#define GLOBALS_H
#include "power.h"
/* Number of updates to table (suggested: 4x number of table entries) */
#define NUPDATE (4 * TableSize)
u64Int ran[128];              /* Current random numbers */
u64Int *Table;

#endif //GLOBALS_H

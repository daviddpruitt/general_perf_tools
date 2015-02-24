/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; -*-
 *
 * This file provides utility functions for the RandomAccess benchmark suite.
 */
#include <sys/time.h>
#include <sys/resource.h>
#include "randomaccess.h"

#define HPL_rzero 0.0f


/* Utility routine to start random number generator at Nth step */
u64Int
HPCC_starts(s64Int n)
{
  int i, j;
  u64Int m2[64];
  u64Int temp, ran;

  while (n < 0) n += PERIOD;
  while (n > PERIOD) n -= PERIOD;
  if (n == 0) return 0x1;

  temp = 0x1;
  for (i=0; i<64; i++) {
    m2[i] = temp;
    temp = (temp << 1) ^ ((s64Int) temp < 0 ? POLY : 0);
    temp = (temp << 1) ^ ((s64Int) temp < 0 ? POLY : 0);
  }

  for (i=62; i>=0; i--)
    if ((n >> i) & 1)
      break;

  ran = 0x2;
  while (i > 0) {
    temp = 0;
    for (j=0; j<64; j++)
      if ((ran >> j) & 1)
        temp ^= m2[j];
    ran = temp;
    i -= 1;
    if ((n >> i) & 1)
      ran = (ran << 1) ^ ((s64Int) ran < 0 ? POLY : 0);
  }

  return ran;
}

/* Utility routine to start LCG random number generator at Nth step */
u64Int
HPCC_starts_LCG(s64Int n)
{
  u64Int mul_k, add_k, ran, un;

  mul_k = LCG_MUL64;
  add_k = LCG_ADD64;

  ran = 1;
  for (un = (u64Int)n; un; un >>= 1) {
    if (un & 1)
      ran = mul_k * ran + add_k;
    add_k *= (mul_k + 1);
    mul_k *= mul_k;
  }

  return ran;
}

double HPL_timer_cputime(void)
{
   struct rusage              ruse;
   double cputime;

   (void) getrusage( RUSAGE_SELF, &ruse );
   cputime = ((double)( ruse.ru_utime.tv_sec  ) +
           ( (double)( ruse.ru_utime.tv_usec ) / 1000000.0 ) );
/*   printf("cputime in call %f\n",cputime);*/
   return cputime;   
/*   return( (double)( ruse.ru_utime.tv_sec  ) +*/
/*           ( (double)( ruse.ru_utime.tv_usec ) / 1000000.0 ) );*/
}

double HPL_timer_walltime( void )
{
   struct timeval             tp;
   static long                start=0, startu;

   if( !start )
   {
      (void) gettimeofday( &tp, NULL );
      start  = tp.tv_sec;
      startu = tp.tv_usec;
      return( (double)( tp.tv_sec - start ) +
           ( (double)( tp.tv_usec-startu ) / 1000000.0 ) );
   }
   (void) gettimeofday( &tp, NULL );
   
/*   printf("walltime %f\n",(double)( tp.tv_sec - start ) +*/
/*           ( (double)( tp.tv_usec-startu ) / 1000000.0 ) );*/
   return( (double)( tp.tv_sec - start ) +
           ( (double)( tp.tv_usec-startu ) / 1000000.0 ) );
}

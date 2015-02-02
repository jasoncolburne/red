/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: time.c 21 2010-04-28 17:38:52Z jason $
 */

#include "he_time.h"
#include "he_return.h"

#include <time.h>
#include <sys/time.h>

#if defined (__APPLE__)
#  include <mach/mach_time.h>
#endif

int
redSleep(
    RedTime duration
    )
{
  int rc = RED_SUCCESS;

  struct timespec rq, rm;

  rq.tv_nsec = duration % RED_TIME_FROM_SECONDS(1);
  rq.tv_sec  = (duration - rq.tv_nsec) / RED_TIME_FROM_SECONDS(1);

  do {
    rc = nanosleep( &rq, &rm );
    rq = rm;
    /* TODO: need to check for ENOSYS here - should become apparent in tests */
  } while(rc);

  return RED_SUCCESS;
}

int
redTimeNow(
    RedTime* time
    )
{
  int rc = RED_SUCCESS;

  struct timeval tp;

  rc = gettimeofday( &tp, NULL );
  
  *time =  RED_TIME_FROM_SECONDS( tp.tv_sec );
  *time += RED_TIME_FROM_USEC( tp.tv_usec );

  return rc;
}


int
redTimeNowPrecise(
    RedTime* time
    )
{
#if defined (__APPLE__)
  static mach_timebase_info_data_t info     = { 0, 0 };
  static int                       executed = 0;

  if (!executed) {
    mach_timebase_info( &info );
    executed = 1;
  }

  if (!(info.denom) || info.numer == info.denom)
    *time = (RedTime)mach_absolute_time();
  else
    *time = (RedTime)(mach_absolute_time() * info.numer / info.denom);
#elif defined (__linux__)
  struct timespec ts;

  if (clock_gettime( CLOCK_REALTIME, &ts ) != 0) {
    /* TODO: errno */
    return 0x666;
  }

  *time = RED_TIME_FROM_SECONDS( ts.tv_sec ) + RED_TIME_FROM_NSEC( ts.tv_nsec );
#endif

  return RED_SUCCESS;
}

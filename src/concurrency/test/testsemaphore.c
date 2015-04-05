/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: testsemaphore.c 21 2010-04-28 17:38:52Z jason $
 */

#include "main.h"

#include "red_semaphore.h"
#include "red_time.h"

RED_TEST_UNIT_IDENTIFY( "testsemaphore.c" );

#include <errno.h>
#include <stdio.h>
#include <string.h>

int
testSemaphore(
    RedStream  conErr,
    RedContext rCtx
    )
{
  int rc = RED_SUCCESS;

  RedSemaphore semaphore = NULL;

  RedTime now, then;

  RED_TEST_CALL( redSemaphoreCreate( &semaphore, 0, rCtx ), RED_SUCCESS,
                 "RedSemaphoreCreate( )" );
  
  RED_TEST_CALL_SILENTLY( redTimeNowPrecise( &then ), RED_SUCCESS,
                          "RedTimePrecise( )" );

  RED_TEST_CALL( redSemaphoreTakeTimed( semaphore, RED_TIME_FROM_MSEC( 50 ) ),
                 RED_FAIL_TIMED_OUT,
                 "RedSemaphoreTakeTimed( 50ms ), empty semaphore" );

  RED_TEST_CALL_SILENTLY( redTimeNowPrecise( &now ), RED_SUCCESS,
                          "RedTimePrecise( )" );

  RED_ASSERT( now - then > RED_TIME_FROM_MSEC( 50 ), "took > 50ms");

  RED_TEST_CALL( redSemaphoreDestroy( &semaphore ), RED_SUCCESS,
                 "RedSemaphoreDestroy( )" );

end:
  return rc;
}


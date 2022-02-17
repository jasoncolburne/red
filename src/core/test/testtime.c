/* Copyright (C) 2010-2022, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: testtime.c 19 2010-04-22 08:27:08Z jason $
 */

#include "main.h"
#include "red_time.h"

RED_TEST_UNIT_IDENTIFY( "time.c" );

RED_TEST_UNIT_IMPLEMENTATION_BEGIN( testTime );

  int i  = 0;

  RedTime then, now, delay;
  RedTime overhead = 0;
  
  RED_TEST_CALL( redTimeNowPrecise( &then ), RED_SUCCESS,
                 "redTimeNowPrecise( )" );

  for (i = 0; i < 4; i++) {
    redStreamPrint( conErr, 0, "sleeping for %d seconds\n", i );
    RED_TEST_CALL_SILENTLY( redSleep( RED_TIME_FROM_SECONDS(i) ), RED_SUCCESS,
                            "redSleep( )" );
  }

  RED_TEST_CALL_SILENTLY( redTimeNowPrecise( &now ), RED_SUCCESS,
                          "redTimeNowPrecise( )" );

  redStreamPrint( conErr, 0, "time elapsed: %lld nsec\n\n",
                  RED_TIME_AS_NSEC(now - then) );

  RED_TEST_CALL( redTimeNow( &now ), RED_SUCCESS, "redTimeNow( )" );

  redStreamPrint( conErr, 0, "days since Jan 1, 1970: %lld\n\n",
                  RED_TIME_AS_DAYS(now) );

  for (delay = 1; delay < 0x10000; delay *= 2) {
    RedTime elapsed;

    redTimeNowPrecise( &then );
    redSleep( RED_TIME_FROM_USEC(delay) );
    redSleep( RED_TIME_FROM_USEC(delay) );
    redSleep( RED_TIME_FROM_USEC(delay) );
    redSleep( RED_TIME_FROM_USEC(delay) );
    redSleep( RED_TIME_FROM_USEC(delay) );
    redSleep( RED_TIME_FROM_USEC(delay) );
    redSleep( RED_TIME_FROM_USEC(delay) );
    redSleep( RED_TIME_FROM_USEC(delay) );
    redSleep( RED_TIME_FROM_USEC(delay) );
    redSleep( RED_TIME_FROM_USEC(delay) );
    redTimeNowPrecise( &now );

    elapsed = (now - then) / 10;

    overhead += elapsed - RED_TIME_FROM_USEC(delay);

    redStreamPrint( conErr, 0, "delay of %lld usec took %lld usec\n",
                    delay, RED_TIME_AS_USEC( elapsed ) );
  }

  redStreamPrint( conErr, 0, "average overhead for sleep: %lld usec",
                  RED_TIME_AS_USEC( overhead / 16 ) );

RED_TEST_UNIT_IMPLEMENTATION_CLEANUP();

RED_TEST_UNIT_IMPLEMENTATION_END();

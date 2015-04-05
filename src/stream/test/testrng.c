/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: testrng.c 18 2010-04-17 05:57:03Z jason $
 */

#include "main.h"

#define TEST_ENTROPY_BYTES 16

RED_TEST_UNIT_IDENTIFY( "rng.c" );

RED_TEST_UNIT_IMPLEMENTATION_BEGIN( testRNG );

  RedStream  rng    = NULL;

  red_u8* entropy = NULL;
  size_t  size    = TEST_ENTROPY_BYTES;

  int i = 0;

  RED_TEST_CALL( redStreamCreateRNG( &rng, RED_STREAM_DEFAULT, rCtx ),
                 RED_SUCCESS, "redStreamCreateRNG( )" );

  RED_TEST_CALL_SILENTLY( redMalloc( rCtx, (void**)&entropy, size ),
                          RED_SUCCESS, "redMalloc( )" );

  RED_TEST_CALL( redStreamTake( rng, entropy, &size ), RED_SUCCESS,
                 "redStreamTake( )" );

  RED_TEST_CALL_SILENTLY( redStreamPrint( conErr, 10, "entropy: " ),
                          RED_SUCCESS, "redStreamPrint( )" );

  for (i = 0; i < size; i++) {
    RED_TEST_CALL_SILENTLY( redStreamPrint( conErr, 3, "%02x", entropy[i] ),
                            RED_SUCCESS, "redStreamPrint( )" );
  }

  RED_TEST_CALL_SILENTLY( redStreamPrint( conErr, 2, "\n" ), RED_SUCCESS,
                          "redStreamPrint( )" );
  
RED_TEST_UNIT_IMPLEMENTATION_CLEANUP();

  redFree( rCtx, (void**)&entropy, 0 );
  redStreamDestroy( &rng );

RED_TEST_UNIT_IMPLEMENTATION_END();

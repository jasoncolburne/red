/* Copyright (C) 2010-2022, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: testconsole.c 18 2010-04-17 05:57:03Z jason $
 */

#include "main.h"

RED_TEST_UNIT_IDENTIFY( "console.c" );

RED_TEST_UNIT_IMPLEMENTATION_BEGIN( testConsole );

  RedStream  stream = NULL;

  RED_TEST_CALL( redStreamCreateConsoleError( &stream, rCtx ),
                 RED_SUCCESS, "redStreamCreateConsoleError( )" );

  RED_TEST_CALL_SILENTLY( redStreamPrint( stream, 20,
                            "truncated at %d characters", 19 ),
                          RED_SUCCESS, "redStreamPrint( )" );

  RED_TEST_CALL_SILENTLY( redStreamPrint( stream, 20, "\n" ), RED_SUCCESS,
                          "redStreamPrint( )" );

  RED_TEST_CALL( redStreamDestroy( &stream ), RED_SUCCESS,
                 "redStreamDestroy( )" );

RED_TEST_UNIT_IMPLEMENTATION_CLEANUP();

RED_TEST_UNIT_IMPLEMENTATION_END();

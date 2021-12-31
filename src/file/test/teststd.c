/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: teststd.c 18 2010-04-17 05:57:03Z jason $
 */

#include "main.h"

#include "red_file.h"

RED_TEST_UNIT_IDENTIFY( "std.c" );

RED_TEST_UNIT_IMPLEMENTATION_BEGIN( testStd );

  char str[] = "Hi, I am stdout!\n";

  RedFile    stdErr = NULL;
  RedFile    stdOut = NULL;

  RED_TEST_CALL( redFileOpenStdout( &stdOut, rCtx ), RED_SUCCESS,
                 "redFileOpenStdout( )" );

  RED_TEST_CALL_SILENTLY( redFileWrite( stdOut, str, sizeof(str) ), RED_SUCCESS,
                          "redFileWrite( )" );

  RED_TEST_CALL( redFileClose( &stdOut ), RED_SUCCESS, "redFileClose( )" );

  str[12] = 'e';
  str[13] = 'r';
  str[14] = 'r';

  RED_TEST_CALL( redFileOpenStderr( &stdErr, rCtx ), RED_SUCCESS,
                 "redFileOpenStderr( )" );

  RED_TEST_CALL_SILENTLY( redFileWrite( stdErr, str, sizeof(str) ), RED_SUCCESS,
                          "redFileWrite( )" );

  RED_TEST_CALL_SILENTLY( redFileClose( &stdErr ), RED_SUCCESS,
                          "redFileClose( )" );

RED_TEST_UNIT_IMPLEMENTATION_CLEANUP();

RED_TEST_UNIT_IMPLEMENTATION_END();

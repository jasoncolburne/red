/* Copyright (C) 2010-2022, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: testmain.c 18 2010-04-17 05:57:03Z jason $
 */

#include "main.h"

RED_TEST_MAIN_BEGIN();

  RED_TEST_UNIT_CALL( testConsole, "RedStreamConsole" );
  RED_TEST_UNIT_CALL(    testFile, "RedStreamFile" );
  RED_TEST_UNIT_CALL(     testRNG, "RedStreamRNG" );

RED_TEST_MAIN_END();

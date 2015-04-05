/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: testmain.c 18 2010-04-17 05:57:03Z jason $
 */

#include "main.h"

RED_TEST_MAIN_BEGIN();

  RED_TEST_UNIT_CALL( testContext, "RedContext" );
  RED_TEST_UNIT_CALL(    testTime, "RedTime" );

RED_TEST_MAIN_END();

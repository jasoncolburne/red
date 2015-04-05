/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: testmain.c 19 2010-04-22 08:27:08Z jason $
 */

#include "main.h"

RED_TEST_MAIN_BEGIN();

  RED_TEST_UNIT_CALL( testQueue, "RedQueue" );
  RED_TEST_UNIT_CALL( testStack, "RedStack" );

RED_TEST_MAIN_END();

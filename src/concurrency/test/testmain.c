/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: testmain.c 21 2010-04-28 17:38:52Z jason $
 */

#include "main.h"

RED_TEST_MAIN_BEGIN();

  RED_TEST_UNIT_CALL(    testThread, "RedThread" );
  RED_TEST_UNIT_CALL(      testLock, "RedLock" );
  RED_TEST_UNIT_CALL(    testRWLock, "RedRWLock" );
  RED_TEST_UNIT_CALL( testCondition, "RedCondition" );
  RED_TEST_UNIT_CALL( testSemaphore, "RedSemaphore" );
#if 0 /* TODO: make atomic stuff work in linux */
  RED_TEST_UNIT_CALL(    testAtomic, "RedAtomic" );
#endif

RED_TEST_MAIN_END();

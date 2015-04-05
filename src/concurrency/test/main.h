/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: main.h 21 2010-04-28 17:38:52Z jason $
 */

#ifndef __MAIN_H__
#define __MAIN_H__


#include "red_test.h"

#include "red_condition.h"
#include "red_lock.h"
#include "red_rwlock.h"
#include "red_thread.h"

RED_TEST_UNIT_PROTOTYPE( testThread );
RED_TEST_UNIT_PROTOTYPE( testLock );
RED_TEST_UNIT_PROTOTYPE( testRWLock );
RED_TEST_UNIT_PROTOTYPE( testCondition );
RED_TEST_UNIT_PROTOTYPE( testSemaphore );
RED_TEST_UNIT_PROTOTYPE( testAtomic );

#endif /* __MAIN_H__ */

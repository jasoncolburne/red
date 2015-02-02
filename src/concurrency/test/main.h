/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: main.h 21 2010-04-28 17:38:52Z jason $
 */

#ifndef __MAIN_H__
#define __MAIN_H__


#include "he_test.h"

#include "he_condition.h"
#include "he_lock.h"
#include "he_rwlock.h"
#include "he_thread.h"

RED_TEST_UNIT_PROTOTYPE( testThread );
RED_TEST_UNIT_PROTOTYPE( testLock );
RED_TEST_UNIT_PROTOTYPE( testRWLock );
RED_TEST_UNIT_PROTOTYPE( testCondition );
RED_TEST_UNIT_PROTOTYPE( testSemaphore );
RED_TEST_UNIT_PROTOTYPE( testAtomic );

#endif /* __MAIN_H__ */

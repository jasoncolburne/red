/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_lock.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef __RED_LOCK_H__
#define __RED_LOCK_H__


#include "red_context.h"


#ifdef __cplusplus
extern "C" {
#endif



struct _RedLock;
typedef struct _RedLock* RedLock;

extern
int
redLockCreate(
    RedLock*   lock,
    RedContext rCtx
    );

extern
int
redLockDestroy(
    RedLock*   lock
    );

extern
int
redLockTake(
    RedLock lock
    );

extern
int
redLockGive(
    RedLock lock
    );




#ifdef __cplusplus
}
#endif


#endif /* __RED_LOCK_H__ */

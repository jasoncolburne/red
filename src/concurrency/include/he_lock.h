/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_lock.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef __HE_LOCK_H__
#define __HE_LOCK_H__


#include "he_context.h"


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


#endif /* __HE_LOCK_H__ */

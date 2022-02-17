/* Copyright (C) 2010-2022, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_spinlock.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef __RED_SPINLOCK_H__
#define __RED_SPINLOCK_H__


#include "red_context.h"


#ifdef __cplusplus
extern "C" {
#endif


struct _RedSpinlock;
typedef struct _RedSpinlock* RedSpinlock;

extern
int
redSpinlockCreate(
    RedSpinlock* lock,
    RedContext   rCtx
    );

extern
int
redSpinlockDestroy(
    RedSpinlock* lock
    );

extern
int
redSpinlockTake(
    RedSpinlock lock
    );

extern
int
redSpinlockGive(
    RedSpinlock lock
    );


#ifdef __cplusplus
}
#endif


#endif /* __RED_SPINLOCK_H__ */

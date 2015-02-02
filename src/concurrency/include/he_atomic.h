/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_atomic.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef __HE_ATOMIC_H__
#define __HE_ATOMIC_H__


#include "he_type.h"


#ifdef __cplusplus
extern "C" {
#endif


extern
int
redAtomicAdd32(
    red_i32           toAdd,
    volatile red_i32* value,
    red_i32*          newValue
    );

extern
int
redAtomicIncrement32(
    volatile red_i32* value,
    red_i32*          newValue
    );

extern
int
redAtomicDecrement32(
    volatile red_i32* value,
    red_i32*          newValue
    );

/* returns RED_SUCCESS if swap occurred, RED_FAIL_ATOMIC_COMPARISON otherwise */
extern
int
redAtomicCompareAndSwap32(
    red_i32           oldValue,
    red_i32           newValue,
    volatile red_i32* value
    );

extern
int
redAtomicAdd64(
    red_i64           toAdd,
    volatile red_i64* value,
    red_i64*          newValue
    );

extern
int
redAtomicIncrement64(
    volatile red_i64* value,
    red_i64*          newValue
    );

extern
int
redAtomicDecrement64(
    volatile red_i64* value,
    red_i64*          newValue
    );

extern
int
redAtomicCompareAndSwap64(
    red_i64           oldValue,
    red_i64           newValue,
    volatile red_i64* value
    );


extern
int
redAtomicCompareAndSwapPointer(
    void*           oldValue,
    void*           newValue,
    void* volatile* value
    );


#ifdef __cplusplus
}
#endif


#endif /* __HE_ATOMIC_H__ */

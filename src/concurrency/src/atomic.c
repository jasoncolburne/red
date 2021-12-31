/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: atomic.c 20 2010-04-23 01:46:23Z jason $
 */

#include "red_atomic.h"
#include "red_return.h"

#if defined (__APPLE__)
#  include <libkern/OSAtomic.h>
#elif defined (__linux__)
/* TODO: implement for linux */
#else
#  error "Target not supported"
#endif

int
redAtomicAdd32(
    red_i32           toAdd,
    volatile red_i32* value,
    red_i32*          newValue
    )
{
  if (!value)
    return RED_ERR_NULL_POINTER;

#if defined (__APPLE__)
  if (newValue)
    *newValue = (red_i32)OSAtomicAdd32Barrier( (int32_t)toAdd,
                                               (int32_t*)value );
  else
    OSAtomicAdd32Barrier( (int32_t)toAdd, (int32_t*)value );
#elif defined (__linux__)

#endif

  return RED_SUCCESS;
}

int
redAtomicIncrement32(
    volatile red_i32* value,
    red_i32*          newValue
    )
{
  if (!value)
    return RED_ERR_NULL_POINTER;

#if defined (__APPLE__)
  if (newValue)
    *newValue = (red_i32)OSAtomicIncrement32Barrier( (int32_t*)value );
  else
    OSAtomicIncrement32Barrier( (int32_t*)value );
#elif defined (__linux__)

#endif

  return RED_SUCCESS;
}

int
redAtomicDecrement32(
    volatile red_i32* value,
    red_i32*          newValue
    )
{
  if (!value)
    return RED_ERR_NULL_POINTER;

#if defined (__APPLE__)
  if (newValue)
    *newValue = (red_i32)OSAtomicDecrement32Barrier( (int32_t*)value );
  else
    OSAtomicDecrement32Barrier( (int32_t*)value );
#elif defined (__linux__)

#endif

  return RED_SUCCESS;
}

int
redAtomicCompareAndSwap32(
    red_i32           oldValue,
    red_i32           newValue,
    volatile red_i32* value
    )
{
  if (!value)
    return RED_ERR_NULL_POINTER;

#if defined (__APPLE__)
  if (OSAtomicCompareAndSwap32Barrier( oldValue, newValue, (int32_t*)value ))
    return RED_SUCCESS;
#elif defined (__linux__)

#endif

  return RED_FAIL_ATOMIC_COMPARISON;
}



int
redAtomicAdd64(
    red_i64           toAdd,
    volatile red_i64* value,
    red_i64*          newValue
    )
{
  if (!value)
    return RED_ERR_NULL_POINTER;

#if defined (__APPLE__)
  if (newValue)
    *newValue = (red_i64)OSAtomicAdd64Barrier( (int64_t)toAdd,
                                               (int64_t*)value );
  else
    OSAtomicAdd64Barrier( (int64_t)toAdd, (int64_t*)value );
#elif defined (__linux__)

#endif

  return RED_SUCCESS;
}

int
redAtomicIncrement64(
    volatile red_i64* value,
    red_i64*          newValue
    )
{
  if (!value)
    return RED_ERR_NULL_POINTER;

#if defined (__APPLE__)
  if (newValue)
    *newValue = (red_i64)OSAtomicIncrement64Barrier( (int64_t*)value );
  else
    OSAtomicIncrement64Barrier( (int64_t*)value );
#elif defined (__linux__)

#endif

  return RED_SUCCESS;
}

int
redAtomicDecrement64(
    volatile red_i64* value,
    red_i64*          newValue
    )
{
  if (!value)
    return RED_ERR_NULL_POINTER;

#if defined (__APPLE__)
  if (newValue)
    *newValue = (red_i64)OSAtomicDecrement64Barrier( (int64_t*)value );
  else
    OSAtomicDecrement64Barrier( (int64_t*)value );
#elif defined (__linux__)

#endif

  return RED_SUCCESS;
}

int
redAtomicCompareAndSwap64(
    red_i64           oldValue,
    red_i64           newValue,
    volatile red_i64* value
    )
{
  if (!value)
    return RED_ERR_NULL_POINTER;

#if defined (__APPLE__)
  if (OSAtomicCompareAndSwap64Barrier( oldValue, newValue, (int64_t*)value ))
    return RED_SUCCESS;
#elif defined (__linux__)

#endif

  return RED_FAIL_ATOMIC_COMPARISON;
}


int
redAtomicCompareAndSwapPointer(
    void*           oldValue,
    void*           newValue,
    void* volatile* value
    )
{
  if (!value)
    return RED_ERR_NULL_POINTER;

#if defined (__APPLE__)
  if (OSAtomicCompareAndSwapPtrBarrier( oldValue, newValue, value ))
    return RED_SUCCESS;
#elif defined (__linux__)

#endif

  return RED_FAIL_ATOMIC_COMPARISON;
}

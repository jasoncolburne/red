/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_spinlock.h 20 2010-04-23 01:46:23Z jason $
 */

#ifndef ___RED_SPINLOCK_H__
#define ___RED_SPINLOCK_H__


#include "red_spinlock.h"

#if defined (__APPLE__)
#  include <libkern/OSAtomic.h>
#elif defined (__linux__)
#  include <pthread.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


struct _RedSpinlock {
#if defined (__APPLE__)
  OSSpinLock lock;
#elif defined (__linux__)
  pthread_spinlock_t lock;
#endif

  RedContext rCtx;
};


#ifdef __cplusplus
}
#endif


#endif /* ___RED_SPINLOCK_H__ */

/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_lock.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef ___HE_LOCK_H__
#define ___HE_LOCK_H__

#include "he_context.h"

#include <pthread.h>

struct _RedLock {
  pthread_mutex_t lock;

  RedContext    rCtx;
};

#endif /*___HE_LOCK_H__ */

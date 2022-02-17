/* Copyright (C) 2010-2022, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_rwlock.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef ___RED_RWLOCK_H__
#define ___RED_RWLOCK_H__

#include "red_context.h"

#include <pthread.h>

struct _RedRWLock {
  pthread_rwlock_t lock;

  RedContext       rCtx;
};

#endif /*___RED_RWLOCK_H__ */

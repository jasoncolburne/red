/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_semaphore.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef ___HE_SEMAPHORE_H__
#define ___HE_SEMAPHORE_H__


#include "he_semaphore.h"

#if defined (__APPLE__)
#  include <mach/task.h>
#  include <mach/semaphore.h>
#elif defined (__linux__)
#  include <semaphore.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct _RedSemaphore {

#if defined (__APPLE__)
  semaphore_t semaphore;
  task_t      owner;
#elif defined (__linux__)
  sem_t       semaphore;
#endif

  RedContext  rCtx;
};

#ifdef __cplusplus
}
#endif


#endif /* ___HE_SEMAPHORE_H__ */

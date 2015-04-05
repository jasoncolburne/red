/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_condition.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef ___HE_CONDITION_H__
#define ___HE_CONDITION_H__

#include "he_context.h"

#include <pthread.h>

struct _RedCondition {
  pthread_cond_t  cond;
  pthread_mutex_t mutex;

  RedContext      rCtx;
};

#endif /*___HE_CONDITION_H__ */

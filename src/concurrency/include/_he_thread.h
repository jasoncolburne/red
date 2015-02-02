/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_thread.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef ___HE_THREAD_H__
#define ___HE_THREAD_H__

#include "he_context.h"

#include <pthread.h>

struct _RedThreadAttributes {
  pthread_attr_t attr;

  RedContext rCtx;
};

struct _RedThread {
  pthread_t     thr;

  void*         data;
  redfuncThread function;

  int           rc;
  volatile int  done;

  RedContext    rCtx;
};

#endif /*___HE_THREAD_H__ */

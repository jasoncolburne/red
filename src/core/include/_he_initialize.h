/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_initialize.h 19 2010-04-22 08:27:08Z jason $
 */

#ifndef ___HE_INITIALIZE_H__
#define ___HE_INITIALIZE_H__


#include "he_context.h"
#include "he_queue.h"
#include "he_thread.h"


#ifdef __cplusplus
extern "C" {
#endif


struct _RedGlobalContext {
  RedQueue  traceQueue;
  RedThread thread;

  volatile int done;

  /* thread local storage? */

  RedContext rCtx;
};
typedef struct _RedGlobalContext* RedGlobalContext;



#ifdef __cplusplus
}
#endif


#endif /* ___HE_INITIALIZE_H__ */

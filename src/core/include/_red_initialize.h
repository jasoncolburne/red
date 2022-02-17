/* Copyright (C) 2010-2022, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_initialize.h 19 2010-04-22 08:27:08Z jason $
 */

#ifndef ___RED_INITIALIZE_H__
#define ___RED_INITIALIZE_H__


#include "red_context.h"
#include "red_queue.h"
#include "red_thread.h"


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


#endif /* ___RED_INITIALIZE_H__ */

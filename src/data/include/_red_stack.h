/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_stack.h 19 2010-04-22 08:27:08Z jason $
 */

#ifndef ___RED_STACK_H__
#define ___RED_STACK_H__

#include "red_stack.h"

#include "red_spinlock.h"
#include "red_semaphore.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int  (*redfuncStackCreateHook)( RedStack );
typedef int (*redfuncStackDestroyHook)( RedStack );
typedef int        (*redfuncStackGrow)( RedStack, size_t );
typedef int       (*redfuncStackDepth)( RedStack, size_t* );
typedef int        (*redfuncStackGive)( RedStack, void* );
typedef int        (*redfuncStackTake)( RedStack, void** );
typedef int   (*redfuncStackGiveTimed)( RedStack, void*, RedTime );
typedef int   (*redfuncStackTakeTimed)( RedStack, void**, RedTime );

struct _RedStackFunctions {
  redfuncStackCreateHook  rfCreateHook;
  redfuncStackDestroyHook rfDestroyHook;
  redfuncStackGrow        rfGrow;
  redfuncStackDepth       rfDepth;
  redfuncStackGive        rfGive;
  redfuncStackTake        rfTake;
  redfuncStackGiveTimed   rfGiveTimed;
  redfuncStackTakeTimed   rfTakeTimed;
};
typedef struct _RedStackFunctions* RedStackFunctions;

struct _RedStack {
  RedSpinlock  lock;

  RedSemaphore takeSemaphore;
  RedSemaphore giveSemaphore;

  void** bottom;
  void** top;

  size_t depthLimit;
  size_t maximumDepthLimit;

  redfuncStackDestroyHook rfDestroyHook;
  redfuncStackGrow        rfGrow;
  redfuncStackDepth       rfDepth;
  redfuncStackGive        rfGive;
  redfuncStackTake        rfTake;
  redfuncStackGiveTimed   rfGiveTimed;
  redfuncStackTakeTimed   rfTakeTimed;

  RedContext rCtx;
};

int
redStackCreate(
    RedStack*             stack,
    size_t                initialDepthLimit,
    size_t                maximumDepthLimit,
    RedStackFunctions     implementation,
    RedContext            rCtx
    );

int
redStackGrow(
    RedStack stack,
    size_t   limit
    );

int
redStackGrowNonblocking(
    RedStack stack,
    size_t   limit
    );

int
redStackTakeNonblocking(
    RedStack stack,
    void**   data
    );

int
redStackGiveNonblocking(
    RedStack stack,
    void*    data
    );

int
redStackDepthNonblocking(
    RedStack stack,
    size_t*  depth
    );

#ifdef __cplusplus
}
#endif


#endif /* ___RED_STACK_H__ */

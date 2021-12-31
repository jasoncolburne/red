/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_queue.h 19 2010-04-22 08:27:08Z jason $
 */

#ifndef ___RED_QUEUE_H__
#define ___RED_QUEUE_H__

#include "red_queue.h"

#include "red_spinlock.h"
#include "red_semaphore.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int  (*redfuncQueueCreateHook)( RedQueue );
typedef int (*redfuncQueueDestroyHook)( RedQueue );
typedef int        (*redfuncQueueGrow)( RedQueue, size_t );
typedef int      (*redfuncQueueLength)( RedQueue, size_t* );
typedef int        (*redfuncQueueGive)( RedQueue, void* );
typedef int        (*redfuncQueueTake)( RedQueue, void** );
typedef int   (*redfuncQueueGiveTimed)( RedQueue, void*, RedTime );
typedef int   (*redfuncQueueTakeTimed)( RedQueue, void**, RedTime );

struct _RedQueueFunctions {
  redfuncQueueCreateHook  rfCreateHook;
  redfuncQueueDestroyHook rfDestroyHook;
  redfuncQueueGrow        rfGrow;
  redfuncQueueLength      rfLength;
  redfuncQueueGive        rfGive;
  redfuncQueueTake        rfTake;
  redfuncQueueGiveTimed   rfGiveTimed;
  redfuncQueueTakeTimed   rfTakeTimed;
};
typedef struct _RedQueueFunctions* RedQueueFunctions;

struct _RedQueue {
  RedSpinlock  lock;

  RedSemaphore takeSemaphore;
  RedSemaphore giveSemaphore;

  void** queue;
  void** head;
  void** tail;

  size_t lengthLimit;
  size_t maximumLengthLimit;

  /* if head == tail, this indicates full or empty */
  int full;

  redfuncQueueDestroyHook rfDestroyHook;
  redfuncQueueGrow        rfGrow;
  redfuncQueueLength      rfLength;
  redfuncQueueGive        rfGive;
  redfuncQueueTake        rfTake;
  redfuncQueueGiveTimed   rfGiveTimed;
  redfuncQueueTakeTimed   rfTakeTimed;

  RedContext rCtx;
};

int
redQueueCreate(
    RedQueue*             queue,
    size_t                initialLengthLimit,
    size_t                maximumLengthLimit,
    RedQueueFunctions     implementation,
    RedContext            rCtx
    );

int
redQueueGrow(
    RedQueue queue,
    size_t   limit
    );

int
redQueueGrowNonblocking(
    RedQueue queue,
    size_t   limit
    );

int
redQueueTakeNonblocking(
    RedQueue queue,
    void**   data
    );

int
redQueueGiveNonblocking(
    RedQueue queue,
    void*    data
    );

int
redQueueLengthNonblocking(
    RedQueue queue,
    size_t*  length
    );


#ifdef __cplusplus
}
#endif


#endif /* ___RED_QUEUE_H__ */

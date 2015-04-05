/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: queuenonblocking.c 19 2010-04-22 08:27:08Z jason $
 */

#include "_he_queue.h"

#include "he_memory.h"
#include "he_return.h"

static
int
createHook(
    RedQueue queue
    )
{
  if (!queue)
    return RED_ERR_NULL_CONTEXT;
  
  return redSpinlockCreate( &(queue->lock), queue->rCtx );
}

static
int
destroyHook(
    RedQueue queue
    )
{
  if (!queue)
    return RED_ERR_NULL_CONTEXT;

  return redSpinlockDestroy( &(queue->lock) );
}


int
redQueueCreateNonblocking(
    RedQueue*  queue,
    size_t     initialLengthLimit,
    size_t     maximumLengthLimit,
    RedContext rCtx
    )
{
  struct _RedQueueFunctions f = { 0 };

  f.rfCreateHook  = createHook;
  f.rfDestroyHook = destroyHook;
  f.rfLength      = redQueueLengthNonblocking;
  f.rfGrow        = redQueueGrowNonblocking;
  f.rfGive        = redQueueGiveNonblocking;
  f.rfTake        = redQueueTakeNonblocking;
  f.rfTakeTimed   = NULL;
  f.rfGiveTimed   = NULL;

  return redQueueCreate( queue, initialLengthLimit, maximumLengthLimit, &f,
                         rCtx );
}

int
redQueueGrowNonblocking(
    RedQueue queue,
    size_t   limit
    )
{
  int rc = RED_SUCCESS;

  void* d = NULL;
  void* s = NULL;

  /* TODO: this is a confusing return code */
  if (limit > queue->maximumLengthLimit && queue->maximumLengthLimit)
    return RED_ERR_INVALID_ARGUMENT;

  /* we'll not throw an error when they ask to shrink beause it likely means
   * they were in a race condition and is not a programmer error
   * TODO: note this in documentation */
  if (limit <= queue->lengthLimit)
    goto end;

  rc = redMalloc( queue->rCtx, &d, sizeof(void*) * limit );
  if (rc) goto end;

  if (queue->queue) {
    char* p = (char*)queue->queue;

    size_t hOff   = (size_t)queue->head - (size_t)queue->queue;
    size_t tOff   = (size_t)queue->tail - (size_t)queue->queue;

    size_t length;

    if (tOff > hOff) {
      length = tOff - hOff;

      rc = redMemcpy( queue->rCtx, d, p + hOff, length );
      if (rc) goto end;

      queue->tail = (void**)((size_t)d + length);
    }
    else if (tOff < hOff || queue->full) {
      length = (size_t)queue->queue + queue->lengthLimit * sizeof(void*) -
               (size_t)queue->head;

      /* copy first half of queue */
      rc = redMemcpy( queue->rCtx, d, p + hOff, length );
      if (rc) goto end;

      /* copy second half of queue */
      p = (char*)d + length;
      rc = redMemcpy( queue->rCtx, p, queue->queue, tOff );
      if (rc) goto end;

      queue->tail  = (void**)((size_t)d + queue->lengthLimit * sizeof(void*));
    }
    else {
      queue->tail = (void**)d;
    }

    s = queue->queue;
  }
  else {
    queue->tail = (void**)d;
  }
  
  queue->queue = (void**)d;
  queue->head  = (void**)d;
  queue->full  = 0;
  queue->lengthLimit = limit;
  d = NULL;

end:
  redFree( queue->rCtx, &d, 0 );
  redFree( queue->rCtx, &s, 0 );

  return rc;
}

int
redQueueGiveNonblocking(
    RedQueue queue,
    void*    data
    )
{
  int rc = RED_SUCCESS;

  if (!queue)
    return RED_ERR_NULL_CONTEXT;

  rc = redSpinlockTake( queue->lock );
  if (rc) return rc;

  /* unlocked references to head are safe since it can't move backwards */
  if (queue->full && queue->head == queue->tail) {
    if (queue->lengthLimit >= queue->maximumLengthLimit) {
      rc = RED_FAIL_COLLECTION_FULL;
      goto end;
    }

    /* double up if we can or expand to max if not */
    if (queue->maximumLengthLimit / 2 > queue->lengthLimit)
      rc = redQueueGrow( queue, queue->lengthLimit * 2 );
    else
      rc = redQueueGrow( queue, queue->maximumLengthLimit );
    if (rc) goto end;
  }

  *(queue->tail) = data;
  (queue->tail)++;

  if ((size_t)queue->tail - (size_t)queue->queue >=
      queue->lengthLimit * sizeof(void*))
    queue->tail = queue->queue;

  if (queue->tail == queue->head)
    queue->full = 1;

end:
  redSpinlockGive( queue->lock );

  return rc;
}

int
redQueueTakeNonblocking(
    RedQueue queue,
    void**   data
    )
{
  int rc = RED_SUCCESS;

  if (!queue)
    return RED_ERR_NULL_CONTEXT;
  if (!data)
    return RED_ERR_NULL_POINTER;

  rc = redSpinlockTake( queue->lock );
  if (rc) return rc;

  if (!(queue->full) && queue->tail == queue->head) {
    redSpinlockGive( queue->lock );
    return RED_FAIL_COLLECTION_EMPTY;
  }

  *data = *(queue->head);
  (queue->head)++;

  if ((size_t)queue->head - (size_t)queue->queue >=
      queue->lengthLimit * sizeof(void*))
    queue->head = queue->queue;

  if (queue->full)
    queue->full = 0;

  return redSpinlockGive( queue->lock );
}

int
redQueueLengthNonblocking(
    RedQueue queue,
    size_t*  length
    )
{
  int rc = RED_SUCCESS;

  rc = redSpinlockTake( queue->lock );
  if (rc) goto end;

  if (queue->tail > queue->head)
    *length = ((size_t)queue->tail - (size_t)queue->head) / sizeof(void*);
  else if (queue->head > queue->tail)
    *length = queue->lengthLimit + ((size_t)queue->tail - (size_t)queue->head) /
              sizeof(void*);
  else if (queue->full)
    *length = queue->lengthLimit;
  else
    *length = 0;

  rc = redSpinlockGive( queue->lock );
  if (rc) goto end;

end:
  return rc;
}

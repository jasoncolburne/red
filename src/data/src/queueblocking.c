/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: queueblocking.c 19 2010-04-22 08:27:08Z jason $
 */

#include <stdint.h>

#include "_he_queue.h"
#include "he_return.h"

static
int
createHook(
    RedQueue queue
    )
{
  int rc = RED_SUCCESS;

  red_u32 gives = 0x7fffffff;

  if (!queue)
    return RED_ERR_NULL_CONTEXT;

  if (queue->maximumLengthLimit)
    gives = (red_u32)queue->maximumLengthLimit;

  rc = redSpinlockCreate( &(queue->lock), queue->rCtx );
  if (rc) goto end;

  rc = redSemaphoreCreate( &(queue->giveSemaphore), gives, queue->rCtx );
  if (rc) goto end;

  rc = redSemaphoreCreate( &(queue->takeSemaphore), 0, queue->rCtx );
  if (rc) goto end;

end:
  if (rc) {
    redSpinlockDestroy( &(queue->lock) );
    redSemaphoreDestroy( &(queue->giveSemaphore) );
    redSemaphoreDestroy( &(queue->takeSemaphore) );
  }

  return rc;
}

static
int
destroyHook(
    RedQueue queue
    )
{
  if (!queue)
    return RED_ERR_NULL_CONTEXT;

  redSpinlockDestroy( &(queue->lock) );
  redSemaphoreDestroy( &(queue->giveSemaphore) );
  redSemaphoreDestroy( &(queue->takeSemaphore) );

  return RED_SUCCESS;
}

static
int
redQueueGiveBlocking(
    RedQueue queue,
    void*    data
    )
{
  int rc = RED_SUCCESS;

  rc = redSemaphoreTakeTimed( queue->giveSemaphore, 0 );
  if (rc) {
    if (rc == RED_FAIL_TIMED_OUT) rc = RED_FAIL_COLLECTION_FULL;
    goto end;
  }

  rc = redQueueGiveNonblocking( queue, data );
  if (rc) {
    /* TODO: try and figure out whether to give back the give semaphore */
    goto end;
  }

  rc = redSemaphoreGive( queue->takeSemaphore );
  if (rc) goto end;

end:
  return rc;
}

static
int
redQueueTakeBlocking(
    RedQueue queue,
    void**   data
    )
{
  int rc = RED_SUCCESS;

  rc = redSemaphoreTakeTimed( queue->takeSemaphore, 0 );
  if (rc) {
    if (rc == RED_FAIL_TIMED_OUT) rc = RED_FAIL_COLLECTION_EMPTY;
    goto end;
  }

  rc = redQueueTakeNonblocking( queue, data );
  if (rc) {
    /* TODO: try and figure out whether to give back the give semaphore */
    goto end;
  }

  rc = redSemaphoreGive( queue->giveSemaphore );
  if (rc) goto end;

end:
  return rc;
}

static
int
redQueueGiveTimedBlocking(
    RedQueue queue,
    void*    data,
    RedTime  timeout
    )
{
  int rc = RED_SUCCESS;

  if (timeout < 0)
    return RED_ERR_INVALID_ARGUMENT;

  if (timeout)
    rc = redSemaphoreTakeTimed( queue->giveSemaphore, timeout );
  else
    rc = redSemaphoreTake( queue->giveSemaphore );
  if (rc) goto end;

  rc = redQueueGiveNonblocking( queue, data );
  if (rc) {
    /* TODO: try and figure out whether to give back the give semaphore */
    goto end;
  }

  rc = redSemaphoreGive( queue->takeSemaphore );
  if (rc) goto end;

end:
  return rc;
}

static
int
redQueueTakeTimedBlocking(
    RedQueue queue,
    void**   data,
    RedTime  timeout
    )
{
  int rc = RED_SUCCESS;

  if (timeout < 0)
    return RED_ERR_INVALID_ARGUMENT;

  if (timeout)
    rc = redSemaphoreTakeTimed( queue->takeSemaphore, timeout );
  else
    rc = redSemaphoreTake( queue->takeSemaphore );
  if (rc) goto end;

  rc = redQueueTakeNonblocking( queue, data );
  if (rc) {
    /* TODO: try and figure out whether to give back the give semaphore */
    goto end;
  }

  rc = redSemaphoreGive( queue->giveSemaphore );
  if (rc) goto end;

end:
  return rc;
}

int
redQueueCreateBlocking(
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
  f.rfGive        = redQueueGiveBlocking;
  f.rfTake        = redQueueTakeBlocking;
  f.rfTakeTimed   = redQueueTakeTimedBlocking;
  f.rfGiveTimed   = redQueueGiveTimedBlocking;

  return redQueueCreate( queue, initialLengthLimit, maximumLengthLimit, &f,
                         rCtx );
}

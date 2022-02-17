/* Copyright (C) 2010-2022, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: queue.c 19 2010-04-22 08:27:08Z jason $
 */

#include "red_queue.h"
#include "_red_queue.h"

#include "red_memory.h"
#include "red_return.h"
#include "red_stream.h"


int
redQueueCreate(
    RedQueue*             queue,
    size_t                initialLengthLimit,
    size_t                maximumLengthLimit,
    RedQueueFunctions     f,
    RedContext            rCtx
    )
{
  int rc = RED_SUCCESS;

  RedQueue q = NULL;

  if (!queue || !f)
    return RED_ERR_NULL_POINTER;
  if (*queue)
    return RED_ERR_INITIALIZED_POINTER;
  if ((maximumLengthLimit && maximumLengthLimit < initialLengthLimit) ||
      !initialLengthLimit)
    return RED_ERR_INVALID_ARGUMENT;
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;

  rc = redMalloc( rCtx, (void**)&q, sizeof(*q) );
  if (rc) goto end;

  q->lengthLimit        = 0;
  q->maximumLengthLimit = maximumLengthLimit;

  q->rfGrow        = f->rfGrow;
  q->rfLength      = f->rfLength;
  q->rfGive        = f->rfGive;
  q->rfTake        = f->rfTake;
  q->rfGiveTimed   = f->rfGiveTimed;
  q->rfTakeTimed   = f->rfTakeTimed;
  q->rfDestroyHook = f->rfDestroyHook;

  q->rCtx = rCtx;

  if (f->rfCreateHook)
    rc = f->rfCreateHook( q );
  if (rc) goto end;

  rc = redQueueGrow( q, initialLengthLimit );
  if (rc) goto end;

  *queue = q;

end:
  if (rc && q) {
    if (f->rfDestroyHook)
      f->rfDestroyHook( q );

    redFree( rCtx, (void**)&(q->queue), 0 );
    redFree( rCtx, (void**)&q, 0 );
  }

  return rc;
}

int
redQueueDestroy(
    RedQueue* queue
    )
{
  if (!queue)
    return RED_ERR_NULL_POINTER;

  if (*queue) {
    RedQueue q = *queue;

    if (q->rfDestroyHook)
      q->rfDestroyHook( q );

    redFree( q->rCtx, (void**)&(q->queue), 0 );
    redFree( q->rCtx, (void**)queue, 0 );
  }

  return RED_SUCCESS;
}

int
redQueueGrow(
    RedQueue queue,
    size_t   limit
    )
{
  if (!queue)
    return RED_ERR_NULL_CONTEXT;
  if (!(queue->rfGrow))
    return RED_ERR_NOT_SUPPORTED;

  return queue->rfGrow( queue, limit );
}

int
redQueueLength(
    RedQueue queue,
    size_t*  length
    )
{
  if (!queue)
    return RED_ERR_NULL_CONTEXT;
  if (!length)
    return RED_ERR_NULL_POINTER;
  if (!(queue->rfLength))
    return RED_ERR_NOT_SUPPORTED;

  return queue->rfLength( queue, length );
}
int
redQueueGive(
    RedQueue queue,
    void*    data
    )
{
  if (!queue)
    return RED_ERR_NULL_CONTEXT;
  if (!(queue->rfGive))
    return RED_ERR_NOT_SUPPORTED;

  return queue->rfGive( queue, data );
}

int
redQueueTake(
    RedQueue queue,
    void**   data
    )
{
  if (!queue)
    return RED_ERR_NULL_CONTEXT;
  if (!data)
    return RED_ERR_NULL_POINTER;
  if (!(queue->rfTake))
    return RED_ERR_NOT_SUPPORTED;

  return queue->rfTake( queue, data );
}

int
redQueueGiveTimed(
    RedQueue queue,
    void*    data,
    RedTime  timeout
    )
{
  if (!queue)
    return RED_ERR_NULL_CONTEXT;
  if (!(queue->rfGiveTimed))
    return RED_ERR_NOT_SUPPORTED;

  return queue->rfGiveTimed( queue, data, timeout );
}

int
redQueueTakeTimed(
    RedQueue queue,
    void**   data,
    RedTime  timeout
    )
{
  if (!queue)
    return RED_ERR_NULL_CONTEXT;
  if (!data)
    return RED_ERR_NULL_POINTER;
  if (!(queue->rfTakeTimed))
    return RED_ERR_NOT_SUPPORTED;

  return queue->rfTakeTimed( queue, data, timeout );
}

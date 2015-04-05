/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: stackblocking.c 19 2010-04-22 08:27:08Z jason $
 */

#include "red_stack.h"
#include "_red_stack.h"

#include "red_return.h"

static
int
createHook(
    RedStack stack
    )
{
  int rc = RED_SUCCESS;

  red_u32 gives = 0x7fffffff;

  if (!stack)
    return RED_ERR_NULL_CONTEXT;

  if (stack->maximumDepthLimit)
    gives = (red_u32)stack->maximumDepthLimit;

  rc = redSpinlockCreate( &(stack->lock), stack->rCtx );
  if (rc) goto end;

  rc = redSemaphoreCreate( &(stack->giveSemaphore), gives, stack->rCtx );
  if (rc) goto end;

  rc = redSemaphoreCreate( &(stack->takeSemaphore), 0, stack->rCtx );
  if (rc) goto end;

end:
  if (rc) {
    redSpinlockDestroy( &(stack->lock) );
    redSemaphoreDestroy( &(stack->giveSemaphore) );
    redSemaphoreDestroy( &(stack->takeSemaphore) );
  }

  return rc;
}

static
int
destroyHook(
    RedStack stack
    )
{
  if (!stack)
    return RED_ERR_NULL_CONTEXT;

  redSpinlockDestroy( &(stack->lock) );
  redSemaphoreDestroy( &(stack->giveSemaphore) );
  redSemaphoreDestroy( &(stack->takeSemaphore) );

  return RED_SUCCESS;
}

static
int
redStackGiveBlocking(
    RedStack stack,
    void*    data
    )
{
  int rc = RED_SUCCESS;

  rc = redSemaphoreTakeTimed( stack->giveSemaphore, 0 );
  if (rc) {
    if (rc == RED_FAIL_TIMED_OUT) rc = RED_FAIL_COLLECTION_FULL;
    goto end;
  }

  rc = redStackGiveNonblocking( stack, data );
  if (rc) {
    /* TODO: try and figure out whether to give back the give semaphore */
    goto end;
  }

  rc = redSemaphoreGive( stack->takeSemaphore );
  if (rc) goto end;

end:
  return rc;
}

static
int
redStackTakeBlocking(
    RedStack stack,
    void**   data
    )
{
  int rc = RED_SUCCESS;

  rc = redSemaphoreTakeTimed( stack->takeSemaphore, 0 );
  if (rc) {
    if (rc == RED_FAIL_TIMED_OUT) rc = RED_FAIL_COLLECTION_EMPTY;
    goto end;
  }

  rc = redStackTakeNonblocking( stack, data );
  if (rc) {
    /* TODO: try and figure out whether to give back the give semaphore */
    goto end;
  }

  rc = redSemaphoreGive( stack->giveSemaphore );
  if (rc) goto end;

end:
  return rc;
}

static
int
redStackGiveTimedBlocking(
    RedStack stack,
    void*    data,
    RedTime  timeout
    )
{
  int rc = RED_SUCCESS;

  if (timeout < 0)
    return RED_ERR_INVALID_ARGUMENT;

  if (timeout)
    rc = redSemaphoreTakeTimed( stack->giveSemaphore, timeout );
  else
    rc = redSemaphoreTake( stack->giveSemaphore );
  if (rc) goto end;

  rc = redStackGiveNonblocking( stack, data );
  if (rc) {
    /* TODO: try and figure out whether to give back the give semaphore */
    goto end;
  }

  rc = redSemaphoreGive( stack->takeSemaphore );
  if (rc) goto end;

end:
  return rc;
}

static
int
redStackTakeTimedBlocking(
    RedStack stack,
    void**   data,
    RedTime  timeout
    )
{
  int rc = RED_SUCCESS;

  if (timeout < 0)
    return RED_ERR_INVALID_ARGUMENT;

  if (timeout)
    rc = redSemaphoreTakeTimed( stack->takeSemaphore, timeout );
  else
    rc = redSemaphoreTake( stack->takeSemaphore );
  if (rc) goto end;

  rc = redStackTakeNonblocking( stack, data );
  if (rc) {
    /* TODO: try and figure out whether to give back the give semaphore */
    goto end;
  }

  rc = redSemaphoreGive( stack->giveSemaphore );
  if (rc) goto end;

end:
  return rc;
}

int
redStackCreateBlocking(
    RedStack*  stack,
    size_t     initialDepthLimit,
    size_t     maximumDepthLimit,
    RedContext rCtx
    )
{
  struct _RedStackFunctions f = { 0 };

  f.rfCreateHook  = createHook;
  f.rfDestroyHook = destroyHook;
  f.rfDepth       = redStackDepthNonblocking;
  f.rfGrow        = redStackGrowNonblocking;
  f.rfGive        = redStackGiveBlocking;
  f.rfTake        = redStackTakeBlocking;
  f.rfTakeTimed   = redStackTakeTimedBlocking;
  f.rfGiveTimed   = redStackGiveTimedBlocking;

  return redStackCreate( stack, initialDepthLimit, maximumDepthLimit, &f,
                         rCtx );
}

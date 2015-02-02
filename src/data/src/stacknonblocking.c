/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: stacknonblocking.c 19 2010-04-22 08:27:08Z jason $
 */

#include "_he_stack.h"

#include "he_memory.h"
#include "he_return.h"

static
int
createHook(
    RedStack stack
    )
{
  if (!stack)
    return RED_ERR_NULL_CONTEXT;
  
  return redSpinlockCreate( &(stack->lock), stack->rCtx );
}

static
int
destroyHook(
    RedStack stack
    )
{
  if (!stack)
    return RED_ERR_NULL_CONTEXT;

  return redSpinlockDestroy( &(stack->lock) );
}


int
redStackCreateNonblocking(
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
  f.rfGive        = redStackGiveNonblocking;
  f.rfTake        = redStackTakeNonblocking;
  f.rfTakeTimed   = NULL;
  f.rfGiveTimed   = NULL;

  return redStackCreate( stack, initialDepthLimit, maximumDepthLimit, &f,
                         rCtx );
}

int
redStackGrowNonblocking(
    RedStack stack,
    size_t   limit
    )
{
  int rc = RED_SUCCESS;

  void* d = NULL;
  void* s = NULL;

  /* TODO: this is a confusing return code */
  if (limit > stack->maximumDepthLimit && stack->maximumDepthLimit)
    return RED_ERR_INVALID_ARGUMENT;
  /* we'll not throw an error when they ask to shrink beause it likely means
   * they were in a race condition and is not a programmer error TODO note this
   * in documentation */
  if (limit <= stack->depthLimit)
    goto end;

  rc = redMalloc( stack->rCtx, &d, sizeof(void*) * limit );
  if (rc) goto end;

  if (stack->bottom) {
    rc = redMemcpy( stack->rCtx, d, stack->bottom,
                    (size_t)stack->top - (size_t)stack->bottom );
    if (rc) goto end;
  }

  s = stack->bottom;

  stack->top        = (void**)((size_t)stack->top - (size_t)stack->bottom +
                               (size_t)d);
  stack->bottom     = d;
  stack->depthLimit = limit;

  d = NULL;

end:
  redFree( stack->rCtx, &d, 0 );
  redFree( stack->rCtx, &s, 0 );

  return rc;
}

int
redStackGiveNonblocking(
    RedStack stack,
    void*    data
    )
{
  int rc = RED_SUCCESS;

  if (!stack)
    return RED_ERR_NULL_CONTEXT;

  rc = redSpinlockTake( stack->lock );
  if (rc) return rc;

  if (((size_t)stack->top - (size_t)stack->bottom) >=
      stack->depthLimit * sizeof(void*)) {

    if (stack->depthLimit >= stack->maximumDepthLimit) {
      rc = RED_FAIL_COLLECTION_FULL;
      goto end;
    }

    /* double up if we can or expand to max if not */
    if (stack->maximumDepthLimit / 2 > stack->depthLimit)
      rc = redStackGrow( stack, stack->depthLimit * 2 );
    else
      rc = redStackGrow( stack, stack->maximumDepthLimit );
    if (rc) goto end;
  }

  *(stack->top) = data;
  (stack->top)++;

end:
  redSpinlockGive( stack->lock );

  return rc;
}

int
redStackTakeNonblocking(
    RedStack stack,
    void**   data
    )
{
  int rc = RED_SUCCESS;

  if (!stack)
    return RED_ERR_NULL_CONTEXT;
  if (!data)
    return RED_ERR_NULL_POINTER;

  rc = redSpinlockTake( stack->lock );
  if (rc) return rc;

  if (stack->top == stack->bottom) {
    redSpinlockGive( stack->lock );
    return RED_FAIL_COLLECTION_EMPTY;
  }

  (stack->top)--;
  *data = *(stack->top);

  return redSpinlockGive( stack->lock );
}

int
redStackDepthNonblocking(
    RedStack stack,
    size_t*  depth
    )
{
  int rc = RED_SUCCESS;

  rc = redSpinlockTake( stack->lock );
  if (rc) goto end;

  *depth = ((size_t)stack->top - (size_t)stack->bottom) / sizeof(void*);

  rc = redSpinlockGive( stack->lock );
  if (rc) goto end;

end:
  return rc;
}

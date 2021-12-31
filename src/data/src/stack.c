/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: stack.c 19 2010-04-22 08:27:08Z jason $
 */

#include "red_stack.h"
#include "_red_stack.h"

#include "red_memory.h"
#include "red_return.h"
#include "red_stream.h"


int
redStackCreate(
    RedStack*             stack,
    size_t                initialDepthLimit,
    size_t                maximumDepthLimit,
    RedStackFunctions     f,
    RedContext            rCtx
    )
{
  int rc = RED_SUCCESS;

  RedStack s = NULL;

  if (!stack || !f)
    return RED_ERR_NULL_POINTER;
  if (*stack)
    return RED_ERR_INITIALIZED_POINTER;
  if ((maximumDepthLimit && maximumDepthLimit < initialDepthLimit) ||
      !initialDepthLimit)
    return RED_ERR_INVALID_ARGUMENT;
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;

  rc = redMalloc( rCtx, (void**)&s, sizeof(*s) );
  if (rc) goto end;

  s->depthLimit        = 0;
  s->maximumDepthLimit = maximumDepthLimit;

  s->rfGrow        = f->rfGrow;
  s->rfDepth       = f->rfDepth;
  s->rfGive        = f->rfGive;
  s->rfTake        = f->rfTake;
  s->rfGiveTimed   = f->rfGiveTimed;
  s->rfTakeTimed   = f->rfTakeTimed;
  s->rfDestroyHook = f->rfDestroyHook;

  s->rCtx = rCtx;

  if (f->rfCreateHook)
    rc = f->rfCreateHook( s );
  if (rc) goto end;

  rc = redStackGrow( s, initialDepthLimit );
  if (rc) goto end;

  *stack = s;

end:
  if (rc && s) {
    if (f->rfDestroyHook)
      f->rfDestroyHook( s );

    redFree( rCtx, (void**)&(s->bottom), 0 );
    redFree( rCtx, (void**)&s, 0 );
  }

  return rc;
}

int
redStackDestroy(
    RedStack* stack
    )
{
  if (!stack)
    return RED_ERR_NULL_POINTER;

  if (*stack) {
    RedStack s = *stack;

    if (s->rfDestroyHook)
      s->rfDestroyHook( s );
    
    redFree( s->rCtx, (void**)&(s->bottom), 0 );
    redFree( s->rCtx, (void**)stack, 0 );
  }

  return RED_SUCCESS;
}

int
redStackGrow(
    RedStack stack,
    size_t   limit
    )
{
  if (!stack)
    return RED_ERR_NULL_CONTEXT;
  if (!(stack->rfGrow))
    return RED_ERR_NOT_SUPPORTED;

  return stack->rfGrow( stack, limit );
}

int
redStackDepth(
    RedStack stack,
    size_t*  depth
    )
{
  if (!stack)
    return RED_ERR_NULL_CONTEXT;
  if (!depth)
    return RED_ERR_NULL_POINTER;
  if (!(stack->rfDepth))
    return RED_ERR_NOT_SUPPORTED;

  return stack->rfDepth( stack, depth );
}
int
redStackGive(
    RedStack stack,
    void*    data
    )
{
  if (!stack)
    return RED_ERR_NULL_CONTEXT;
  if (!(stack->rfGive))
    return RED_ERR_NOT_SUPPORTED;

  return stack->rfGive( stack, data );
}

int
redStackTake(
    RedStack stack,
    void**   data
    )
{
  if (!stack)
    return RED_ERR_NULL_CONTEXT;
  if (!data)
    return RED_ERR_NULL_POINTER;
  if (!(stack->rfTake))
    return RED_ERR_NOT_SUPPORTED;

  return stack->rfTake( stack, data );
}

int
redStackGiveTimed(
    RedStack stack,
    void*    data,
    RedTime  timeout
    )
{
  if (!stack)
    return RED_ERR_NULL_CONTEXT;
  if (!(stack->rfGiveTimed))
    return RED_ERR_NOT_SUPPORTED;

  return stack->rfGiveTimed( stack, data, timeout );
}

int
redStackTakeTimed(
    RedStack stack,
    void**   data,
    RedTime  timeout
    )
{
  if (!stack)
    return RED_ERR_NULL_CONTEXT;
  if (!data)
    return RED_ERR_NULL_POINTER;
  if (!(stack->rfTakeTimed))
    return RED_ERR_NOT_SUPPORTED;

  return stack->rfTakeTimed( stack, data, timeout );
}

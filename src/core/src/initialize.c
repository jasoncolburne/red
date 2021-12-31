/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: initialize.c 19 2010-04-22 08:27:08Z jason $
 */

#include "red_stringbuffer.h"


#include "red_initialize.h"
#include "_red_initialize.h"
#include "_red_trace.h"

#include "red_memory.h"
#include "red_return.h"

static RedGlobalContext __redGlobalContext = NULL;

static
int
_globalThread(
    RedThread  thread,
    void*      data,
    RedContext rCtx
    )
{
  int rc = RED_SUCCESS;
  
  RedGlobalContext r = (RedGlobalContext)data;
  RedTraceContext  t = NULL;

  /* wait indefinitely */
  while ((rc = redQueueTakeTimed( r->traceQueue, (void**)&t, 0 ))
          == RED_SUCCESS) {
    if (t) {
      char*  s;
      size_t sz;

      rc = redStringBufferTake( t->messages, &s, &sz );
      /* TODO: how do we want to handle this */
      if (rc) break;

      rc = t->rfWrite( t->privateData, s, sz );
      /* TODO: how do we want to handle this */
      if (rc) break;

      rc = redStringBufferAdvance( t->messages );
      /* TODO: how do we want to handle this */
      if (rc) break;
    }
    else if (r->done)
      break;
  }

  return rc;
}


int
redInitialize(
    RedContext rCtx
    )
{
  int rc = RED_SUCCESS;

  RedGlobalContext r = NULL;

  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;
  if (__redGlobalContext)
    return RED_ERR_LIBRARY_INITIALIZED;

  rc = redMalloc( rCtx, (void**)&r, sizeof(*r) );
  if (rc) goto end;

  rc = redQueueCreateBlocking( &(r->traceQueue), 32, 0, rCtx );
  if (rc) goto end;

  r->rCtx = rCtx;
  r->done = 0;

  /* TODO: create thread local storage */

  rc = redThreadCreate( &(r->thread), NULL, _globalThread, r, rCtx );
  if (rc) goto end;

  __redGlobalContext = r;

end:
  if (rc && r) {
    redQueueDestroy( &(r->traceQueue) );
    redFree( rCtx, (void**)&r, 0 );
  }

  return rc;
}

int
redDeinitialize(
    void
    )
{
  int rc = RED_SUCCESS;

  RedGlobalContext r = __redGlobalContext;

  if (!__redGlobalContext)
    return RED_ERR_LIBRARY_NOT_INITIALIZED;

  r->done = 1;

  rc = redQueueGive( r->traceQueue, NULL );
  if (rc) goto end;

  rc = redThreadJoin( r->thread );
  if (rc) goto end;

  rc = redThreadDestroy( &(r->thread) );
  if (rc) goto end;

  rc = redQueueDestroy( &(r->traceQueue) );
  if (rc) goto end;

  rc = redFree( r->rCtx, (void**)&__redGlobalContext, 0 );
  
end:
  return rc;
}

int
redGlobalContextTraceQueueGet(
    RedQueue* traceQueue
    )
{
  RedGlobalContext r = __redGlobalContext;

  if (!r)
    return RED_ERR_LIBRARY_NOT_INITIALIZED;
  if (!traceQueue)
    return RED_ERR_NULL_POINTER;

  *traceQueue = r->traceQueue;

  return RED_SUCCESS;
}

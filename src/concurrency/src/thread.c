/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: thread.c 19 2010-04-22 08:27:08Z jason $
 */

#include "he_thread.h"
#include "_he_thread.h"

#include "he_memory.h"
#include "he_return.h"

#include <errno.h>

static
void*
redThreadWrapper(
    void* arg
    )
{
  RedThread t = (RedThread)arg;

  /* todo: handle this somehow */
  t->rc   = (t->function)( t, t->data, t->rCtx );
  t->done = 1;

  return NULL;
}


int
redThreadCreate(
    RedThread*          thread,
    RedThreadAttributes attr,
    redfuncThread       function,
    void*               data,
    RedContext          rCtx
    )
{
  int rc = RED_SUCCESS;

  RedThread t = NULL;

  if (!thread)
    return RED_ERR_NULL_POINTER;
  if (*thread)
    return RED_ERR_INITIALIZED_POINTER;
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;

  rc = redMalloc( rCtx, (void**)&t, sizeof(*t) );
  if (!rc) {
    t->data     = data;
    t->function = function;
    t->rCtx     = rCtx;
    
    /* unnecessary since redMalloc zeroes data */
    /* t->done = 0; t->rc = RED_SUCCESS; */

    if (attr)
      rc = pthread_create( &(t->thr), &(attr->attr), redThreadWrapper, t );
    else
      rc = pthread_create( &(t->thr), NULL, redThreadWrapper, t );

    /* TODO: wrap errno errors */
    if (rc) goto end;

    *thread = t;
  }

end:
  if (rc)
    redFree( rCtx, (void**)&t, 0 );

  return rc;
}

int
redThreadDestroy(
    RedThread* thread
    )
{
  if (!thread)
    return RED_ERR_NULL_POINTER;
  if (!*thread)
    return RED_ERR_NULL_CONTEXT;

  /* todo: add a check for 'done' var? */

  return redFree( (*thread)->rCtx, (void**)thread, 0 );
}

int
redThreadJoin(
    RedThread thread
    )
{
  int rc = RED_SUCCESS;

  if (!thread)
    return RED_ERR_NULL_CONTEXT;

  rc = pthread_join( thread->thr, NULL );
  if (rc) {
         if (rc == ESRCH)  rc = RED_ERR_THREAD_DNE;
    else if (rc == EINVAL) rc = RED_ERR_THREAD_NOT_JOINABLE;
    /* TODO: EDEADLK */
  }

  return rc;
}

/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: lock.c 18 2010-04-17 05:57:03Z jason $
 */

#include "red_lock.h"
#include "_red_lock.h"

#include "red_memory.h"
#include "red_return.h"

int
redLockCreate(
    RedLock*   lock,
    RedContext rCtx
    )
{
  int rc = RED_SUCCESS;

  RedLock l = NULL;

  if (!lock)
    return RED_ERR_NULL_POINTER;
  if (*lock)
    return RED_ERR_INITIALIZED_POINTER;
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;

  rc = redMalloc( rCtx, (void**)&l, sizeof(*l) );
  if (!rc) {
    l->rCtx = rCtx;

    rc = pthread_mutex_init( &(l->lock), NULL );
    /* TODO: wrap errno errors */
    if (rc) goto end;

    *lock = l;
  }

end:
  if (rc)
    redFree( rCtx, (void**)&l, 0 );

  return rc;
}

int
redLockDestroy(
    RedLock* lock
    )
{
  int rc = RED_SUCCESS;

  if (!lock)
    return RED_ERR_NULL_POINTER;
  if (!*lock)
    return RED_ERR_NULL_CONTEXT;

  rc = pthread_mutex_destroy( &((*lock)->lock) );
  /* TODO: translate error */
  if (rc) goto end;

  rc = redFree( (*lock)->rCtx, (void**)lock, 0 );
end:
  if (rc)
    redFree( (*lock)->rCtx, (void**)lock, 0 );

  return rc;
}

int
redLockTake(
    RedLock lock
    )
{
  if (!lock)
    return RED_ERR_NULL_CONTEXT;

  /* TODO: translate errors */
  return pthread_mutex_lock( &(lock->lock) );
}

int
redLockGive(
    RedLock lock
    )
{
  if (!lock)
    return RED_ERR_NULL_CONTEXT;

  /* TODO: translate errors */
  return pthread_mutex_unlock( &(lock->lock) );
}

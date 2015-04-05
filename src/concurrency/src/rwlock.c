/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: rwlock.c 21 2010-04-28 17:38:52Z jason $
 */

#include "he_rwlock.h"
#include "_he_rwlock.h"

#include "he_memory.h"
#include "he_return.h"

int
redRWLockCreate(
    RedRWLock* lock,
    RedContext rCtx
    )
{
  int rc = RED_SUCCESS;

  RedRWLock l = NULL;

  if (!lock)
    return RED_ERR_NULL_POINTER;
  if (*lock)
    return RED_ERR_INITIALIZED_POINTER;
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;

  rc = redMalloc( rCtx, (void**)&l, sizeof(*l) );
  if (!rc) {
#if defined (__linux__)
    pthread_rwlockattr_t attr;

    rc = pthread_rwlockattr_init( &attr );
    /* TODO: errno */
    if (rc) goto end;

    rc = pthread_rwlockattr_setkind_np( &attr,
                                PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP );
    /* TODO: errno */
    if (rc) goto end;

    rc = pthread_rwlock_init( &(l->lock), &attr );
    /* TODO: errno */
    if (rc) goto end;
    
    rc = pthread_rwlockattr_destroy( &attr );
    if (rc) goto end;
#else
    rc = pthread_rwlock_init( &(l->lock), NULL );
#endif

    /* TODO: wrap errno errors */
    if (rc) goto end;

    l->rCtx = rCtx;

    *lock = l;
  }

end:
  if (rc)
    redFree( rCtx, (void**)&l, 0 );

  return rc;
}

int
redRWLockDestroy(
    RedRWLock* lock
    )
{
  int rc = RED_SUCCESS;

  if (!lock)
    return RED_ERR_NULL_POINTER;
  if (!*lock)
    return RED_ERR_NULL_CONTEXT;

  rc = pthread_rwlock_destroy( &((*lock)->lock) );
  /* TODO: translate error */
  if (rc) goto end;

  rc = redFree( (*lock)->rCtx, (void**)lock, 0 );
end:
  if (rc)
    redFree( (*lock)->rCtx, (void**)lock, 0 );

  return rc;
}

int
redRWLockTakeRead(
    RedRWLock lock
    )
{
  if (!lock)
    return RED_ERR_NULL_CONTEXT;

  /* TODO: translate errors */
  return pthread_rwlock_rdlock( &(lock->lock) );
}

int
redRWLockTakeWrite(
    RedRWLock lock
    )
{
  if (!lock)
    return RED_ERR_NULL_CONTEXT;

  /* TODO: translate errors */
  return pthread_rwlock_wrlock( &(lock->lock) );
}

int
redRWLockGive(
    RedRWLock lock
    )
{
  if (!lock)
    return RED_ERR_NULL_CONTEXT;

  /* TODO: translate errors */
  return pthread_rwlock_unlock( &(lock->lock) );
}

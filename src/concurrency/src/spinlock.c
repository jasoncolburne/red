/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: spinlock.c 20 2010-04-23 01:46:23Z jason $
 */


#include "he_spinlock.h"
#include "_he_spinlock.h"

#include "he_memory.h"
#include "he_return.h"

int
redSpinlockCreate(
    RedSpinlock* lock,
    RedContext   rCtx
    )
{
  int rc = RED_SUCCESS;

  RedSpinlock l = NULL;

  if (!lock)
    return RED_ERR_NULL_POINTER;
  if (*lock)
    return RED_ERR_INITIALIZED_POINTER;
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;

  rc = redMalloc( rCtx, (void**)&l, sizeof(*l) );
  if (rc) goto end;

#if defined (__APPLE__)
  /* OS X init since a lock is just an integer that atomicinc/dec operate on */
  l->lock = 0;
#elif defined (__linux__)
  if ( pthread_spin_init( &(l->lock), PTHREAD_PROCESS_PRIVATE ) != 0 ) {
    /* TODO: errno */
    rc = 0x666;
    goto end;
  }
#endif

  l->rCtx = rCtx;

  *lock = l;

end:
  if (rc && l)
    redFree( rCtx, (void**)&l, 0 );

  return rc;
}

int
redSpinlockDestroy(
    RedSpinlock* lock
    )
{
  if (!lock)
    return RED_ERR_NULL_POINTER;

  if (*lock) {
#if defined (__linux__)
    pthread_spin_destroy( &((*lock)->lock) );
#endif
    return redFree( (*lock)->rCtx, (void**)lock, 0 );
  }

  return RED_SUCCESS;
}

int
redSpinlockTake(
    RedSpinlock lock
    )
{
  if (!lock)
    return RED_ERR_NULL_CONTEXT;

#if defined (__APPLE__)
  OSSpinLockLock( &(lock->lock) );
#elif defined (__linux__)
  if (pthread_spin_lock( &(lock->lock) ) != 0) {
    /* TODO: errno */
    return 0x666;
  }
#endif

  return RED_SUCCESS;
}

int
redSpinlockGive(
    RedSpinlock lock
    )
{
  if (!lock)
    return RED_ERR_NULL_CONTEXT;

#if defined (__APPLE__)
  OSSpinLockUnlock( &(lock->lock) );
#elif defined (__linux__)
  if (pthread_spin_unlock( &(lock->lock) ) != 0) {
    /* TODO: errno */
    return 0x666;
  }
#endif

  return RED_SUCCESS;
}

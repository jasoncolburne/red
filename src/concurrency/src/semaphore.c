/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: semaphore.c 21 2010-04-28 17:38:52Z jason $
 */

#include "he_semaphore.h"
#include "_he_semaphore.h"

#include "he_memory.h"
#include "he_return.h"

#if defined (__APPLE__)
#  include <mach/mach_init.h>
#  include <mach/sync_policy.h>
#elif defined (__linux__)

#endif

int
redSemaphoreCreate(
    RedSemaphore* semaphore,
    red_u32       value,
    RedContext    rCtx
    )
{
  int rc = RED_SUCCESS;

  RedSemaphore s = NULL;

  if (!semaphore)
    return RED_ERR_NULL_POINTER;
  if (*semaphore)
    return RED_ERR_INITIALIZED_POINTER;
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;

  rc = redMalloc( rCtx, (void**)&s, sizeof(*s) );
  if (rc) goto end;

#if defined (__APPLE__)
  s->owner = mach_task_self();

  rc = semaphore_create( s->owner, &(s->semaphore), SYNC_POLICY_FIFO,
                         (int)value );
  if (rc != KERN_SUCCESS) {
    if (rc == KERN_RESOURCE_SHORTAGE)
      rc = RED_FAIL_OUT_OF_RESOURCES;
    if (rc == KERN_INVALID_ARGUMENT)
      rc = RED_ERR_INVALID_ARGUMENT;

    goto end;
  }
#elif defined (__linux__)
  if (sem_init( &(s->semaphore), 0, (unsigned)value ) == -1) {
    /* TODO: errno translation */
    rc = 0x666;
    goto end;
  }
#endif

  s->rCtx = rCtx;
  
  *semaphore = s;

end:
  if (rc)
    redFree( rCtx, (void**)&s, 0 );

  return rc;
}


int
redSemaphoreDestroy(
    RedSemaphore* semaphore
    )
{
  if (!semaphore)
    return RED_ERR_NULL_POINTER;

  if (*semaphore) {
#if defined (__APPLE__)
    /* TODO: should check rc and translate */
    semaphore_destroy( (*semaphore)->owner, (*semaphore)->semaphore );
#elif defined (__linux__)
    if (sem_destroy( &((*semaphore)->semaphore) ) == -1) {
      /* TODO: errno */
      return 0x666;
    }
#endif
    return redFree( (*semaphore)->rCtx, (void**)semaphore, 0 );
  }

  return RED_SUCCESS;
}


int
redSemaphoreGive(
    RedSemaphore semaphore
    )
{
  int rc = RED_SUCCESS;

  if (!semaphore)
    return RED_ERR_NULL_CONTEXT;

#if defined (__APPLE__)
  rc = semaphore_signal( semaphore->semaphore );
  if (rc != KERN_SUCCESS) {
    /* TODO: wrap these */
    if (rc == KERN_TERMINATED)
      return 0x666;
          
    return RED_ERR_INVALID_ARGUMENT;
  }
#elif defined (__linux__)
  if (sem_post( &(semaphore->semaphore) ) == -1) {
    /* TODO: errno */
    return 0x666;
  }
#endif

  return rc;
}


int
redSemaphoreTake(
    RedSemaphore semaphore
    )
{
  int rc = RED_SUCCESS;

  if (!semaphore)
    return RED_ERR_NULL_CONTEXT;

#if defined (__APPLE__)
  rc = semaphore_wait( semaphore->semaphore );
  if (rc != KERN_SUCCESS) {
    /* TODO: wrap these */
    if (rc == KERN_TERMINATED)
      return 0x666;
    if (rc == KERN_ABORTED)
      return 0x666;
    
    return RED_ERR_INVALID_ARGUMENT;
  }
  /* KERN_SUCCESS is 0, if this changes code will need to be modified */
#elif defined (__linux__)
  if (sem_wait( &(semaphore->semaphore) ) == -1) {
    /* TODO: errno */
    return 0x666;
  }
#endif

  return rc;
}

#include <stdio.h>
int
redSemaphoreTakeTimed(
    RedSemaphore semaphore,
    RedTime      timeout
    )
{
  int rc = RED_SUCCESS;

#if defined (__APPLE__)
  mach_timespec_t to;
#elif defined (__linux__)
  struct timespec to;
  RedTime         now = 0;

  redTimeNowPrecise( &now );
  timeout += now;
#endif

  if (!semaphore)
    return RED_ERR_NULL_CONTEXT;

  to.tv_nsec = timeout % RED_TIME_FROM_SECONDS( 1 );
  to.tv_sec  = (timeout - to.tv_nsec) / RED_TIME_FROM_SECONDS( 1 );

#if defined (__APPLE__)
  rc = semaphore_timedwait( semaphore->semaphore, to );
  if (rc == KERN_SUCCESS)
    return RED_SUCCESS;

  /* TODO: handle other errors */
#elif defined (__linux__)
  rc = sem_timedwait( &(semaphore->semaphore), &to );
  if (rc != -1)
    return RED_SUCCESS;

  /* TODO: check errno */
#endif

  return RED_FAIL_TIMED_OUT;
}


int
redSemaphoreValue(
    RedSemaphore semaphore,
    red_i32*     value
    )
{
#if defined (__APPLE__)
  return RED_NOT_IMPLEMENTED;
#elif defined (__linux__)
  int v;

  if (!semaphore)
    return RED_ERR_NULL_CONTEXT;
  if (!value)
    return RED_ERR_NULL_POINTER;

  if (sem_getvalue( &(semaphore->semaphore), &v ) == -1) {
    /* TODO: errno */
    return 0x666;
  }

  *value = (red_i32)v;

  return RED_SUCCESS;
#endif
}

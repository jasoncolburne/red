/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: condition.c 20 2010-04-23 01:46:23Z jason $
 */

#include "he_condition.h"
#include "_he_condition.h"

#include "he_memory.h"
#include "he_return.h"

int
redConditionCreate(
    RedCondition* cond,
    RedContext    rCtx
    )
{
  int rc = RED_SUCCESS;

  RedCondition c = NULL;

  if (!cond)
    return RED_ERR_NULL_POINTER;
  if (*cond)
    return RED_ERR_INITIALIZED_POINTER;
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;

  rc = redMalloc( rCtx, (void**)&c, sizeof(*c) );
  if (!rc) {
    c->rCtx = rCtx;

#if defined (__APPLE__) || defined (__linux__)
    rc = pthread_mutex_init( &(c->mutex), NULL );
    /* TODO: wrap errno errors */
    if (rc) goto end;

    rc = pthread_cond_init( &(c->cond), NULL );
    /* TODO: wrap errno errors */
    if (rc) goto end;
#endif

    *cond = c;
  }

end:
  if (rc) {
    if (c) {
#if defined (__APPLE__) || defined (__linux__)
      pthread_mutex_destroy( &(c->mutex) );
#endif
    }

    redFree( rCtx, (void**)&c, 0 );
  }

  return rc;
}

int
redConditionDestroy(
    RedCondition* cond
    )
{
  int rc = RED_SUCCESS;

  if (!cond)
    return RED_ERR_NULL_POINTER;

  if (*cond) {
    RedCondition c  = *cond;

#if defined (__APPLE__) || defined (__linux__)
    rc = pthread_cond_destroy( &(c->cond) );
    /* TODO: translate error */
    if (rc) goto end;

    rc = pthread_mutex_destroy( &(c->mutex) );
    /* TODO: translate error */
    if (rc) goto end;
#endif
    
    rc = redFree( c->rCtx, (void**)cond, 0 );
  }
end:
  return rc;
}

int
redConditionTake(
    RedCondition cond
    )
{
  int rc = RED_SUCCESS;

  if (!cond)
    return RED_ERR_NULL_CONTEXT;

#if defined (__APPLE__) || defined (__linux__)
  /* TODO: translate errors */
  rc = pthread_mutex_lock( &(cond->mutex) );
  if (rc) goto end;

  rc = pthread_cond_wait( &(cond->cond), &(cond->mutex) );
  /* failure here can't really be 'fixed' */
  pthread_mutex_unlock( &(cond->mutex) );
#endif
end:
  return rc;
}

int
redConditionTakeTimed(
    RedCondition cond,
    RedTime      timeout
    )
{
  int rc = RED_SUCCESS;

#if defined (__APPLE__) || defined (__linux__)
  struct timespec ts;
#endif

  if (!cond)
    return RED_ERR_NULL_CONTEXT;

#if defined (__APPLE__) || defined (__linux__)
  ts.tv_nsec = timeout % RED_TIME_FROM_SECONDS(1);
  ts.tv_sec  = (timeout - ts.tv_nsec) / RED_TIME_FROM_SECONDS(1);
  
  /* TODO: translate errors */
  rc = pthread_mutex_lock( &(cond->mutex) );
  if (rc) goto end;

  /* TODO: actual timing and looping if interrupted */
  rc = pthread_cond_timedwait( &(cond->cond), &(cond->mutex), &ts );

  /* failure here can't really be 'fixed' */
  pthread_mutex_unlock( &(cond->mutex) );
#endif
  
end:
  return rc;
}


int
redConditionGive(
    RedCondition cond
    )
{
  if (!cond)
    return RED_ERR_NULL_CONTEXT;

#if defined (__APPLE__) || defined (__linux__)
  /* TODO: translate errors */
  return pthread_cond_signal( &(cond->cond) );
#endif
}

int
redConditionGiveAll(
    RedCondition cond
    )
{
  if (!cond)
    return RED_ERR_NULL_CONTEXT;

#if defined (__APPLE__) || defined (__linux__)
  /* TODO: translate errors */
  return pthread_cond_broadcast( &(cond->cond) );
#endif
}

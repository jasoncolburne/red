/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: resourcepool.c 19 2010-04-22 08:27:08Z jason $
 */

#include "he_resourcepool.h"
#include "_he_resourcepool.h"
#include "_he_context.h"

#include "he_memory.h"
#include "he_return.h"

static
int
redResourcePoolGrow(
    RedResourcePool pool,
    size_t          count
    )
{
  int rc = RED_SUCCESS;
  
  void*   data = NULL;
  red_u8* p    = NULL;
  size_t  i    = 0;

  rc = redMalloc( pool->rCtx, (void**)&data, pool->size * count );
  if (rc) goto end;

  rc = redStackGive( pool->dataStack, data );
  if (rc) goto end;

  for (i = 0, p = data; i < count; i++, p += pool->size) {
    if (pool->rfResourceCreate) {
      rc = pool->rfResourceCreate( p, pool->hookData, pool->rCtx );
      if (rc) break;
    }

    rc = redQueueGive( pool->freeQueue, p );
    if (rc) break;
  }

  /* make sure we only add i in case we failed */
  pool->count += i;

end:
  /* if i isn't 0, we have pushed some items on the stack and it's hard to
   * guarantee they have not been removed so we'll just absorb the lost
   * memory and it will be freed later when this resource pool is destroyed */
  if (rc && !i) {
    void* d = NULL;

    if (redStackTake( pool->dataStack, &d ) == RED_SUCCESS) {
      if (d != data)
        redStackGive( pool->dataStack, d );
    }

    redFree( pool->rCtx, (void**)&data, 0 );
  }

  return rc;
}

typedef int (*redfuncQueueCreate)( RedQueue*, size_t, size_t, RedContext );

static
int
redResourcePoolCreate(
    RedResourcePool*                   pool,
    size_t                             resourceSize,
    size_t                             initialCount,
    size_t                             maximumCount,
    redfuncQueueCreate                 rfQueueCreate,
    redfuncResourcePoolResourceCreate  rfResourceCreate,
    redfuncResourcePoolResourceDestroy rfResourceDestroy,
    void*                              hookData,
    RedContext                         rCtx
    )
{
  int rc = RED_SUCCESS;

  RedResourcePool p      = NULL;
  RedStack        dStack = NULL;
  RedQueue        fQueue = NULL;
  
  size_t          s = resourceSize;
  size_t          i = sizeof(void*) - 1;

  if (!pool)
    return RED_ERR_NULL_POINTER;
  if (*pool)
    return RED_ERR_INITIALIZED_POINTER;
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;
  if (!initialCount)
    return RED_ERR_INVALID_ARGUMENT;

  s = (s + i) & ~i;

  rc = redMalloc( rCtx, (void**)&p, sizeof(*p) );
  if (rc) goto end;

  p->size              = s;
  p->count             = 0;
  p->maximum           = maximumCount;
  p->rfResourceCreate  = rfResourceCreate;
  p->rfResourceDestroy = rfResourceDestroy;
  p->hookData          = hookData;
  p->rCtx              = rCtx;

  i = 2;
  s = initialCount;
  /* implicitly prevent an overflow at the cost of size 2 stack when
   * initial = maximum
   */
  while (s < maximumCount / 2) {
    i++;
    s *= 2;
  }
  rc = redStackCreateNonblocking( &dStack, i, (maximumCount ? i : 0), rCtx );
  if (rc) goto end;

  rc = rfQueueCreate( &fQueue, initialCount, maximumCount, rCtx );
  if (rc) goto end;

  p->dataStack = dStack;
  p->freeQueue = fQueue;

  rc = redResourcePoolGrow( p, initialCount );
  if (rc) goto end;

  *pool = p;

end:
  if (rc) {
    redQueueDestroy( &fQueue );
    redStackDestroy( &dStack );
    redFree( rCtx, (void**)&p, 0 );
  }

  return rc;
}

int
redResourcePoolCreateNonblocking(
    RedResourcePool*                   pool,
    size_t                             resourceSize,
    size_t                             initialCount,
    size_t                             maximumCount,
    redfuncResourcePoolResourceCreate  rfResourceCreate,
    redfuncResourcePoolResourceDestroy rfResourceDestroy,
    void*                              hookData,
    RedContext                         rCtx
    )
{
  return redResourcePoolCreate( pool, resourceSize, initialCount, maximumCount,
                                redQueueCreateNonblocking, rfResourceCreate,
                                rfResourceDestroy, hookData, rCtx );
}

int
redResourcePoolCreateBlocking(
    RedResourcePool*                   pool,
    size_t                             resourceSize,
    size_t                             initialCount,
    size_t                             maximumCount,
    redfuncResourcePoolResourceCreate  rfResourceCreate,
    redfuncResourcePoolResourceDestroy rfResourceDestroy,
    void*                              hookData,
    RedContext                         rCtx
    )
{
  return redResourcePoolCreate( pool, resourceSize, initialCount, maximumCount,
                                redQueueCreateBlocking, rfResourceCreate,
                                rfResourceDestroy, hookData, rCtx );
}

int
redResourcePoolDestroy(
    RedResourcePool* pool
    )
{
  int rc = RED_SUCCESS;

  if (!pool)
    return RED_ERR_NULL_POINTER;

  if (*pool) {
    RedResourcePool p        = *pool;
    size_t          length   = 0;
    void*           resource = NULL;
    
    rc = redQueueLength( p->freeQueue, &length );
    if (rc) return rc;

    if (p->count != length)
      return RED_FAIL_OUTSTANDING_RESOURCES;

    if (p->rfResourceDestroy) {
      while ((rc = redQueueTake( p->freeQueue, &resource)) == RED_SUCCESS) {
        rc = p->rfResourceDestroy( resource, p->hookData, p->rCtx );
        /* TODO perhaps don't return here and instead just trace it out */
        if (rc) return rc;
      }
      if (rc != RED_FAIL_COLLECTION_EMPTY)
        return rc;
    }

    while ((rc = redStackTake( p->dataStack, &resource)) == RED_SUCCESS) {
      rc = redFree( p->rCtx, &resource, 0 );
      if (rc) return rc;
    }
    if (rc != RED_FAIL_COLLECTION_EMPTY)
      return rc;

    redQueueDestroy( &(p->freeQueue) );
    redStackDestroy( &(p->dataStack) );
    redFree( p->rCtx, (void**)pool, 0 );
  }

  return RED_SUCCESS;
}

int
redResourcePoolTake(
    RedResourcePool pool,
    void**          resource
    )
{
  if (!pool)
    return RED_ERR_NULL_CONTEXT;
  if (!resource)
    return RED_ERR_NULL_POINTER;

  return redQueueTake( pool->freeQueue, resource );
}

int
redResourcePoolTakeTimed(
    RedResourcePool pool,
    void**          resource,
    RedTime         timeout
    )
{
  if (!pool)
    return RED_ERR_NULL_CONTEXT;
  if (!resource)
    return RED_ERR_NULL_POINTER;

  return redQueueTakeTimed( pool->freeQueue, resource, timeout );
}

int
redResourcePoolGive(
    RedResourcePool pool,
    void*           resource
    )
{
  if (!pool)
    return RED_ERR_NULL_CONTEXT;
  if (!resource)
    return RED_ERR_NULL_POINTER;

  /* TODO: figure out a check to see if this pointer belongs to us */
  return redQueueGive( pool->freeQueue, resource );
}

extern
int
redResourcePoolGiveTimed(
    RedResourcePool pool,
    void*           resource,
    RedTime         timeout
    )
{
  if (!pool)
    return RED_ERR_NULL_CONTEXT;
  if (!resource)
    return RED_ERR_NULL_POINTER;

  /* TODO: figure out a check to see if this pointer belongs to us */
  return redQueueGiveTimed( pool->freeQueue, resource, timeout );
}

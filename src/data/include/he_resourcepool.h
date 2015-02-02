/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_resourcepool.h 19 2010-04-22 08:27:08Z jason $
 */

#ifndef __HE_RESOURCEPOOL_H__
#define __HE_RESOURCEPOOL_H__


#include "he_context.h"
#include "he_time.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _RedResourcePool;
typedef struct _RedResourcePool* RedResourcePool;

typedef int  (*redfuncResourcePoolResourceCreate)( void*, void*, RedContext );
typedef int (*redfuncResourcePoolResourceDestroy)( void*, void*, RedContext );

extern
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
    );

extern
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
    );

extern
int
redResourcePoolDestroy(
    RedResourcePool* pool
    );

extern
int
redResourcePoolTake(
    RedResourcePool pool,
    void**          resource
    );

extern
int
redResourcePoolTakeTimed(
    RedResourcePool pool,
    void**          resource,
    RedTime         timeout
    );

extern
int
redResourcePoolGive(
    RedResourcePool pool,
    void*           resource
    );

extern
int
redResourcePoolGiveTimed(
    RedResourcePool pool,
    void*           resource,
    RedTime         timeout
    );


#ifdef __cplusplus
}
#endif


#endif /* __HE_RESOURCEPOOL_H__ */

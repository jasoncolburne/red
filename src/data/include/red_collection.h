/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_collection.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef __RED_COLLECTION_H__
#define __RED_COLLECTION_H__


#include "red_context.h"
#include "red_iterator.h"

#ifdef __cplusplus
extern "C" {
#endif


struct _RedCollection;
typedef struct _RedCollection* RedCollection;

/* used for testing equality and sorting lists */
typedef int       (*redfuncObjectCompare)( int*, void*, void* );
typedef int       (*redfuncObjectDestroy)( void** );

typedef int      (*redfuncCollectionEach)( void*, void* );
typedef int    (*redfuncCollectionFilter)( void*, void*, int* );
typedef int (*redfuncCollectionTransform)( void*, void**, void* );


extern
int
redCollectionSize(
    RedCollection collection,
    size_t*       size
    );

extern
int
redCollectionContains(
    RedCollection collection,
    void*         object
    );


extern
int
redCollectionEach(
    RedCollection         collection,
    redfuncCollectionEach action,
    void*                 data
    );

extern
int
redCollectionFilter(
    RedCollection           collection,
    redfuncCollectionFilter predicate,
    RedCollection           destination,
    void*                   data
    );

extern
int
redCollectionTransform(
    RedCollection              collection,
    redfuncCollectionTransform transformation,
    RedCollection              destination,
    void*                      data
    );



#ifdef __cplusplus
}
#endif


#endif /* __RED_COLLECTION_H__ */

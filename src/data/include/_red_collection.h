/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_collection.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef ___RED_COLLECTION_H__
#define ___RED_COLLECTION_H__


#include "red_collection.h"
#include "red_rwlock.h"

#define RED_COLLECTION_KIND_SET  0x0001
#define RED_COLLECTION_KIND_LIST 0x0002
#define RED_COLLECTION_KIND_MAP  0x0003

typedef int (*redfuncCollectionContains)( RedCollection, void*, int* );
typedef int     (*redfuncCollectionGive)( RedCollection, void* );
typedef int     (*redfuncCollectionTake)( RedCollection, void* );
typedef int (*redfuncCollectionIterator)( RedCollection, RedIterator* );


typedef struct {
  redfuncObjectCompare rfCompare;
  redfuncObjectDestroy rfDestroy;

  redfuncCollectionContains rfContains;
  redfuncCollectionGive     rfGive;
  redfuncCollectionTake     rfTake;
  redfuncCollectionIterator rfIterator;
} RedCollectionFunctions;


struct _RedCollection {
  void*  data;

  size_t size;
  int    kind;

  RedRWLock lock;

  redfuncObjectCompare rfCompare;
  redfuncObjectDestroy rfDestroy;

  redfuncCollectionContains rfContains;
  redfuncCollectionGive     rfGive;
  redfuncCollectionTake     rfTake;
  redfuncCollectionIterator rfIterator;

  RedContext iteratorCtx;
  RedContext rCtx;
};


extern
int
redCollectionCreate(
    RedCollection*          collection,
    void*                   data,
    int                     kind,
    RedCollectionFunctions* functions,
    RedContext              rCtx
    );

extern
int
redCollectionDestroy(
    RedCollection* collection
    );



#endif /* ___RED_COLLECTION_H__ */

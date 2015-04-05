/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: collection.c 18 2010-04-17 05:57:03Z jason $
 */

#include "red_collection.h"
#include "_red_collection.h"

#include "red_memory.h"
#include "red_return.h"

int
redCollectionCreate(
    RedCollection*          collection,
    void*                   data,
    int                     kind,
    RedCollectionFunctions* functions,
    RedContext              rCtx
    )
{
  int rc = RED_SUCCESS;

  RedCollection c = NULL;

  if (!collection || !data || !functions)
    return RED_ERR_NULL_POINTER;
  if (*collection)
    return RED_ERR_INITIALIZED_POINTER;
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;

  rc = redMalloc( rCtx, (void**)&c, sizeof(*c) );
  if (rc) goto end;

  rc = redRWLockCreate( &(c->lock), rCtx );
  if (rc) goto end;

  c->data = data;
  c->rCtx = rCtx;

  /* TODO: make this a pool */
  c->iteratorCtx = rCtx;

  /* TODO: validate this */
  c->kind = kind;
  c->size = 0;

  c->rfCompare  = functions->rfCompare;
  c->rfDestroy  = functions->rfDestroy;
  c->rfContains = functions->rfContains;
  c->rfGive     = functions->rfGive;
  c->rfTake     = functions->rfTake;
  c->rfIterator = functions->rfIterator;

  *collection = c;

end:
  if (rc && c) {
    redRWLockDestroy( &(c->lock) );
    redFree( rCtx, (void**)&c, 0 );
  }

  return rc;
}


int
redCollectionDestroy(
    RedCollection* collection
    )
{
  if (!collection)
    return RED_ERR_NULL_POINTER;

  if (*collection) {
    redRWLockDestroy( &((*collection)->lock) );
    return redFree( (*collection)->rCtx, (void**)collection, 0 );
  }

  return RED_SUCCESS;
}

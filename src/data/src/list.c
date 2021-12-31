/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: list.c 18 2010-04-17 05:57:03Z jason $
 */

#include "red_list.h"
#include "_red_list.h"
#include "_red_collection.h"

#include "red_memory.h"
#include "red_return.h"

int
redListCreate(
    RedList*             list,
    redfuncObjectCompare rfCompare,
    redfuncObjectDestroy rfDestroy,
    RedContext           rCtx
    )
{
  int rc = RED_SUCCESS;

  RedCollectionFunctions fns = { 0 };
  
  RedList     l = NULL;
  RedListData d = NULL;

  if (!list)
    return RED_ERR_NULL_POINTER;
  if (*list)
    return RED_ERR_INITIALIZED_POINTER;
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;

  rc = redMalloc( rCtx, (void**)&d, sizeof(*d) );
  if (rc) goto end;
  
  d->head = NULL;
  d->tail = NULL;
  
  /* TODO: pool allocator */
  d->nodeCtx = rCtx;
  
  fns.rfCompare  = rfCompare;
  fns.rfDestroy  = rfDestroy;

  /* contains can be null, collection can use iterator + comparison
   * overload for performance */
  fns.rfContains = NULL;
  /* optional, we don't want to deal with this for a list */
  fns.rfTake     = NULL;

  /* default to giving head */
  fns.rfGive     = redListGiveHead;
  fns.rfIterator = redListIterator;

  rc = redCollectionCreate( &l, d, RED_COLLECTION_KIND_LIST, &fns, rCtx );
  if (rc) goto end;

  *list = l;

end:
  if (rc)
    redFree( rCtx, (void**)&d, 0 );

  return rc;
}

int
redListDestroy(
    RedList* list,
    int      destroyObjects
    )
{
  if (!list)
    return RED_ERR_NULL_POINTER;

  if (*list) {
    /* TODO: destroy all nodes, passing destroyObjects to them */

    redFree( (*list)->rCtx, &((*list)->data), 0 );
    redCollectionDestroy( list );
  }

  return RED_SUCCESS;
}

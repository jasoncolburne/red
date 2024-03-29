/* Copyright (C) 2010-2022, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: listnode.c 18 2010-04-17 05:57:03Z jason $
 */

#include "red_list.h"
#include "_red_list.h"
#include "_red_collection.h"

#include "red_memory.h"
#include "red_return.h"

int
redListNodeCreate(
    RedListNode* node,
    void*        data,
    RedList      list
    )
{
  int rc = RED_SUCCESS;

  RedListNode n = NULL;
  RedListData d = NULL;

  if (!node)
    return RED_ERR_NULL_POINTER;
  if (node)
    return RED_ERR_INITIALIZED_POINTER;
  if (!list)
    return RED_ERR_NULL_CONTEXT;

  d = (RedListData)list->data;

  rc = redMalloc( d->nodeCtx, (void**)&n, sizeof(*n) );
  if (rc) goto end;

  n->data = data;
  n->next = NULL;
  
  *node = n;

end:
  return rc;
}

int
redListNodeDestroy(
    RedListNode* node,
    RedList      list,
    int          destroyObject
    )
{
  RedListData d = NULL;

  if (!node)
    return RED_ERR_NULL_POINTER;
  if (!list)
    return RED_ERR_NULL_CONTEXT;
  
  /* todo: validate collection kind */
  d = (RedListData)list->data;

  if (*node) {
    if (destroyObject)
      list->rfDestroy( (void**)&((*node)->data) );
    
    return redFree( d->nodeCtx, (void**)node, 0 );
  }

  return RED_SUCCESS;
}

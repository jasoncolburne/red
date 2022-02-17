/* Copyright (C) 2010-2022, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: listiterator.c 18 2010-04-17 05:57:03Z jason $
 */

#include "red_list.h"
#include "_red_list.h"
#include "_red_collection.h"

#include "red_iterator.h"
#include "_red_iterator.h"

#include "red_return.h"

#if 0

static
int
_value(
    RedIterator iterator,
    void**      value
    )
{
  /* caller validates input sanity */
  RedListNode n = (RedListNode)iterator->node;

  if (!n)
    return RED_ERR_COLLECTION_END;

  *value = n->data;

  return RED_SUCCESS;
}

static
int
_next(
    RedIterator iterator,
    void**      value
    )
{
  /* caller validates input sanity */
  RedListNode n = (RedListNode)iterator->node;

  if (!n)
    return RED_ERR_COLLECTION_END;

  *value         = n->data;
  iterator->node = n->next;

  return RED_SUCCESS;
}



int
redListIterator(
    RedList      list,
    RedIterator* iterator
    )
{
  int rc = RED_SUCCESS;

  RedListData d = NULL;

  /* IteratorCreate will validate iterator */
  if (!list)
    return RED_ERR_NULL_CONTEXT;

  /* this should be valid unless someone is messing with the data structures */
  d = (RedListData)list->data;

  /* always lock for iterators? */
  rc = redRWLockTakeRead( list->lock );
  if (rc) goto end;

  rc = redIteratorCreate( iterator, d->head, _value, _next, list->iteratorCtx );
  if (rc) redRWLockGive( list->lock );
end:
  return rc;
}

#endif

/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: listgivetake.c 18 2010-04-17 05:57:03Z jason $
 */

#include "he_list.h"
#include "_he_list.h"
#include "_he_collection.h"

#include "he_atomic.h"
#include "he_return.h"

#if 0

int
redListGiveHead(
    RedList list,
    void*   object
    )
{
  int rc = RED_SUCCESS;

  RedListNode n       = NULL;
  RedListData d       = NULL;

  if (!list)
    return RED_ERR_NULL_POINTER;

  d = (RedListData)list->data;

  rc = redListNodeCreate( &n, object, list );
  if (rc) goto end;


end:
  return rc;
}

int
redListGiveTail(
    RedList list,
    void*   object
    )
{
  int rc = RED_SUCCESS;

  RedListNode n       = NULL;
  RedListNode oldTail = NULL;
  RedListData d       = NULL;

  if (!list)
    return RED_ERR_NULL_POINTER;

  d = (RedListData)list->data;

  rc = redListNodeCreate( &n, object, list );
  if (rc) goto end;

  /* this won't work */
  do {
    oldTail = d->tail;
    n->next = oldHead;
  } while (redAtomicCompareAndSwapPointer( oldHead, n, &(d->head) )
           == RED_FAIL_ATOMIC_COMPARISON)

end:
  return rc;
}

int
redListTakeHead(
    RedList list,
    void**  object
    )
{

}

int
redListTakeTail(
    RedList list,
    void**  object
    )
{

}

#endif

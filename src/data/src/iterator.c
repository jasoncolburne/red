/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: iterator.c 18 2010-04-17 05:57:03Z jason $
 */

#include "he_iterator.h"
#include "_he_iterator.h"

#include "he_memory.h"
#include "he_return.h"

int
redIteratorCreate(
    RedIterator*         iterator,
    void*                node,
    redfuncIteratorValue rfValue,
    redfuncIteratorNext  rfNext,
    RedContext           rCtx
    )
{
  int rc = RED_SUCCESS;

  RedIterator i = NULL;

  if (!iterator || !rfValue || !rfNext)
    return RED_ERR_NULL_POINTER;
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;
  if (*iterator)
    return RED_ERR_INITIALIZED_POINTER;

  rc = redMalloc( rCtx, (void**)&i, sizeof(*i) );
  if (rc) goto end;

  i->node    = node;
  i->rfValue = rfValue;
  i->rfNext  = rfNext;
  i->rCtx    = rCtx;

  *iterator = i;
end:
  return rc;
}


int
redIteratorDestroy(
    RedIterator* iterator
    )
{
  if (!iterator)
    return RED_ERR_NULL_POINTER;

  /* TODO: unlock collection? */
  if (*iterator)
    return redFree( (*iterator)->rCtx, (void**)iterator, 0 );

  return RED_SUCCESS;
}


int
redIteratorValue(
    RedIterator iterator,
    void**      value
    )
{
  if (!iterator)
    return RED_ERR_NULL_CONTEXT;
  if (!value)
    return RED_ERR_NULL_POINTER;

  return iterator->rfValue( iterator, value );
}


int
redIteratorNext(
    RedIterator iterator,
    void**      value
    )
{
  if (!iterator)
    return RED_ERR_NULL_CONTEXT;
  if (!value)
    return RED_ERR_NULL_POINTER;

  return iterator->rfNext( iterator, value );
}


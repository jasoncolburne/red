/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: threadattributes.c 18 2010-04-17 05:57:03Z jason $
 */

#include "red_thread.h"
#include "_red_thread.h"

#include "red_memory.h"
#include "red_return.h"

int
redThreadAttributesCreate(
    RedThreadAttributes* attr,
    RedContext           rCtx
    )
{
  int rc = RED_SUCCESS;

  RedThreadAttributes a = NULL;

  if (!attr)
    return RED_ERR_NULL_POINTER;
  if (*attr)
    return RED_ERR_INITIALIZED_POINTER;
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;

  rc = redMalloc( rCtx, (void**)&a, sizeof(*a) );
  if (!rc) {
    rc = pthread_attr_init( &(a->attr) );
    if (rc) goto end;

    a->rCtx = rCtx;

    *attr = a;
  }

end:
  if (rc)
    redFree( rCtx, (void**)&a, 0 );

  return rc;
}

int
redThreadAttributesDestroy(
    RedThreadAttributes* attr
    )
{
  if (!attr)
    return RED_ERR_NULL_POINTER;
  if (!*attr)
    return RED_ERR_NULL_CONTEXT;

  return redFree( (*attr)->rCtx, (void**)attr, 0 );
}

int
redThreadAttributesAssign(
    RedThreadAttributes attr,
    red_u32             attrId,
    void*               value,
    size_t              valueSz
    )
{
  int rc = RED_SUCCESS;

  size_t s;

  if (!attr)
    return RED_ERR_NULL_CONTEXT;

  switch (attrId)
  {
    case RED_ATTRIBUTE_THREAD_DETACHED:
      rc = pthread_attr_setdetachstate( &(attr->attr), PTHREAD_CREATE_DETACHED );

      /* TODO: wrap error */
      break;
    case RED_ATTRIBUTE_THREAD_JOINABLE:
      rc = pthread_attr_setdetachstate( &(attr->attr), PTHREAD_CREATE_JOINABLE );

      /* TODO: wrap error */
      break;
    case RED_ATTRIBUTE_THREAD_STACKSIZE:
      if (valueSz != sizeof(s))
        return RED_ERR_THREAD_ATTR_SIZE;

      s  = *(size_t*)value;
      rc = pthread_attr_setstacksize( &(attr->attr), s );

      /* TODO: wrap error */
      break;
  }

  return rc;
}

int
redThreadAttributesQuery(
    RedThreadAttributes attr,
    red_u32             attrId,
    void*               value,
    size_t*             valueSz
    )
{
  if (!attr)
    return RED_ERR_NULL_CONTEXT;
  if (!valueSz)
    return RED_ERR_NULL_POINTER;

  /* TODO */
  return RED_SUCCESS;
}

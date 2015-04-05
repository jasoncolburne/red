/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: context.c 18 2010-04-17 05:57:03Z jason $
 */

#include "red_context.h"

#include "_red_context.h"
#include "_red_memorydefault.h"

int
redContextCreateDefault(
    RedContext* ctx
    )
{
  int rc = RED_SUCCESS;

  RedContext c = NULL;

  if (!ctx)
    return RED_ERR_NULL_POINTER;
  if (*ctx)
    return RED_ERR_INITIALIZED_POINTER;

  rc = rfDefaultMalloc( (void**)&c, sizeof(*c) );
  if (!rc)
  {
    c->rfMalloc = rfDefaultMalloc;
    c->rfFree   = rfDefaultFree;
    c->rfMemcpy = rfDefaultMemcpy;
    c->rfMemset = rfDefaultMemset;
    c->rfMemcmp = rfDefaultMemcmp;

    *ctx = c;
  }

  return rc;
}

int
redContextDestroy(
    RedContext* ctx
    )
{
  if (!ctx)
    return RED_ERR_NULL_POINTER;
  if (!*ctx)
    return RED_ERR_NULL_CONTEXT;

  return ((*ctx)->rfFree)( (void**)ctx, sizeof(**ctx) );
}

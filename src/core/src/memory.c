/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: memory.c 18 2010-04-17 05:57:03Z jason $
 */

#include "red_context.h"
#include "red_memory.h"

#include "_red_context.h"


int
redMalloc(
    RedContext rCtx,
    void**           ptr,
    size_t           sz
    )
{
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;
  if (!(rCtx->rfMalloc))
    return RED_ERR_NOT_SUPPORTED;

  return rCtx->rfMalloc( ptr, sz );
}

int
redFree(
    RedContext rCtx,
    void**           ptr,
    size_t           sz
    )
{
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;
  if (!(rCtx->rfFree))
    return RED_ERR_NOT_SUPPORTED;

  return rCtx->rfFree( ptr, sz );
}

int
redMemcpy(
    RedContext  rCtx,
    void*       dst,
    const void* src,
    size_t      sz
    )
{
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;
  if (!(rCtx->rfMemcpy))
    return RED_ERR_NOT_SUPPORTED;

  return rCtx->rfMemcpy( dst, src, sz );
}

int
redMemset(
    RedContext rCtx,
    void*            dst,
    unsigned char    val,
    size_t           sz
    )
{
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;
  if (!(rCtx->rfMemset))
    return RED_ERR_NOT_SUPPORTED;

  return rCtx->rfMemset( dst, val, sz );
}

int
redMemcmp(
    RedContext rCtx,
    const void*      s1,
    const void*      s2,
    size_t           sz,
    int*             result
    )
{
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;
  if (!(rCtx->rfMemcmp))
    return RED_ERR_NOT_SUPPORTED;
  
  return rCtx->rfMemcmp( s1, s2, sz, result );
}

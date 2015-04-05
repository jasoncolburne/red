/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: stream.c 19 2010-04-22 08:27:08Z jason $
 */

#include "he_return.h"
#include "he_memory.h"
#include "he_string.h"

#include "he_stream.h"
#include "_he_stream.h"

static
int
rfStreamDestroyDefault(
    RedStream* stream
    )
{
  /* wrapper guarantees stream and *stream */
  return redFree( (*stream)->rCtx, (void**)stream, 0 );
}

int
redStreamCreate(
    RedStream*           stream,
    red_u32              flags,
    void*                data,
    redfuncStreamTake    rfTake,
    redfuncStreamGive    rfGive,
    redfuncStreamDestroy rfDestroy,
    RedStream            source,
    RedStream            sink,
    RedContext           rCtx
    )
{
  int rc = RED_SUCCESS;

  RedStream s = NULL;

  if (!stream)
    return RED_ERR_NULL_POINTER;
  if (*stream)
    return RED_ERR_INITIALIZED_POINTER;
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;

  rc = redMalloc( rCtx, (void**)&s, sizeof(*s) );
  if (!rc)
  {
    s->flags  = flags;

    s->data   = data;
    
    s->rfTake = rfTake;
    s->rfGive = rfGive;

    if (rfDestroy)
      s->rfDestroy = rfDestroy;
    else
      s->rfDestroy = rfStreamDestroyDefault;

    s->source = source;
    s->sink   = sink;
    
    s->rCtx   = rCtx;

    *stream = s;
  }

  return rc;
}


int
redStreamDestroy(
    RedStream* stream
    )
{
  if (!stream)
    return RED_ERR_NULL_POINTER;

  if (*stream)
    return (*stream)->rfDestroy( stream );

  /* TODO: should this return RED_ERR_NULL_CONTEXT? most times it will be
   * ignored and could help identify a programmer error */
  return RED_SUCCESS;
}

int
redStreamTake(
    RedStream stream,
    void*     dst,
    size_t*   count
    )
{
  if (!stream)
    return RED_ERR_NULL_CONTEXT;

  /* TODO further checks? let callback handle it? */

  if (stream->rfTake)
    return stream->rfTake( stream, dst, count );

  return RED_ERR_UNSUPPORTED_BY_STREAM;
}

int
redStreamGive(
    RedStream stream,
    void*     src,
    size_t    count
    )
{
  if (!stream)
    return RED_ERR_NULL_CONTEXT;

  /* TODO further checks? let callback handle it? */

  if (stream->rfGive)
    return stream->rfGive( stream, src, count );

  return RED_ERR_UNSUPPORTED_BY_STREAM;
}

int
redStreamDataGet(
    RedStream stream,
    void**    data
    )
{
  if (!stream)
    return RED_ERR_NULL_CONTEXT;
  if (!data)
    return RED_ERR_NULL_POINTER;

  *data = stream->data;

  return RED_SUCCESS;
}

int
redStreamPrint(
    RedStream   stream,
    size_t      maximum,
    const char* format,
    ...
    )
{
  int rc = RED_SUCCESS;

  /* slow implementation for now */
  char*   buffer = NULL;
  va_list ap;
  size_t  bytes;

  if (!stream)
    return RED_ERR_NULL_CONTEXT;
  if (!format)
    return RED_ERR_NULL_POINTER;

  if (!maximum)
    maximum = RED_STREAM_PRINT_DEFAULT_MAXIMUM;

  /* TODO: this should be scrapped in favour of tracing */
  rc = redMalloc( stream->rCtx, (void**)&buffer, maximum );
  if (rc) goto end;

  va_start( ap, format );
  /* bytes can be bigger than maximum here and create a nasty bug */
  bytes = (size_t)redVsnPrintf( buffer, maximum, format, ap );
  va_end( ap );

  rc = redStreamGive( stream, buffer, bytes + 1 );
end:
  redFree( stream->rCtx, (void**)&buffer, 0 );

  return rc;
}


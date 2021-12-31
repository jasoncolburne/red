/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: stringbuffer.c 19 2010-04-22 08:27:08Z jason $
 */

#include "red_stringbuffer.h"
#include "_red_stringbuffer.h"

#include "red_memory.h"
#include "red_return.h"
#include "red_string.h"

int
redStringBufferCreate(
    RedStringBuffer* buffer,
    size_t           size,
    RedContext       rCtx
    )
{
  int rc = RED_SUCCESS;

  RedStringBuffer b = NULL;

  if (!buffer)
    return RED_ERR_NULL_POINTER;
  if (size < 32)
    return RED_ERR_INVALID_ARGUMENT;
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;

  rc = redMalloc( rCtx, (void**)&b, sizeof(*b) );
  if (rc) goto end;

  rc = redResourcePoolCreateNonblocking( &(b->pairpool),
                                         sizeof(struct _RedStringBufferPair),
                                         size / 32, 0, NULL, NULL, NULL, rCtx );
  if (rc) goto end;

  rc = redSpinlockCreate( &(b->lock), rCtx );
  if (rc) goto end;

  rc = redQueueCreateNonblocking( &(b->pairs), size / 32, 0, rCtx );
  if (rc) goto end;

  rc = redMalloc( rCtx, (void**)&(b->buffer), size );
  if (rc) goto end;

  /*
  b->head  = 0;
  b->tail  = 0;
  b->full  = 0;
  b->given = NULL;
   */
  
  b->size = size;
  b->rCtx = rCtx;

  *buffer = b;
end:
  if (rc && b) {
    redFree( rCtx, (void**)&(b->buffer), 0 );
    redQueueDestroy( &(b->pairs) );
    redSpinlockDestroy( &(b->lock) );
    redResourcePoolDestroy( &(b->pairpool) );

    redFree( rCtx, (void**)&b, 0 );
  }

  return rc;
}

int
redStringBufferDestroy(
    RedStringBuffer* buffer
    )
{
  if (!buffer)
    return RED_ERR_NULL_POINTER;

  if (*buffer) {
    RedStringBuffer b = *buffer;

    redFree( b->rCtx, (void**)&(b->buffer), 0 );
    redQueueDestroy( &(b->pairs) );
    redSpinlockDestroy( &(b->lock) );
    redResourcePoolDestroy( &(b->pairpool) );

    redFree( b->rCtx, (void**)buffer, 0 );
  }

  return RED_SUCCESS;
}

int
redStringBufferPrintf(
    RedStringBuffer buffer,
    const char*     format,
    ...
    )
{
  int rc = RED_SUCCESS;

  va_list ap;

  va_start( ap, format );
  rc = redStringBufferVPrintf( buffer, format, ap );
  va_end( ap );

  return rc;
}

int
redStringBufferVPrintf(
    RedStringBuffer buffer,
    const char*     format,
    va_list         ap
    )
{
  int rc = RED_SUCCESS;
  int n, i;

  char* p;

  size_t newTail;

  RedStringBufferPair pair = NULL;

  if (!buffer)
    return RED_ERR_NULL_CONTEXT;
  /* TODO: find out what ap really looks like and if this is sane everywhere */
  if (!format || !ap)
    return RED_ERR_INVALID_ARGUMENT;

  n = redVsnPrintf( NULL, 0, format, ap );
  if (n < 0) {
    /* TODO: errno */
    return 0x666;
  }

  rc = redResourcePoolTake( buffer->pairpool, (void**)&pair );
  if (rc) return rc;

  rc = redSpinlockTake( buffer->lock );
  if (rc) {
    redResourcePoolGive( buffer->pairpool, pair );
    return rc;
  }

  if ((buffer->size <= n) ||
      (buffer->full) ||
      (buffer->head < buffer->tail &&
       buffer->head <= n &&
       buffer->size - buffer->tail <= n) ||
      (buffer->head > buffer->tail &&
       buffer->head - buffer->tail <= n)) {
    rc = RED_FAIL_COLLECTION_FULL;
    goto end;
  }

  if (buffer->head <= buffer->tail) {
    if (buffer->size - buffer->tail > n) {
      p = buffer->buffer + buffer->tail;
      newTail = buffer->tail + n + 1;
    }
    else { /* buffer->head > n asserted above */
      p = buffer->buffer;
      newTail = n + 1;
    }
  }
  else {
    p = buffer->buffer + buffer->tail;
    newTail = buffer->tail + n + 1;
  }

  i = redVsnPrintf( p, n + 1, format, ap );
  if (i < 0 || i != n) {
    /* TODO: errno */
    rc = 0x666;
    goto end;
  }

  pair->string = p;
  pair->length = n;

  rc = redQueueGive( buffer->pairs, pair );
  if (rc) goto end;

  buffer->tail = newTail;

  if (buffer->tail >= buffer->size)
    buffer->tail = 0;

  if (buffer->tail == buffer->head)
    buffer->full = 1;

end:
  if (rc && pair)
    redResourcePoolGive( buffer->pairpool, pair );

  redSpinlockGive( buffer->lock );

  return rc;
}

int
redStringBufferGive(
    RedStringBuffer buffer,
    const char*     src,
    size_t          sz
    )
{
  int rc = RED_SUCCESS;

  char* p;

  size_t newTail;

  RedStringBufferPair pair = NULL;

  if (!buffer)
    return RED_ERR_NULL_CONTEXT;
  /* TODO: find out what ap really looks like and if this is sane everywhere */
  if (!src)
    return RED_ERR_NULL_POINTER;
  if (!sz)
    return RED_ERR_INVALID_ARGUMENT;

  rc = redResourcePoolTake( buffer->pairpool, (void**)&pair );
  if (rc) return rc;

  rc = redSpinlockTake( buffer->lock );
  if (rc) {
    redResourcePoolGive( buffer->pairpool, pair );
    return rc;
  }

  if ((buffer->size <= sz) ||
      (buffer->full) ||
      (buffer->head < buffer->tail &&
       buffer->head <= sz &&
       buffer->size - buffer->tail <= sz) ||
      (buffer->head > buffer->tail &&
       buffer->head - buffer->tail <= sz)) {
    rc = RED_FAIL_COLLECTION_FULL;
    goto end;
  }

  if (buffer->head <= buffer->tail) {
    if (buffer->size - buffer->tail > sz) {
      p = buffer->buffer + buffer->tail;
      newTail = buffer->tail + sz + 1;
    }
    else { /* buffer->head > n asserted above */
      p = buffer->buffer;
      newTail = sz + 1;
    }
  }
  else {
    p = buffer->buffer + buffer->tail;
    newTail = buffer->tail + sz + 1;
  }

  rc = redMemcpy( buffer->rCtx, p, src, sz + 1 );
  if (rc) goto end;

  pair->string = p;
  pair->length = sz;

  rc = redQueueGive( buffer->pairs, pair );
  if (rc) goto end;

  buffer->tail = newTail;

  if (buffer->tail >= buffer->size)
    buffer->tail = 0;

  if (buffer->tail == buffer->head)
    buffer->full = 1;

end:
  if (rc && pair)
    redResourcePoolGive( buffer->pairpool, pair );

  redSpinlockGive( buffer->lock );

  return rc;
}

int
redStringBufferTake(
    RedStringBuffer buffer,
    char**          src,
    size_t*         size
    )
{
  return RED_NOT_IMPLEMENTED;
}

int
redStringBufferAdvance(
    RedStringBuffer buffer
    )
{
  return RED_NOT_IMPLEMENTED;
}

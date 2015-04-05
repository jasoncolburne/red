/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_stringbuffer.h 19 2010-04-22 08:27:08Z jason $
 */

#ifndef __HE_STRINGBUFFER_H__
#define __HE_STRINGBUFFER_H__


#include "he_context.h"

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

struct _RedStringBuffer;
typedef struct _RedStringBuffer* RedStringBuffer;

extern
int
redStringBufferCreate(
    RedStringBuffer* buffer,
    size_t           size,
    RedContext       rCtx
    );

extern
int
redStringBufferDestroy(
    RedStringBuffer* buffer
    );

/* this is safe, vsnprintf is used internally */
extern
int
redStringBufferPrintf(
    RedStringBuffer buffer,
    const char*     format,
    ...
    );

/* this is safe, vsnprintf is used internally */
extern
int
redStringBufferVPrintf(
    RedStringBuffer buffer,
    const char*     format,
    va_list         ap
    );

/* this will perform a memcpy with the rCtx provided during buffer creation */
/* expects sz to NOT include the null terminator (sz == strlen(src)) 
 * sz + 1 bytes will be copied */
extern
int
redStringBufferGive(
    RedStringBuffer buffer,
    const char*     src,
    size_t          sz
    );

/* this won't actually advance the buffer, just give you the string directly */
extern
int
redStringBufferTake(
    RedStringBuffer buffer,
    char**          src,
    size_t*         size
    );

/* this will discard the head and advance the queue */
extern
int
redStringBufferAdvance(
    RedStringBuffer buffer
    );


#if 0
  /* can write at end - bytes + 1 is considered due to strict > */
  if ((trace->ringHead <= trace->ringTail &&
       trace->ringSize -  trace->ringTail > bytes) ||
      (trace->ringHead >  trace->ringTail &&
       trace->ringHead -  trace->ringTail > bytes) {
    offset = trace->ringTail;
  }
  /* must write to beginning */
  else if (trace->ringHead <= trace->ringTail &&
           trace->ringHead > bytes) {
    offset = 0;
  }
  /* cannot write! not enough space! */
  else {
    redSpinlockGive( trace->ringLock );
    rc = RED_FAIL_COLLECTION_FULL;
    goto end;
  }

  n = redVsnPrintf( NULL, 0, format, ap );
  if (n < 0) {
    /* TODO: wrap errno */
    rc = 0x666;
    goto end;
  }

  m = redVsnPrintf( p, n + 1, format, ap );


#endif


#ifdef __cplusplus
}
#endif


#endif /* __HE_STRINGBUFFER_H__ */

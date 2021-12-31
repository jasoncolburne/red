/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_trace.h 19 2010-04-22 08:27:08Z jason $
 */

#ifndef ___RED_TRACE_H__
#define ___RED_TRACE_H__

#include "red_trace.h"

#include "red_context.h"
#include "red_queue.h"
#include "red_stringbuffer.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 32k default ringbuffer size */
#define RED_TRACE_STRINGBUFFER_DEFAULT_SIZE     0x8000
#define RED_TRACE_PREFIX_AND_FORMAT_BUFFER_SIZE 0x0800

typedef int   (*redfuncTraceWrite)( void*, const void*, size_t );
typedef int (*redfuncTraceDestroy)( void** );


struct _RedTraceContext {
  void* privateData;

  char identity[32];

  RedQueue globalQueue;

  red_u32 priorityMask;
  red_u32 flags;

  redfuncTraceWrite   rfWrite;
  redfuncTraceDestroy rfDestroy;

  RedStringBuffer messages;

  RedContext rCtx;
};

/* implementation lives in src/core/src/initialize.c to protect global var */
int
redGlobalContextTraceQueueGet(
    RedQueue* traceQueue
    );


#ifdef __cplusplus
}
#endif


#endif /* ___RED_TRACE_H__ */

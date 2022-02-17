/* Copyright (C) 2010-2022, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: trace.c 19 2010-04-22 08:27:08Z jason $
 */

#include "red_trace.h"
#include "_red_trace.h"

#include "red_return.h"
#include "red_stream.h"
#include "red_string.h"
#include "red_time.h"

#include <stdarg.h>
#include <time.h>

static
char*
_tracePriorityAsString(
    red_u32 priority
    )
{
  char* rc = NULL;
  
  if (priority & RED_TRACE_PRIORITY_TRACE)
    rc = "trace";
  if (priority & RED_TRACE_PRIORITY_DEBUG)
    rc = "debug";
  if (priority & RED_TRACE_PRIORITY_INFO)
    rc = "info";
  if (priority & RED_TRACE_PRIORITY_NOTICE)
    rc = "notice";
  if (priority & RED_TRACE_PRIORITY_WARNING)
    rc = "warning";
  if (priority & RED_TRACE_PRIORITY_ERROR)
    rc = "error";
  if (priority & RED_TRACE_PRIORITY_CRITICAL)
    rc = "critical";
  if (priority & RED_TRACE_PRIORITY_ALERT)
    rc = "alert";
  if (priority & RED_TRACE_PRIORITY_PANIC)
    rc = "panic";
  if (!rc)
    rc = "unknown";

  return rc;
}

int
redTraceContextCreate(
    RedTraceContext*    trace,
    const char*         identity,
    red_u32             flags,
    red_u32             minPriority,
    void*               privateData,
    redfuncTraceWrite   rfWrite,
    redfuncTraceDestroy rfDestroy,
    RedContext          rCtx
    )
{
  int rc = RED_SUCCESS;

  RedTraceContext t = NULL;

  if (!trace || !privateData)
    return RED_ERR_NULL_POINTER;
  if (*trace)
    return RED_ERR_INITIALIZED_POINTER;
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;

  /* unused for now */
  (void)flags;

  rc = redMalloc( rCtx, (void**)&t, sizeof(*t) );
  if (rc) goto end;

  t->privateData = privateData;
  t->rCtx        = rCtx;
  t->rfWrite     = rfWrite;
  t->rfDestroy   = rfDestroy;
  t->flags       = flags;

  if (identity)
    redStrncpy( t->identity, identity, sizeof(t->identity) - 1 );

  rc = redStringBufferCreate( &(t->messages),
                              RED_TRACE_STRINGBUFFER_DEFAULT_SIZE, rCtx );
  if (rc) goto end;

  rc = redTracePrioritySet( t, minPriority );
  if (rc) goto end;

  rc = redGlobalContextTraceQueueGet( &(t->globalQueue) );
  if (rc) goto end;

  *trace = t;

end:
  if (rc && t) {
    redStringBufferDestroy( &(t->messages) );
    redFree( rCtx, (void**)&t, 0 );
  }

  return rc;
}

static
int
traceStreamGive(
    void*       s,
    const void* d,
    size_t      size
    )
{
  RedStream stream = (RedStream)s;

  return redStreamGive( stream, (void*)d, size );
}

int
redTraceContextCreateFromStream(
    RedTraceContext* trace,
    const char*      identity,
    red_u32          flags,
    red_u32          minPriority,
    RedStream        stream,
    RedContext       rCtx
    )
{
  return redTraceContextCreate( trace, identity, flags, minPriority, stream,
                                traceStreamGive, NULL, rCtx );
}

int
redTraceContextCreateFromSyslog(
    RedTraceContext* trace,
    const char*      identity,
    red_u32          flags,
    red_u32          minPriority,
    RedContext       rCtx
    )
{
  return RED_NOT_IMPLEMENTED;
}


int
redTraceContextDestroy(
    RedTraceContext* trace
    )
{
  if (!trace)
    return RED_ERR_NULL_POINTER;

  if (*trace) {
    if ((*trace)->rfDestroy)
      (*trace)->rfDestroy( (void**)&((*trace)->privateData) );

    redStringBufferDestroy( &((*trace)->messages) );
    redFree( (*trace)->rCtx, (void**)trace, 0 );
  }

  return RED_SUCCESS;
}


int
redTracePrioritySet(
    RedTraceContext trace,
    red_u32         minPriority
    )
{
  red_u32 n = 1;

  if (!trace)
    return RED_ERR_NULL_CONTEXT;

  if (!minPriority) {
    trace->priorityMask = 0xffffffff;
    return RED_SUCCESS;
  }

  while (!(minPriority & 1)) {
    minPriority >>= 1;
    n           <<= 1;
  }

  trace->priorityMask = 0;

  while (n) {
    trace->priorityMask |= n;
    n <<= 1;
  }

  return RED_SUCCESS;
}

int
redTracePrioritySetMask(
    RedTraceContext trace,
    red_u32         mask
    )
{
  if (!trace)
    return RED_ERR_NULL_CONTEXT;

  trace->priorityMask = mask;

  return RED_SUCCESS;
}

static
int
_traceConstructFormatString(
    RedTraceContext trace,
    red_u32         priority,
    const char*     filename,
    int             line,
    const char*     in,
    char*           out,
    size_t          maximum
    )
{
  int rc = RED_SUCCESS;

  int   n;
  int   left = maximum;

  char* p    = out;

  if (trace->flags & RED_TRACE_FLAG_SHOW_TIMESTAMP) {
    struct tm  tm;
    struct tm* t;
    time_t     now = time( NULL );

    if (trace->flags & RED_TRACE_FLAG_SHOW_UTC)
      t = gmtime_r( &now, &tm );
    else
      t = localtime_r( &now, &tm );

    if (t != &tm) {
      /* TODO: errno */
      rc = 0x666;
      goto end;
    }

    n = (int)strftime( p, (size_t)left, "%Y-%m-%d %H:%M:%S ", &tm );
    if (!n) {
      /* TODO: error */
      rc = 0x666;
      goto end;
    }

    left -= n;
    p    += n;
  }
  
  if (*(trace->identity) && (trace->flags & RED_TRACE_FLAG_SHOW_IDENTITY)) {
    n = redSnPrintf( p, left, "%s: ", trace->identity );
    if (n < 0 || n >= left) {
      /* TODO: errno */
      rc = 0x666;
      goto end;
    }

    left -= n;
    p    += n;
  }
  
  if (trace->flags & RED_TRACE_FLAG_SHOW_PRIORITY) {
    n = redSnPrintf( p, left, "[%s] ", _tracePriorityAsString(priority) );
    if (n < 0 || n >= left) {
      /* TODO: errno */
      rc = 0x666;
      goto end;
    }

    left -= n;
    p    += n;
  }

  if (filename && (trace->flags & RED_TRACE_FLAG_SHOW_FILE)) {
    n = redSnPrintf( p, left, "(%s:%d) ", filename, line );
    if (n < 0 || n >= left) {
      /* TODO: errno */
      rc = 0x666;
      goto end;
    }

    left -= n;
    p    += n;
  }

  n = redSnPrintf( p, left, "%s", in );
  if (n < 0 || n >= left) {
    /* TODO: errno */
    rc = 0x666;
    goto end;
  }

end:
  return rc;
}


int
redTracePrintf(
    RedTraceContext trace,
    red_u32         priority,
    const char*     filename,
    int             line,
    const char*     format,
    ...
    )
{
  int rc = RED_SUCCESS;

  va_list ap;
  char    fmt[RED_TRACE_PREFIX_AND_FORMAT_BUFFER_SIZE];
  char*   p;

  if (!trace)
    return RED_ERR_NULL_CONTEXT;
  if (!format)
    return RED_ERR_NULL_POINTER;

  /* priority didn't hit the mask */
  if (!(priority & trace->priorityMask))
    return RED_SUCCESS;

  if (trace->flags) {
    rc = _traceConstructFormatString( trace, priority, filename,
                                      line, format, fmt,
                                      RED_TRACE_PREFIX_AND_FORMAT_BUFFER_SIZE );
    if (rc) goto end;

    p = fmt;
  }
  else
    p = (char*)format;
  
  va_start( ap, format );
  rc = redStringBufferVPrintf( trace->messages, p, ap );
  va_end( ap );
  if (rc) goto end;

  rc = redQueueGive( trace->globalQueue, trace );
  if (rc) goto end;

end:
  return rc;
}

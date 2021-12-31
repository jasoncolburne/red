/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_trace.h 19 2010-04-22 08:27:08Z jason $
 */

#ifndef __RED_TRACE_H__
#define __RED_TRACE_H__



#include "red_context.h"

#include "red_stream.h"
#include "red_file.h"

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif


struct _RedTraceContext;
typedef struct _RedTraceContext* RedTraceContext;


#define RED_TRACE_PRIORITY_PANIC    0x01000000
#define RED_TRACE_PRIORITY_ALERT    0x00200000
#define RED_TRACE_PRIORITY_CRITICAL 0x00040000
#define RED_TRACE_PRIORITY_ERROR    0x00008000
#define RED_TRACE_PRIORITY_WARNING  0x00001000
#define RED_TRACE_PRIORITY_NOTICE   0x00000200
#define RED_TRACE_PRIORITY_INFO     0x00000040
#define RED_TRACE_PRIORITY_DEBUG    0x00000008
#define RED_TRACE_PRIORITY_TRACE    0x00000001
#define RED_TRACE_PRIORITY_NONE     0x00000000


#define redTrPanic( trace, format, ... ) \
  redTracePrintf( trace, RED_TRACE_PRIORITY_PANIC,    __FILE__, __LINE__, \
                  format, ## __VA_ARGS__ )
#define redTrAlert( trace, format, ... ) \
  redTracePrintf( trace, RED_TRACE_PRIORITY_ALERT,    __FILE__, __LINE__, \
                  format, ## __VA_ARGS__ )
#define redTrCritical( trace, format, ... ) \
  redTracePrintf( trace, RED_TRACE_PRIORITY_CRITICAL, __FILE__, __LINE__, \
                  format, ## __VA_ARGS__ )
#define redTrError( trace, format, ... ) \
  redTracePrintf( trace, RED_TRACE_PRIORITY_ERROR,    __FILE__, __LINE__, \
                  format, ## __VA_ARGS__ )
#define redTrWarning( trace, format, ... ) \
  redTracePrintf( trace, RED_TRACE_PRIORITY_WARNING,  __FILE__, __LINE__, \
                  format, ## __VA_ARGS__ )
#define redTrNotice( trace, format, ... ) \
  redTracePrintf( trace, RED_TRACE_PRIORITY_NOTICE,   __FILE__, __LINE__, \
                  format, ## __VA_ARGS__ )
#define redTrInfo( trace, format, ... ) \
  redTracePrintf( trace, RED_TRACE_PRIORITY_INFO,     __FILE__, __LINE__, \
                  format, ## __VA_ARGS__ )
#define redTrDebug( trace, format, ... ) \
  redTracePrintf( trace, RED_TRACE_PRIORITY_DEBUG,    __FILE__, __LINE__, \
                  format, ## __VA_ARGS__ )
#define redTrTrace( trace, format, ... ) \
  redTracePrintf( trace, RED_TRACE_PRIORITY_TRACE,    __FILE__, __LINE__, \
                  format, ## __VA_ARGS__ )


#define RED_TRACE_FLAG_SHOW_TIMESTAMP 0x0001
#define RED_TRACE_FLAG_SHOW_PRIORITY  0x0002
#define RED_TRACE_FLAG_SHOW_FILE      0x0004
#define RED_TRACE_FLAG_SHOW_UTC       0x0008
#define RED_TRACE_FLAG_SHOW_IDENTITY  0x0010

/* identity will be truncated at 31 characters */
extern
int
redTraceContextCreateFromStream(
    RedTraceContext* trace,
    const char*      identity,
    red_u32          flags,
    red_u32          minPriority,
    RedStream        stream,
    RedContext       rCtx
    );

/* identity will be truncated at 31 characters */
extern
int
redTraceContextCreateFromSyslog(
    RedTraceContext* trace,
    const char*      identity,
    red_u32          flags,
    red_u32          minPriority,
    RedContext       rCtx
    );

extern
int
redTraceContextDestroy(
    RedTraceContext* trace
    );

extern
int
redTracePrioritySet(
    RedTraceContext trace,
    red_u32         minPriority
    );

extern
int
redTracePrioritySetMask(
    RedTraceContext trace,
    red_u32         mask
    );

extern
int
redTracePrintf(
    RedTraceContext trace,
    red_u32         priority,
    const char*     filename,
    int             line,
    const char*     format,
    ...
    );

extern
int
redTracePrintHex(
    RedTraceContext trace,
    red_u32         priority,
    void*           data,
    size_t          bytes
    );


#ifdef __cplusplus
}
#endif


#endif /* __RED_TRACE_H__ */

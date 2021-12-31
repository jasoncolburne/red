/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_stream.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef __RED_STREAM_H__
#define __RED_STREAM_H__


#include "red_context.h"
#include "red_file.h"
#include "red_memory.h"

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

struct _RedStream;
typedef struct _RedStream* RedStream;

typedef int    (*redfuncStreamTake)( RedStream, void*, size_t* );
typedef int    (*redfuncStreamGive)( RedStream, void*, size_t );
typedef int (*redfuncStreamDestroy)( RedStream* );
/* flush? */

typedef enum {
  RED_STREAM_DEFAULT     = 0x0000,
  RED_STREAM_NONBLOCKING = 0x0001,
  RED_STREAM_BLOCKING    = 0x0002
} RedStreamFlags;

extern
int
redStreamCreate(
    RedStream*           stream,
    RedStreamFlags       flags,
    void*                data,
    redfuncStreamTake    rfTake,
    redfuncStreamGive    rfGive,
    redfuncStreamDestroy rfDestroy,
    RedStream            source,
    RedStream            sink,
    RedContext           rCtx
    );


extern
int
redStreamDestroy(
    RedStream* stream
    );


extern
int
redStreamTake(
    RedStream stream,
    void*     dst,
    size_t*   count
    );

extern
int
redStreamGive(
    RedStream stream,
    void*     src,
    size_t    count
    );

extern
int
redStreamDataGet(
    RedStream stream,
    void**    data
    );


/* when 0 is supplied for maximum, default to 16kb */
#define RED_STREAM_PRINT_DEFAULT_MAXIMUM 0x4000

extern
int
redStreamPrint(
    RedStream   stream,
    size_t      maximum,
    const char* format,
    ...
    );



/* stream implementations */
extern
int
redStreamCreateFromFile(
    RedStream*    stream,
    RedFile       file,
    RedFreePolicy filePolicy,
    RedContext    rCtx
    );

/* typically opens stdout for output and stdin for input where supported */
extern
int
redStreamCreateConsole(
    RedStream* stream,
    RedContext rCtx
    );

/* typically opens stderr for output on platforms that support it */
extern
int
redStreamCreateConsoleError(
    RedStream* stream,
    RedContext rCtx
    );

/* get an rng.. accepts RED_STREAM_NONBLOCKING, defaults to RED_STREAM_BLOCKING
 * which in linux correspond to /dev/urandom and /dev/random respectively */
extern
int
redStreamCreateRNG(
    RedStream*     stream,
    RedStreamFlags flags,
    RedContext     rCtx
    );

/* not implemented yet - maybe just allow inclusion of private header to simplify

extern
int
redStreamSetSource(
    RedStream stream,
    RedStream source
    );

extern
int
redStreamGetSource(
    RedStream  stream,
    RedStream* source
    );


extern
int
redStreamSetSink(
    RedStream stream,
    RedStream sink
    );

extern
int
redStreamGetSink(
    RedStream  stream,
    RedStream* sink
    );


extern
int
redStreamSetFlags(
    RedStream stream,
    red_u32   flags
    );

extern
int
redStreamGetFlags(
    RedStream stream,
    red_u32*  flags
    );

*/
         
#ifdef __cplusplus
}
#endif


#endif /* __RED_STREAM_H__ */

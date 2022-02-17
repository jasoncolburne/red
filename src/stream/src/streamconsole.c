/* Copyright (C) 2010-2022, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: streamconsole.c 18 2010-04-17 05:57:03Z jason $
 */

#include "red_stream.h"
#include "_red_stream.h"

#include "red_memory.h"
#include "red_return.h"

struct _RedStreamConsoleData {
  RedFile in;
  RedFile out;
};
typedef struct _RedStreamConsoleData* RedStreamConsoleData;

static
int
rfStreamDestroyConsole(
    RedStream* stream
    )
{
  /* we know stream/stream are valid */
  RedStreamConsoleData data = (*stream)->data;

  if (data) {
    redFileClose( &(data->in) );
    redFileClose( &(data->out) );
    redFree( (*stream)->rCtx, (void**)&data, 0 );
    (*stream)->data = NULL;
  }

  return redFree( (*stream)->rCtx, (void**)stream, 0 );
}

static
int
rfStreamGiveConsole(
    RedStream stream,
    void*     data,
    size_t    count
    )
{
  RedStreamConsoleData consoleData = stream->data;

  /* this can only happen if some fool corrupts the stream context */
  if (!consoleData)
    return RED_ERR_NULL_POINTER;

  /* FileRead will validate data/count */
  return redFileWrite( consoleData->out, data, count );
}

static
int
rfStreamTakeConsole(
    RedStream stream,
    void*     data,
    size_t*   count
    )
{
  RedStreamConsoleData consoleData = stream->data;

  /* this can only happen if some fool corrupts the stream context */
  if (!consoleData)
    return RED_ERR_NULL_POINTER;

  /* FileRead will validate data/count */
  return redFileRead( consoleData->in, data, count );
}


int
redStreamCreateConsole(
    RedStream* stream,
    RedContext rCtx
    )
{
  int rc = RED_SUCCESS;

  RedStreamConsoleData data = NULL;

  rc = redMalloc( rCtx, (void**)&data, sizeof(*data) );
  if (rc) goto end;

  rc = redFileOpenStdout( &(data->out), rCtx );
  if (rc) goto end;

  rc = redFileOpenStdin( &(data->in), rCtx );
  if (rc) goto end;
  
  rc = redStreamCreate( stream, 0, data, rfStreamTakeConsole,
                        rfStreamGiveConsole, rfStreamDestroyConsole, NULL,
                        NULL, rCtx );
end:
  if (rc) {
    if (data) {
      redFileClose( &(data->in) );
      redFileClose( &(data->out) );
      redFree( rCtx, (void**)&data, 0 );
    }
  }

  return rc;
}

int
redStreamCreateConsoleError(
    RedStream* stream,
    RedContext rCtx
    )
{
  int rc = RED_SUCCESS;

  RedStreamConsoleData data = NULL;

  rc = redMalloc( rCtx, (void**)&data, sizeof(*data) );
  if (rc) goto end;

  rc = redFileOpenStderr( &(data->out), rCtx );
  if (rc) goto end;

  rc = redStreamCreate( stream, 0, data, NULL, rfStreamGiveConsole,
                        rfStreamDestroyConsole, NULL, NULL, rCtx );
end:
  if (rc) {
    if (data) {
      redFileClose( &(data->out) );
      redFree( rCtx, (void**)&data, 0 );
    }
  }

  return rc;
}

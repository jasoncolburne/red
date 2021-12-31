/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: streamfile.c 19 2010-04-22 08:27:08Z jason $
 */

#include "red_stream.h"
#include "_red_stream.h"

#include "red_memory.h"
#include "red_return.h"

static
int
rfStreamDestroyFile(
    RedStream* stream
    )
{
  /* wrapper guarantees stream and *stream */
  /* TODO: how do we handle this.. (return wise) */
  redFileClose( (RedFile*)&((*stream)->data) );

  return redFree( (*stream)->rCtx, (void**)stream, 0 );
}

static
int
rfTakeFile(
    RedStream stream,
    void*     dst,
    size_t*   count
    )
{
  /* we know stream isn't NULL (wrapper verified) */
  RedFile file = (RedFile)stream->data;

  /* FileRead() will validate all passed vars, no need to do it here */
  return redFileRead( file, dst, count );
}

static
int
rfGiveFile(
    RedStream stream,
    void*     src,
    size_t    count
    )
{
  /* we know stream isn't NULL (wrapper verified) */
  RedFile file = (RedFile)stream->data;

  /* FileWrite() will validate all passed vars, no need to do it here */
  return redFileWrite( file, src, count );
}

int
redStreamCreateFromFile(
    RedStream*    stream,
    RedFile       file,
    RedFreePolicy filePolicy,
    RedContext    rCtx
    )
{
  if (!file)
    return RED_ERR_NULL_CONTEXT;

  redfuncStreamDestroy rf = NULL;
  
  if (filePolicy == RED_FREE_POLICY_ASSUME_OWNERSHIP)
    rf = rfStreamDestroyFile;

  return redStreamCreate( stream, 0, file, rfTakeFile, rfGiveFile, rf,
                          NULL, NULL, rCtx );
}

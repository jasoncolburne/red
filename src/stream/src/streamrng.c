/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: streamrng.c 18 2010-04-17 05:57:03Z jason $
 */

#include "red_stream.h"
#include "red_return.h"
#include "red_file.h"

int
redStreamCreateRNG(
    RedStream*     stream,
    RedStreamFlags flags,
    RedContext     rCtx
    )
{
  int rc = RED_SUCCESS;

  RedFile fd = NULL;

  if (flags & RED_STREAM_NONBLOCKING && flags & RED_STREAM_BLOCKING)
    return RED_ERR_INVALID_FLAG;

  /* TODO: change file open flags when implemented */
  if (flags & RED_STREAM_NONBLOCKING)
    rc = redFileOpen( &fd, 0, "/dev/urandom", rCtx );
  else
    rc = redFileOpen( &fd, 0, "/dev/random", rCtx );
  if (rc) goto end;

  rc = redStreamCreateFromFile( stream, fd, RED_FREE_POLICY_ASSUME_OWNERSHIP,
                                rCtx );
end:
  return rc;
}


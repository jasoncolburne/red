/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: fileread.c 18 2010-04-17 05:57:03Z jason $
 */

#include "red_file.h"
#include "_red_file.h"
#include "red_memory.h"
#include "red_return.h"

int
redFileRead(
    RedFile file,
    void*   data,
    size_t* bytes
    )
{
  if (!file)
    return RED_ERR_NULL_CONTEXT;
  if (!data || !bytes)
    return RED_ERR_NULL_POINTER;
  if (!*bytes)
    return RED_SUCCESS; /* TODO: do we want to return an error? */

  if (read( file->fd, data, *bytes ) == -1) {
    /* TODO: something */
    return 0x666;
  }

  return RED_SUCCESS;
}

int
redFileReadAlloc(
    RedFile file,
    void**  data,
    size_t* bytes
    )
{
  int rc = RED_SUCCESS;

  void* d = NULL;

  if (!file)
    return RED_ERR_NULL_CONTEXT;
  if (!data || !bytes)
    return RED_ERR_NULL_POINTER;
  if (*data)
    return RED_ERR_INITIALIZED_POINTER;
  if (!*bytes)
    return RED_SUCCESS; /* TODO: do we want to return an error? */

  rc = redMalloc( file->rCtx, &d, *bytes );
  if (rc) goto end;

  /* likely want to invert these and return the number of bytes read,
   * checking for interrupts/looping etc */
  if (read( file->fd, d, *bytes ) == -1) {
    rc = 0x666;
    /* TODO: something */
    goto end;
  }

  *data = d;

  end:
  if (rc)
    redFree( file->rCtx, &d, 0 );

  return rc;
}

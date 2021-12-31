/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: filewrite.c 18 2010-04-17 05:57:03Z jason $
 */

#include "red_file.h"
#include "_red_file.h"

#include "red_return.h"

int
redFileWrite(
    RedFile     file,
    const void* data,
    size_t      bytes
    )
{
  ssize_t written = -1;

  if (!file)
    return RED_ERR_NULL_CONTEXT;
  if (!data)
    return RED_ERR_NULL_POINTER;
  if (!bytes)
    return RED_SUCCESS; /* TODO do we want to return an error? */

  written = write( file->fd, data, bytes );

  if ( written != bytes ) {
    /* TODO: something */
    return 0x666;
  }

  return RED_SUCCESS;
}

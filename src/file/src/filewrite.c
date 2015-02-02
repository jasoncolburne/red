/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: filewrite.c 18 2010-04-17 05:57:03Z jason $
 */

#include "he_file.h"
#include "_he_file.h"

#include "he_return.h"

int
redFileWrite(
    RedFile     file,
    const void* data,
    size_t      bytes
    )
{
  if (!file)
    return RED_ERR_NULL_CONTEXT;
  if (!data)
    return RED_ERR_NULL_POINTER;
  if (!bytes)
    return RED_SUCCESS; /* TODO do we want to return an error? */

  if (!write( file->fd, data, bytes ) == -1) {
    /* TODO: something */
    return 0x666;
  }

  return RED_SUCCESS;
}

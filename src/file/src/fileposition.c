/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: fileposition.c 19 2010-04-22 08:27:08Z jason $
 */

#include "red_file.h"
#include "_he_file.h"

#include "red_return.h"

int
redFilePositionSet(
    RedFile file,
    red_i64 position,
    red_u32 relativeTo
    )
{
  int   whence;
  off_t offset;

  if (!file)
    return RED_ERR_NULL_CONTEXT;

  if (relativeTo == RED_FILE_POSITION_CURRENT)
    whence = SEEK_CUR;
  else if (relativeTo == RED_FILE_POSITION_START)
    whence = SEEK_SET;
  else if (relativeTo == RED_FILE_POSITION_END)
    whence = SEEK_END;
  else
    return RED_ERR_INVALID_ARGUMENT;

  offset = lseek( file->fd, (off_t)position, whence );
  if (offset < 0) {
    /* TODO: errno translation */
    return 0x666;
  }

  return RED_SUCCESS;
}


int
redFilePositionGet(
    RedFile  file,
    red_i64* position
    )
{
  off_t offset;

  if (!file)
    return RED_ERR_NULL_CONTEXT;
  if (!position)
    return RED_ERR_NULL_POINTER;

  offset = lseek( file->fd, 0, SEEK_CUR );
  if (offset == -1) {
    /* TODO: errno translation */
    return 0x666;
  }

  *position = offset;

  return RED_SUCCESS;
}


int
redFileLength(
    RedFile  file,
    red_i64* length
    )
{
  off_t old, new;

  if (!file)
    return RED_ERR_NULL_CONTEXT;
  if (!length)
    return RED_ERR_NULL_POINTER;

  /* likely a better way to do all this using the file system */

  old = lseek( file->fd, 0, SEEK_CUR );
  if (old == -1) {
    /* TODO: errno */
    return 0x666;
  }

  new = lseek( file->fd, 0, SEEK_END );
  if (new == -1) {
    /* TODO: errno */
    return 0x666;
  }

  if (lseek( file->fd, old, SEEK_SET ) == -1) {
    /* TODO: errno */
    return 0x666;
  }

  *length = new;

  return RED_SUCCESS;
}

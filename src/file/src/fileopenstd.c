/* Copyright (C) 2010-2022, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: fileopenstd.c 18 2010-04-17 05:57:03Z jason $
 */

#include "red_file.h"
#include "_red_file.h"

#include "red_return.h"

static
int
_redFileOpenStd(
    RedFile*   file,
    int        fd,
    RedContext rCtx
    )
{
  int rc = RED_SUCCESS;

  RedFile f = NULL;

  if (!file)
    return RED_ERR_NULL_POINTER;
  if (*file)
    return RED_ERR_INITIALIZED_POINTER;

  rc = _redFileAlloc( &f, rCtx );
  if (rc) goto end;

  f->leaveOpen = 1;
  f->fd        = fd;

  *file = f;
end:
  return rc;
}

int
redFileOpenStdin(
    RedFile*   file,
    RedContext rCtx
    )
{
  return _redFileOpenStd( file, STDIN_FILENO, rCtx );
}

int
redFileOpenStdout(
    RedFile*   file,
    RedContext rCtx
    )
{
  return _redFileOpenStd( file, STDOUT_FILENO, rCtx );
}


int
redFileOpenStderr(
    RedFile*   file,
    RedContext rCtx
    )
{
  return _redFileOpenStd( file, STDERR_FILENO, rCtx );
}

/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: fileopenclose.c 20 2010-04-23 01:46:23Z jason $
 */

#include "red_file.h"
#include "_red_file.h"

#include "red_memory.h"
#include "red_return.h"

int
_redFileAlloc(
    RedFile*   file,
    RedContext rCtx
    )
{
  int rc = RED_SUCCESS;

  /* assume internal fns will not pass NULL for file */
  if (!rCtx)
    return RED_ERR_NULL_CONTEXT;

  rc = redMalloc( rCtx, (void**)file, sizeof(**file) );
  if (rc) goto end;

  (*file)->rCtx = rCtx;
  
end:
  return rc;
}

int
redFileOpen(
    RedFile*    file,
    red_u32     flags,
    const char* path,
    RedContext  rCtx
    )
{
  int rc = RED_SUCCESS;

  int fd    = -1;
  int oflag = 0;

  if (!file)
    return RED_ERR_NULL_POINTER;
  if (*file)
    return RED_ERR_INITIALIZED_POINTER;
  if (!path)
    return RED_ERR_NULL_POINTER;
  /* rCtx checked by _redFileAlloc( ) */

  if (flags & RED_FILE_OPEN_RDONLY)
    oflag |= O_RDONLY;
  if (flags & RED_FILE_OPEN_WRONLY)
    oflag |= O_WRONLY;
  if (flags & RED_FILE_OPEN_RDWR)
    oflag |= O_RDWR;
  if (flags & RED_FILE_OPEN_NONBLOCK)
    oflag |= O_NONBLOCK;
  if (flags & RED_FILE_OPEN_APPEND)
    oflag |= O_APPEND;
  if (flags & RED_FILE_OPEN_CREAT)
    oflag |= O_CREAT;
  if (flags & RED_FILE_OPEN_TRUNC)
    oflag |= O_TRUNC;
  if (flags & RED_FILE_OPEN_EXCL)
    oflag |= O_EXCL;
#if !defined (__linux__)
  if (flags & RED_FILE_OPEN_SHLOCK)
    oflag |= O_SHLOCK;
  if (flags & RED_FILE_OPEN_EXLOCK)
    oflag |= O_EXLOCK;
  if (flags & RED_FILE_OPEN_NOFOLLOW)
    oflag |= O_NOFOLLOW;
  if (flags & RED_FILE_OPEN_SYMLINK)
    oflag |= O_SYMLINK;
  if (flags & RED_FILE_OPEN_EVTONLY)
    oflag |= O_EVTONLY;
#endif
  
  fd = open( path, oflag );
  if (fd < 0) {
    /* TODO: wrap error */
    return 0x666;
  }

  rc = _redFileAlloc( file, rCtx );
  if (rc) goto end;

  (*file)->fd = fd;

end:
  if (rc && fd > -1)
    close( fd );

  return rc;
}


int
redFileClose(
    RedFile* file
    )
{
  if (!file)
    return RED_ERR_NULL_POINTER;

  if (*file) {
    RedFile f = *file;

    if (!(f->leaveOpen))
      close( f->fd );

    redFree ( f->rCtx, (void**)file, 0 );
  }

  return RED_SUCCESS;
}

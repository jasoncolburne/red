/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: filereadall.c 18 2010-04-17 05:57:03Z jason $
 */

#include "he_file.h"
#include "_he_file.h"

#include "he_memory.h"

int
redFileReadAllAlloc(
    RedFile file,
    void**  data,
    size_t* bytes
    )
{
  int rc = RED_SUCCESS;

  size_t  length = 0;
  red_i64 l64    = 0;

  /* FileLength validates file */
  rc = redFileLength( file, &l64 );
  if (rc) goto end;

  /* TODO: this won't work everywhere, ie 32 bit platforms */
  length = l64;

  rc = redFilePositionSet( file, 0, RED_FILE_POSITION_START );
  if (rc) goto end;
  
  rc = redFileReadAlloc( file, data, &length );
  if (rc) goto end;

  *bytes = length;
end:
  return rc;
}

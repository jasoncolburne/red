/* Copyright (C) 2010-2022, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_file.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef ___RED_FILE_H__
#define ___RED_FILE_H__

#include "red_context.h"
#include "red_file.h"

#include <unistd.h>
#include <fcntl.h>

struct _RedFile {
  int        fd;
  red_u32    leaveOpen;

  RedContext rCtx;
};

int
_redFileAlloc(
    RedFile*   file,
    RedContext rCtx
    );

#endif /* ___RED_FILE_H__ */

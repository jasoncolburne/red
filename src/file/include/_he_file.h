/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_file.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef ___HE_FILE_H__
#define ___HE_FILE_H__

#include "he_context.h"
#include "he_file.h"

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

#endif /* ___HE_FILE_H__ */

/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_stream.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef ___HE_STREAM_H__
#define ___HE_STREAM_H__


#include "he_stream.h"

struct _RedStream {
  red_u32              flags;
  void*                data;
  
  redfuncStreamTake    rfTake;
  redfuncStreamGive    rfGive;
  redfuncStreamDestroy rfDestroy;
  
  RedStream            source;
  RedStream            sink;
  
  RedContext           rCtx;
};


#endif /* ___HE_STREAM_H__ */

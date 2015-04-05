/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_stringbuffer.h 19 2010-04-22 08:27:08Z jason $
 */

#ifndef ___HE_STRINGBUFFER_H__
#define ___HE_STRINGBUFFER_H__


#include "he_stringbuffer.h"
#include "he_spinlock.h"
#include "he_queue.h"
#include "he_resourcepool.h"

#include "he_context.h"

#ifdef __cplusplus
extern "C" {
#endif


struct _RedStringBufferPair {
  char*  string;
  size_t length;
};
typedef struct _RedStringBufferPair* RedStringBufferPair;


struct _RedStringBuffer {
  /* TODO: need a queue here to queue offset + length pairs  */
  char*   buffer;
  size_t  head;
  size_t  tail;

  size_t  size;

  int full;

  RedSpinlock lock;

  RedQueue        pairs;
  RedResourcePool pairpool;

  RedStringBufferPair given;

  RedContext rCtx;
};


#ifdef __cplusplus
}
#endif


#endif /* ___HE_STRINGBUFFER_H__ */

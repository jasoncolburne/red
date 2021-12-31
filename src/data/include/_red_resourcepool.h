/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_resourcepool.h 19 2010-04-22 08:27:08Z jason $
 */

#ifndef ___RED_RESOURCEPOOL_H__
#define ___RED_RESOURCEPOOL_H__


#include "red_resourcepool.h"

#include "red_stack.h"
#include "red_queue.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _RedResourcePool {

  RedQueue freeQueue;
  RedStack dataStack;

  red_i32 count;
  
  size_t maximum;
  
  size_t size;

  redfuncResourcePoolResourceCreate  rfResourceCreate;
  redfuncResourcePoolResourceDestroy rfResourceDestroy;

  void*  hookData;

  RedContext rCtx;
};


#ifdef __cplusplus
}
#endif


#endif /* ___RED_RESOURCEPOOL_H__ */

/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_iterator.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef ___HE_ITERATOR_H__
#define ___HE_ITERATOR_H__


#include "he_iterator.h"


#ifdef __cplusplus
extern "C" {
#endif


struct _RedIterator {
  void*      node;
  
  redfuncIteratorValue rfValue;
  redfuncIteratorNext  rfNext;
  
  RedContext rCtx;
};


int
redIteratorCreate(
    RedIterator*         iterator,
    void*                node,
    redfuncIteratorValue rfValue,
    redfuncIteratorNext  rfNext,
    RedContext           rCtx
    );


#ifdef __cplusplus
}
#endif


#endif /* ___HE_ITERATOR_H__ */

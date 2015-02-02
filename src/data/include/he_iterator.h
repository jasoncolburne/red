/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_iterator.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef __HE_ITERATOR_H__
#define __HE_ITERATOR_H__


#include "he_context.h"


#ifdef __cplusplus
extern "C" {
#endif

struct _RedIterator;
typedef struct _RedIterator* RedIterator;


typedef int (*redfuncIteratorValue)( RedIterator, void** );
typedef int  (*redfuncIteratorNext)( RedIterator, void** );

extern
int
redIteratorDestroy(
    RedIterator* iterator
    );

extern
int
redIteratorValue(
    RedIterator iterator,
    void**      value
    );

extern
int
redIteratorNext(
    RedIterator iterator,
    void**      value
    );


#ifdef __cplusplus
}
#endif


#endif /* __HE_ITERATOR_H__ */

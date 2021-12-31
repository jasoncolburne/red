/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_iterator.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef __RED_ITERATOR_H__
#define __RED_ITERATOR_H__


#include "red_context.h"


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


#endif /* __RED_ITERATOR_H__ */

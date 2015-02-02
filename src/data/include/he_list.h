/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_list.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef __HE_LIST_H__
#define __HE_LIST_H__


#include "he_collection.h"

typedef RedCollection RedList;

/* look in he_collection.h for prototypes of rfCompare and rfDestroy */
extern
int
redListCreate(
    RedList*             list,
    redfuncObjectCompare rfCompare,
    redfuncObjectDestroy rfDestroy,
    RedContext           rCtx
    );

extern
int
redListDestroy(
    RedList* list,
    int      destroyObjects
    );


extern
int
redListIterator(
    RedList      list,
    RedIterator* iterator
    );



extern
int
redListGiveHead(
    RedList list,
    void*   object
    );

extern
int
redListGiveTail(
    RedList list,
    void*   object
    );

extern
int
redListTakeHead(
    RedList list,
    void**  object
    );

extern
int
redListTakeTail(
    RedList list,
    void**  object
    );


/* TODO: add sort function */

/*
 * list, set, hashtable
 *
 *  * redListSort( list, function )
 * redListFoldL( list, function, object)
 *   ( [1, 2, 3], mult, 2 )    -> (((2 * 1) * 2) * 3)
 *   ( [1, 2, 3], mult, null ) -> (((1) * 2) * 3)
 * redListFoldR( list, function, object)
 *   ( [1, 2, 3], mult, 2 )    -> (1 * (2 * (3 * 2)))
 *   ( [1, 2, 3], mult, null ) -> (1 * (2 * (3)))
 *
 */

#endif /* __HE_LIST_H__ */

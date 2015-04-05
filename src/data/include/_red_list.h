/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_list.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef ___HE_LIST_H__
#define ___HE_LIST_H__


#include "red_context.h"

struct _RedListNode;
typedef struct _RedListNode* RedListNode;

struct _RedListNode {
  RedListNode next;
  void*       data;
};

struct _RedListData {
  RedListNode head;
  RedListNode tail;

  /* TODO: make and use this */
  /* pool allocator for nodes since they are all the same size */
  RedContext  nodeCtx;
};
typedef struct _RedListData* RedListData;


int
redListNodeCreate(
    RedListNode* node,
    void*        data,
    RedList      list
    );

int
redListNodeDestroy(
    RedListNode* node,
    RedList      list,
    int          destroyObject
    );


#endif /* ___HE_LIST_H__ */

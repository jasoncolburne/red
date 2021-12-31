/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_stack.h 19 2010-04-22 08:27:08Z jason $
 */

#ifndef __RED_STACK_H__
#define __RED_STACK_H__


#include "red_context.h"
#include "red_time.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _RedStack;
typedef struct _RedStack* RedStack;


extern
int
redStackCreateNonblocking(
    RedStack*  stack,
    size_t     initialDepthLimit,
    size_t     maximumDepthLimit,
    RedContext rCtx
    );

extern
int
redStackCreateBlocking(
    RedStack*  stack,
    size_t     initialDepthLimit,
    size_t     maximumDepthLimit,
    RedContext rCtx
    );

extern
int
redStackDestroy(
    RedStack* stack
    );

extern
int
redStackGive(
    RedStack stack,
    void*    data
    );

extern
int
redStackTake(
    RedStack stack,
    void**   data
    );

/* 0 for these timeouts block indefinitely, Give/Take always nonblocking */
extern
int
redStackGiveTimed(
    RedStack stack,
    void*    data,
    RedTime  timeout
    );

extern
int
redStackTakeTimed(
    RedStack stack,
    void**   data,
    RedTime  timeout
    );

extern
int
redStackDepth(
    RedStack stack,
    size_t*  depth
    );


#ifdef __cplusplus
}
#endif


#endif /* __RED_STACK_H__ */

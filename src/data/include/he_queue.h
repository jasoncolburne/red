/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_queue.h 19 2010-04-22 08:27:08Z jason $
 */

#ifndef __HE_QUEUE_H__
#define __HE_QUEUE_H__


#include "he_context.h"
#include "he_time.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _RedQueue;
typedef struct _RedQueue* RedQueue;


extern
int
redQueueCreateNonblocking(
    RedQueue*  queue,
    size_t     initialDepthLimit,
    size_t     maximumDepthLimit,
    RedContext rCtx
    );

extern
int
redQueueCreateBlocking(
    RedQueue*  queue,
    size_t     initialDepthLimit,
    size_t     maximumDepthLimit,
    RedContext rCtx
    );

extern
int
redQueueDestroy(
    RedQueue* queue
    );

extern
int
redQueueGive(
    RedQueue queue,
    void*    data
    );

extern
int
redQueueTake(
    RedQueue queue,
    void**   data
    );

/* 0 for these timeouts block indefinitely, StackGive/Take return immediately */
extern
int
redQueueGiveTimed(
    RedQueue queue,
    void*    data,
    RedTime  timeout
    );

extern
int
redQueueTakeTimed(
    RedQueue queue,
    void**   data,
    RedTime  timeout
    );

extern
int
redQueueLength(
    RedQueue queue,
    size_t*  length
    );


#ifdef __cplusplus
}
#endif


#endif /* __HE_QUEUE_H__ */

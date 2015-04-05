/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_semaphore.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef __HE_SEMAPHORE_H__
#define __HE_SEMAPHORE_H__


#include "red_context.h"
#include "red_time.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _RedSemaphore;
typedef struct _RedSemaphore* RedSemaphore;


extern
int
redSemaphoreCreate(
    RedSemaphore* semaphore,
    red_u32       value,
    RedContext    rCtx
    );

extern
int
redSemaphoreDestroy(
    RedSemaphore* semaphore
    );

extern
int
redSemaphoreGive(
    RedSemaphore semaphore
    );

extern
int
redSemaphoreTake(
    RedSemaphore semaphore
    );

extern
int
redSemaphoreTakeTimed(
    RedSemaphore semaphore,
    RedTime      timeout
    );

extern
int
redSemaphoreValue(
    RedSemaphore semaphore,
    red_i32*     value
    );



#ifdef __cplusplus
}
#endif


#endif /* __HE_SEMAPHORE_H__ */

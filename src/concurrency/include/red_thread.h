/* Copyright (C) 2010-2022, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_thread.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef __RED_THREAD_H__
#define __RED_THREAD_H__

#include "red_context.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _RedThreadAttributes;
typedef struct _RedThreadAttributes* RedThreadAttributes;

struct _RedThread;
typedef struct _RedThread* RedThread;


typedef int (*redfuncThread)(RedThread, void*, RedContext);


#define RED_ATTRIBUTE_THREAD_DETACHED  0x00000001
#define RED_ATTRIBUTE_THREAD_JOINABLE  0x00000002
#define RED_ATTRIBUTE_THREAD_STACKSIZE 0x00000004

extern
int
redThreadAttributesCreate(
    RedThreadAttributes* attr,
    RedContext           rCtx
    );

extern
int
redThreadAttributesDestroy(
    RedThreadAttributes* attr
    );

extern
int
redThreadAttributesAssign(
    RedThreadAttributes attr,
    red_u32             attrId,
    void*               value,
    size_t              valueSz
    );

extern
int
redThreadAttributesQuery(
    RedThreadAttributes attr,
    red_u32             attrId,
    void*               value,
    size_t*             valueSz
    );



extern
int
redThreadCreate(
    RedThread*          thread,
    RedThreadAttributes attr,
    redfuncThread       function,
    void*               data,
    RedContext          rCtx
    );

extern
int
redThreadDestroy(
    RedThread* thread
    );

extern
int
redThreadJoin(
    RedThread thread
    );


#ifdef __cplusplus
}
#endif


#endif /* __RED_THREAD_H__ */

/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_context.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef __HE_CONTEXT_H__
#define __HE_CONTEXT_H__


#include "he_type.h"
#include "he_return.h"

#ifdef __cplusplus
extern "C" {
#endif


struct _RedContext;
typedef struct _RedContext* RedContext;


typedef int (*redfuncMalloc)(void**, size_t);
typedef int   (*redfuncFree)(void**, size_t);
typedef int (*redfuncMemcpy)(void*, const void*, size_t);
typedef int (*redfuncMemset)(void*, unsigned char, size_t);
typedef int (*redfuncMemcmp)(const void*, const void*, size_t, int*);

typedef struct _RedMemoryFunctions {
  redfuncMalloc rfMalloc;
  redfuncFree   rfFree;
  redfuncMemcpy rfMemcpy;
  redfuncMemset rfMemset;
  redfuncMemcmp rfMemcmp;
} RedMemoryFunctions;


/**
 *
 */
extern
int
redContextCreateDefault(
    RedContext* ctx
    );
  
/**
 *
 */
extern
int
redContextCreate(
    RedContext*         ctx,
    RedMemoryFunctions* funcs
    );                       

/**
 *
 */
extern
int
redContextDestroy(
    RedContext* ctx
    );



#ifdef __cplusplus
}
#endif


#endif /* __HE_CONTEXT_H__ */

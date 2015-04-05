/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_memory.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef __HE_MEMORY_H__
#define __HE_MEMORY_H__

#include "he_context.h"
#include "he_return.h"
#include "he_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  RED_FREE_POLICY_DEFAULT          = 0x0000,
  RED_FREE_POLICY_ASSUME_OWNERSHIP = 0x0001,
  RED_FREE_POLICY_YIELD_OWNERSHIP  = 0x0002
} RedFreePolicy;

/**
 *
 */
extern
int
redMalloc(
    RedContext rCtx,
    void**     ptr,
    size_t     sz
    );
	
/**
 * Frees, zeroes memory for sz > 0 and sets ptr to NULL
 */
extern
int
redFree(
    RedContext rCtx,
    void**     ptr,
    size_t     sz
    );

/**
 *
 */
extern
int
redMemcpy(
    RedContext  rCtx,
    void*       dst,
    const void* src,
    size_t      sz
    );
	
/**
 *
 */
extern
int
redMemset(
    RedContext    rCtx,
    void*         dst,
    unsigned char val,
    size_t        sz
    );

/**
 *
 */
extern
int
redMemcmp(
    RedContext  rCtx,
    const void* s1,
    const void* s2,
    size_t      sz,
    int*        result
    );


#ifdef __cplusplus
}
#endif


#endif /* __HE_MEMORY_H__ */

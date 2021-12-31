/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: _he_memorydefault.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef ___RED_MEMORYDEFAULT_H__
#define ___RED_MEMORYDEFAULT_H__


int
rfDefaultMalloc(
    void** ptr,
    size_t sz
    );

int
rfDefaultFree(
    void** ptr,
    size_t sz
    );

int
rfDefaultMemcpy(
    void*       dst,
    const void* src,
    size_t      sz
    );

int
rfDefaultMemset(
    void*         dst,
    unsigned char val,
    size_t        sz
    );

int
rfDefaultMemcmp(
    const void* s1,
    const void* s2,
    size_t      sz,
    int*        result
    );


#endif /* ___RED_MEMORYDEFAULT_H__ */

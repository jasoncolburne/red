/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: memorydefault.c 18 2010-04-17 05:57:03Z jason $
 */

#include "red_context.h"
#include "red_memory.h"
#include "_red_memorydefault.h"

#include "string.h"

int
rfDefaultMalloc(
    void** ptr,
    size_t sz
    )
{
	if (!ptr) return RED_ERR_NULL_POINTER;
	if (!sz)  return RED_SUCCESS;
	
	*ptr = malloc( sz );
	if (!(*ptr)) return RED_FAIL_ALLOC;
	
	/* leave this? */
	memset( *ptr, 0, sz );
	
	return RED_SUCCESS;
}

int
rfDefaultFree(
    void** ptr,
    size_t sz
    )
{
	if (!ptr) return RED_ERR_NULL_POINTER;
	if (sz) memset( *ptr, 0, sz );
	free( *ptr );
	*ptr = NULL;
	
	return RED_SUCCESS;
}

int
rfDefaultMemcpy(
    void*       dst,
    const void* src,
    size_t      sz
    )
{
	if (!dst || !src) return RED_ERR_NULL_POINTER;
	memcpy( dst, src, sz );
	
	return RED_SUCCESS;
}

int
rfDefaultMemset(
    void*         dst,
    unsigned char val,
    size_t        sz
    )
{
	if (!dst) return RED_ERR_NULL_POINTER;
	memset( dst, val, sz );
	
	return RED_SUCCESS;
}

int
rfDefaultMemcmp(
    const void* s1,
    const void* s2,
    size_t      sz,
    int*        result
    )
{
  if (!s1 || !s2 || !result) return RED_ERR_NULL_POINTER;
  
  *result = memcmp( s1, s2, sz );
  
  return RED_SUCCESS;
}

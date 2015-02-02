/* Copyright (C) 2010, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_type.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef __HE_TYPE_H__
#define __HE_TYPE_H__


#include <stdlib.h>	/* size_t, NULL, etc */
#include <sys/types.h>

/* for os x */
typedef u_int64_t red_u64;
typedef u_int32_t red_u32;
typedef u_int16_t red_u16;
typedef u_int8_t  red_u8;

typedef int64_t   red_i64;
typedef int32_t   red_i32;
typedef int16_t   red_i16;
typedef int8_t    red_i8;

/*
typedef unsigned long long red_u64;
typedef unsigned long      red_u32;
typedef unsigned short     red_u16;
typedef unsigned char      red_u8;

typedef signed long long   red_i64;
typedef signed long        red_i32;
typedef signed short       red_i16;
typedef signed char        red_i8;
*/

#endif /* __HE_TYPE_H__ */

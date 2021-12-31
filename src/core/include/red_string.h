/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_string.h 19 2010-04-22 08:27:08Z jason $
 */

#ifndef __RED_STRING_H__
#define __RED_STRING_H__

#include <stdio.h>
#include <string.h>

#define RED_TO_STRING( s ) _RED_TO_STRING( s )
#define _RED_TO_STRING( s ) #s

/* for most platforms */
#define redVsnPrintf vsnprintf
#define redSnPrintf  snprintf
#define redStrncpy   strncpy
#define redStrstr    strstr
#define redStrchr    strchr
#define redStrlen    strlen

#endif /* __RED_STRING_H__ */

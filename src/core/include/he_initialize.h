/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_initialize.h 19 2010-04-22 08:27:08Z jason $
 */

#ifndef __HE_INITIALIZE_H__
#define __HE_INITIALIZE_H__


#include "he_context.h"

#ifdef __cplusplus
extern "C" {
#endif


extern
int
redInitialize(
    RedContext rCtx
    );

extern
int
redDeinitialize(
    void
    );


#ifdef __cplusplus
}
#endif


#endif /* __HE_INITIALIZE_H__ */

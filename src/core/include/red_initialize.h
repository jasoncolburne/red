/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_initialize.h 19 2010-04-22 08:27:08Z jason $
 */

#ifndef __RED_INITIALIZE_H__
#define __RED_INITIALIZE_H__


#include "red_context.h"

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


#endif /* __RED_INITIALIZE_H__ */

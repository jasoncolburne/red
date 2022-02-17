/* Copyright (C) 2010-2022, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_condition.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef __RED_CONDITION_H__
#define __RED_CONDITION_H__


#include "red_context.h"
#include "red_time.h"

#ifdef __cplusplus
extern "C" {
#endif


struct _RedCondition;
typedef struct _RedCondition* RedCondition;

extern
int
redConditionCreate(
    RedCondition* cond,
    RedContext    rCtx
    );

extern
int
redConditionDestroy(
    RedCondition* cond
    );


/* unblocks one (and maybe more) threads blocked on cond */
extern
int
redConditionGive(
    RedCondition cond
    );

/* unblocks all threads blocked on cond */
extern
int
redConditionGiveAll(
    RedCondition cond
    );

extern
int
redConditionTake(
    RedCondition cond
    );

extern
int
redConditionTakeTimed(
    RedCondition cond,
    RedTime      timeout
    );
/* signal, broadcast, wait etc */


#ifdef __cplusplus
}
#endif


#endif /* __redConditionVariable_h__ */

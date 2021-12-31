/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: testcondition.c 21 2010-04-28 17:38:52Z jason $
 */

#include "main.h"

#include "red_memory.h"
#include "red_time.h"
#include "_red_thread.h"

#define TEST_THREAD_COUNT 5
#define TEST_STACK_SIZE   0x4000

RED_TEST_UNIT_IDENTIFY( "condition.c" );

struct _Payload {
  RedCondition cond;
  RedLock      lock;
  int          n;
  RedStream    conErr;
};
typedef struct _Payload* Payload;

static
int
threadFunc(
    RedThread  thread,
    void*      data,
    RedContext rCtx
    )
{
  int rc = RED_SUCCESS;

  Payload   p      = (Payload)data;
  RedStream conErr = p->conErr;

  /* need to test timeout too */
  RED_TEST_CALL_SILENTLY( redConditionTake( p->cond ), RED_SUCCESS,
                          "redConditionTake( )" );

  /* mutual exclusion for increment, could use an atomic var once implemented */
  RED_TEST_CALL_SILENTLY( redLockTake( p->lock ), RED_SUCCESS,
                          "redLockTake( )" );

  p->n = p->n + 1;

  RED_TEST_CALL_SILENTLY( redLockGive( p->lock ), RED_SUCCESS,
                          "redLockGive( )" );

end:
  return rc;
}



RED_TEST_UNIT_IMPLEMENTATION_BEGIN( testCondition );
  int i;

  size_t s = TEST_STACK_SIZE;

  RedThread*          threads    = NULL;
  RedThreadAttributes attributes = NULL;

  struct _Payload payload = { NULL, NULL, 0, conErr };

  RED_TEST_CALL_SILENTLY( redMalloc( rCtx, (void**)&threads,
                            sizeof(RedThread) * TEST_THREAD_COUNT),
                          RED_SUCCESS, "redMalloc( )" );

  RED_TEST_CALL( redThreadAttributesCreate( &attributes, rCtx ),
                 RED_SUCCESS, "redThreadAttributesCreate()" );

  RED_TEST_CALL( redThreadAttributesAssign( attributes,
                   RED_ATTRIBUTE_THREAD_JOINABLE, NULL, 0 ),
                 RED_SUCCESS, "redThreadAttributesAssign( ..., "
                 "RED_ATTRIBUTE_THREAD_JOINABLE, ... )" );

  RED_TEST_CALL_SILENTLY( redThreadAttributesAssign( attributes,
                            RED_ATTRIBUTE_THREAD_STACKSIZE, &s, sizeof(s) ),
                          RED_SUCCESS, "redThreadAttributesAssign( ..., "
                          "RED_ATTRIBUTE_THREAD_STACKSIZE, N, ... )" );

  RED_TEST_CALL( redConditionCreate( &(payload.cond), rCtx ),
                 RED_SUCCESS, "redConditionCreate( )" );

  RED_TEST_CALL_SILENTLY( redLockCreate( &(payload.lock), rCtx ),
                          RED_SUCCESS, "redLockCreate( )" );

  redStreamPrint( conErr, 0, "---spawning %d listeners---\n",
                  TEST_THREAD_COUNT );

  for (i = 0; i < TEST_THREAD_COUNT; i++) {
    RED_TEST_CALL_SILENTLY( redThreadCreate( &(threads[i]), attributes,
                              threadFunc, &payload, rCtx ),
                            RED_SUCCESS, "redThreadCreate( )" );
  }

  redSleep( RED_TIME_FROM_MSEC(250) );
  RED_ASSERT( (payload.n == 0), "listeners blocked" );

  RED_TEST_CALL( redConditionGive( payload.cond ), RED_SUCCESS,
                 "redConditionGive( )" );

  redSleep( RED_TIME_FROM_MSEC(250) );
  RED_ASSERT( (payload.n != 0), "at least 1 listener woke" );

  RED_TEST_CALL( redConditionGiveAll( payload.cond ), RED_SUCCESS,
                 "redConditionGiveAll( )" );

  redSleep( RED_TIME_FROM_MSEC(250) );
  RED_ASSERT( (payload.n == TEST_THREAD_COUNT), "all listeners awoke" );

  for (i = 0; i < TEST_THREAD_COUNT; i++) {
    RED_TEST_CALL_SILENTLY( redThreadJoin( threads[i] ), RED_SUCCESS,
                            "redThreadJoin( )" );
    RED_ASSERT_SILENTLY( (threads[i]->rc == RED_SUCCESS), "threads[i]->rc" );

    RED_TEST_CALL_SILENTLY( redThreadDestroy( &(threads[i]) ), RED_SUCCESS,
                            "redThreadDestroy( )" );
  }

  RED_TEST_CALL( redConditionDestroy( &(payload.cond) ), RED_SUCCESS,
                 "redConditionDestroy( )" );

RED_TEST_UNIT_IMPLEMENTATION_CLEANUP();

  redFree( rCtx, (void**)&threads, 0 );
  redConditionDestroy( &(payload.cond) );
  redLockDestroy( &(payload.lock) );
  redThreadAttributesDestroy( &attributes );

RED_TEST_UNIT_IMPLEMENTATION_END();

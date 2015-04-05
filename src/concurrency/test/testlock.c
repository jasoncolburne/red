/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: testlock.c 21 2010-04-28 17:38:52Z jason $
 */

#include "main.h"

#include "red_memory.h"
#include "_he_thread.h"

#define TEST_THREAD_COUNT 8
#define TEST_THREAD_LOOPS 0x2000
#define TEST_STACK_SIZE   0x4000

RED_TEST_UNIT_IDENTIFY( "lock.c" );

struct _Payload {
  RedLock   lock;
  int       n;
  RedStream conErr;
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
  int i;

  Payload   p      = (Payload)data;
  RedStream conErr = p->conErr;

  for (i = 0; i < TEST_THREAD_LOOPS; i++) {
    RED_TEST_CALL_SILENTLY( redLockTake( p->lock ), RED_SUCCESS,
                            "redLockTake( )" );
    
    p->n = p->n + 1;

    RED_TEST_CALL_SILENTLY( redLockGive( p->lock ), RED_SUCCESS,
                            "redLockGive( )" );
  }

end:
  return rc;
}



RED_TEST_UNIT_IMPLEMENTATION_BEGIN( testLock );

  int i;

  size_t s = TEST_STACK_SIZE;

  struct _Payload payload = { NULL, 0, conErr };

  RedThread*          threads    = NULL;
  RedThreadAttributes attributes = NULL;

  RED_TEST_CALL_SILENTLY( redMalloc( rCtx, (void**)&threads,
                            sizeof(RedThread) * TEST_THREAD_COUNT ),
                          RED_SUCCESS, "redMalloc( )" );

  RED_TEST_CALL_SILENTLY( redThreadAttributesCreate( &attributes, rCtx ),
                          RED_SUCCESS, "redThreadAttributesCreate()" );

  RED_TEST_CALL_SILENTLY( redThreadAttributesAssign( attributes,
                            RED_ATTRIBUTE_THREAD_JOINABLE, NULL, 0 ),
                          RED_SUCCESS, "redThreadAttributesAssign( ..., "
                          "RED_ATTRIBUTE_THREAD_JOINABLE, ... )" );

  RED_TEST_CALL_SILENTLY( redThreadAttributesAssign( attributes,
                            RED_ATTRIBUTE_THREAD_STACKSIZE, &s, sizeof(s) ),
                          RED_SUCCESS, "redThreadAttributesAssign( ..., "
                          "RED_ATTRIBUTE_THREAD_STACKSIZE, N, ... )" );

  RED_TEST_CALL( redLockCreate( &(payload.lock), rCtx ), RED_SUCCESS,
                 "redLockCreate( )" );

  redStreamPrint( conErr, 0, "---spawning %d threads---\n", TEST_THREAD_COUNT );

  for (i = 0; i < TEST_THREAD_COUNT; i++) {
    RED_TEST_CALL_SILENTLY( redThreadCreate( &(threads[i]), attributes,
                              threadFunc, &payload, rCtx ),
                            RED_SUCCESS, "redThreadCreate( )" );
  }

  for (i = 0; i < TEST_THREAD_COUNT; i++) {
    RED_TEST_CALL_SILENTLY( redThreadJoin( threads[i] ), RED_SUCCESS,
                            "redThreadJoin( )" );
    RED_ASSERT_SILENTLY( (threads[i]->rc == RED_SUCCESS), "threads[i]->rc");

    RED_TEST_CALL_SILENTLY( redThreadDestroy( &(threads[i]) ), RED_SUCCESS,
                            "redThreadDestroy( )" );
  }
  RED_ASSERT( (payload.n == TEST_THREAD_COUNT * TEST_THREAD_LOOPS),
              "counter seems protected by lock" );

  RED_TEST_CALL( redLockDestroy( &(payload.lock) ), RED_SUCCESS,
                 "redLockDestroy( )" );

RED_TEST_UNIT_IMPLEMENTATION_CLEANUP();

  redFree( rCtx, (void**)&threads, 0 );
  redLockDestroy( &(payload.lock) );
  redThreadAttributesDestroy( &attributes );

RED_TEST_UNIT_IMPLEMENTATION_END();

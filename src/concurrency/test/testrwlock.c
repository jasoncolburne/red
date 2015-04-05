/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: testrwlock.c 21 2010-04-28 17:38:52Z jason $
 */

#include "main.h"

#include "red_memory.h"
#include "_he_thread.h"
#include "red_time.h"

#define TEST_THREAD_COUNT_READER 16
#define TEST_THREAD_COUNT_WRITER 3
#define TEST_THREAD_LOOPS        0x100
#define TEST_STACK_SIZE          0x4000

RED_TEST_UNIT_IDENTIFY( "rwlock.c" );

struct _Payload {
  RedRWLock lock;
  int       n;
  RedStream conErr;
};
typedef struct _Payload* Payload;


static
int
readerFunc(
    RedThread  thread,
    void*      data,
    RedContext rCtx
    )
{
  int rc = RED_SUCCESS;
  int i;

  Payload   p      = (Payload)data;
  RedStream conErr = p->conErr;

  for (i = 0; i < TEST_THREAD_LOOPS * 10; i++) {
    RED_TEST_CALL_SILENTLY( redRWLockTakeRead( p->lock ), RED_SUCCESS,
                            "redRWLockTakeRead( )" );

    redSleep( RED_TIME_FROM_USEC(50) );

    RED_TEST_CALL_SILENTLY( redRWLockGive( p->lock ), RED_SUCCESS,
                            "redRWLockGive( )" );
  }

  RED_TEST_CALL_SILENTLY( redRWLockTakeRead( p->lock ), RED_SUCCESS,
                          "redRWLockTakeRead( )" );

  RED_ASSERT_SILENTLY( (p->n == TEST_THREAD_LOOPS * TEST_THREAD_COUNT_WRITER),
                       "writer starvation" );

  RED_TEST_CALL_SILENTLY( redRWLockGive( p->lock ), RED_SUCCESS,
                          "redRWLockGive( )" );

end:
  return rc;
}


static
int
writerFunc(
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
    RED_TEST_CALL_SILENTLY( redRWLockTakeWrite( p->lock ), RED_SUCCESS,
                            "redRWLockTakeWrite( )" );
    
    p->n = p->n + 1;

    RED_TEST_CALL_SILENTLY( redRWLockGive( p->lock ), RED_SUCCESS,
                            "redRWLockGive( )" );
  }

end:
  return rc;
}



RED_TEST_UNIT_IMPLEMENTATION_BEGIN( testRWLock );

  int i;

  size_t s = TEST_STACK_SIZE;

  RedThread* readers = NULL;
  RedThread* writers = NULL;

  RedThreadAttributes attributes = NULL;

  struct _Payload payload = { NULL, 0 };

  RED_TEST_CALL_SILENTLY( redMalloc( rCtx, (void**)&readers,
                            sizeof(RedThread) * TEST_THREAD_COUNT_READER),
                          RED_SUCCESS, "redMalloc( )" );

  RED_TEST_CALL_SILENTLY( redMalloc( rCtx, (void**)&writers,
                            sizeof(RedThread) * TEST_THREAD_COUNT_WRITER),
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

  RED_TEST_CALL_SILENTLY( redRWLockCreate( &(payload.lock), rCtx ),
                          RED_SUCCESS, "redRWLockCreate( )" );

  redStreamPrint( conErr, 0, "---spawning %d writers---\n",
                  TEST_THREAD_COUNT_WRITER );
  redStreamPrint( conErr, 0, "---spawning %d readers---\n",
                  TEST_THREAD_COUNT_READER );

  for (i = 0; i < TEST_THREAD_COUNT_READER; i++) {
    RED_TEST_CALL_SILENTLY( redThreadCreate( &(readers[i]), attributes,
                              readerFunc, &payload, rCtx ), RED_SUCCESS,
                            "redThreadCreate( )" );
  }

  redSleep( RED_TIME_FROM_USEC(500) );

  for (i = 0; i < TEST_THREAD_COUNT_WRITER; i++) {
    RED_TEST_CALL_SILENTLY( redThreadCreate( &(writers[i]), attributes,
                              writerFunc, &payload, rCtx ),
                              RED_SUCCESS, "redThreadCreate( )" );
  }

  for (i = 0; i < TEST_THREAD_COUNT_READER; i++) {
    RED_TEST_CALL_SILENTLY( redThreadJoin( readers[i] ), RED_SUCCESS,
                            "redThreadJoin( )" );
    RED_ASSERT_SILENTLY( (readers[i]->rc == RED_SUCCESS), "readers[i]->rc" );

    RED_TEST_CALL_SILENTLY( redThreadDestroy( &(readers[i]) ), RED_SUCCESS,
                            "redThreadDestroy( )" );
  }
  for (i = 0; i < TEST_THREAD_COUNT_WRITER; i++) {
    RED_TEST_CALL_SILENTLY( redThreadJoin( writers[i] ), RED_SUCCESS,
                            "redThreadJoin( )" );
    RED_ASSERT_SILENTLY( (writers[i]->rc == RED_SUCCESS), "writers[i]->rc" );

    RED_TEST_CALL_SILENTLY( redThreadDestroy( &(writers[i]) ), RED_SUCCESS,
                            "redThreadDestroy( )" );
  }
  redStreamPrint( conErr, 0, "writers do not appear starved\n");

  RED_TEST_CALL( redRWLockDestroy( &(payload.lock) ), RED_SUCCESS,
                 "redRWLockDestroy( )" );

RED_TEST_UNIT_IMPLEMENTATION_CLEANUP();

  redFree( rCtx, (void**)&readers, 0 );
  redFree( rCtx, (void**)&writers, 0 );
  redRWLockDestroy( &(payload.lock) );
  redThreadAttributesDestroy( &attributes );

RED_TEST_UNIT_IMPLEMENTATION_END();

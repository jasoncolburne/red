/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: testatomic.c 18 2010-04-17 05:57:03Z jason $
 */

#include "main.h"

#include "red_atomic.h"
#include "red_time.h"
#include "_red_thread.h"

RED_TEST_UNIT_IDENTIFY( "atomic.c" );

#define TEST_THREAD_COUNT 8
#define TEST_THREAD_LOOPS 0x4000

struct _Payload {
  int operation;

  red_i32 add32;
  red_i32 inc32;
  red_i32 dec32;
  red_i32 swap32;

  red_i64 add64;
  red_i64 inc64;
  red_i64 dec64;
  red_i64 swap64;

  RedLock lock;

  RedStream conErr;
};
typedef struct _Payload* Payload;

enum {
  TEST_OPERATION_ADD32 = 0,
  TEST_OPERATION_INC32,
  TEST_OPERATION_DEC32,
  TEST_OPERATION_ADD64,
  TEST_OPERATION_INC64,
  TEST_OPERATION_DEC64,
  TEST_OPERATION_TERMINATOR
};

const char operationDescriptions[][6] = {
  "Add32",
  "Inc32",
  "Dec32",
  "Add64",
  "Inc64",
  "Dec64"
};

static
int
threadAtomicFunc(
    RedThread  thread,
    void*      data,
    RedContext rCtx
    )
{
  int rc = RED_SUCCESS;

  int i;

  Payload   p      = (Payload)data;
  RedStream conErr = p->conErr;

  switch (p->operation) {
    case TEST_OPERATION_ADD32:
      for (i = 0; i < TEST_THREAD_LOOPS; i++)
        redAtomicAdd32( 5, &(p->add32), NULL );
      break;
    case TEST_OPERATION_INC32:
      for (i = 0; i < TEST_THREAD_LOOPS; i++)
        redAtomicIncrement32( &(p->inc32), NULL );
      break;
    case TEST_OPERATION_DEC32:
      for (i = 0; i < TEST_THREAD_LOOPS; i++)
        redAtomicDecrement32( &(p->dec32), NULL );
      break;
    case TEST_OPERATION_ADD64:
      for (i = 0; i < TEST_THREAD_LOOPS; i++)
        redAtomicAdd64( 5, &(p->add64), NULL );
      break;
    case TEST_OPERATION_INC64:
      for (i = 0; i < TEST_THREAD_LOOPS; i++)
        redAtomicIncrement64( &(p->inc64), NULL );
      break;
    case TEST_OPERATION_DEC64:
      for (i = 0; i < TEST_THREAD_LOOPS; i++)
        redAtomicDecrement64( &(p->dec64), NULL );
      break;
    default:
      RED_ASSERT( 0, "valid operation" );
  }

end:
  return rc;
}

static
int
threadLockFunc(
    RedThread  thread,
    void*      data,
    RedContext rCtx
    )
{
  int rc = RED_SUCCESS;

  int i;

  Payload   p      = (Payload)data;
  RedStream conErr = p->conErr;

  switch (p->operation) {
    case TEST_OPERATION_ADD32:
      for (i = 0; i < TEST_THREAD_LOOPS; i++) {
        redLockTake( p->lock );
        p->add32 += 5;
        redLockGive( p->lock );
      }
      break;
    case TEST_OPERATION_INC32:
      for (i = 0; i < TEST_THREAD_LOOPS; i++) {
        redLockTake( p->lock );
        p->inc32++;
        redLockGive( p->lock );
      }
      break;
    case TEST_OPERATION_DEC32:
      for (i = 0; i < TEST_THREAD_LOOPS; i++) {
        redLockTake( p->lock );
        p->dec32--;
        redLockGive( p->lock );
      }
      break;
    case TEST_OPERATION_ADD64:
      for (i = 0; i < TEST_THREAD_LOOPS; i++) {
        redLockTake( p->lock );
        p->add64 += 5;
        redLockGive( p->lock );
      }
      break;
    case TEST_OPERATION_INC64:
      for (i = 0; i < TEST_THREAD_LOOPS; i++) {
        redLockTake( p->lock );
        p->inc64++;
        redLockGive( p->lock );
      }
      break;
    case TEST_OPERATION_DEC64:
      for (i = 0; i < TEST_THREAD_LOOPS; i++) {
        redLockTake( p->lock );
        p->dec64--;
        redLockGive( p->lock );
      }
      break;
    default:
      RED_ASSERT( 0, "valid operation" );
  }

end:
  return rc;
}



RED_TEST_UNIT_IMPLEMENTATION_BEGIN( testAtomic );

  int i;
  int j;

  void*   swapPtr = &i;
  red_i32 swap32  = 16;
  red_i64 swap64  = 32;

  RedThread*      threads = NULL;

  struct _Payload payload = { 0 };

  payload.add32  = 0;
  payload.inc32  = 0;
  payload.dec32  = 0;
  payload.swap32 = 0;

  payload.add64  = 0;
  payload.inc64  = 0;
  payload.dec64  = 0;
  payload.swap64 = 0;

  payload.conErr = conErr;

  RED_TEST_CALL_SILENTLY( redMalloc( rCtx, (void**)&threads,
                            sizeof(RedThread) * TEST_THREAD_COUNT ),
                          RED_SUCCESS, "redMalloc( )" );

  RED_TEST_CALL_SILENTLY( redLockCreate( &(payload.lock), rCtx ), RED_SUCCESS,
                          "redLockCreate( )" );

  RED_TEST_CALL( redAtomicCompareAndSwapPointer( &i, NULL, &swapPtr ),
                 RED_SUCCESS, "redAtomicCompareAndSwapPointer( a, b, &a )" );
  RED_ASSERT( (swapPtr == NULL), "  swap occurred" );

  RED_TEST_CALL( redAtomicCompareAndSwapPointer( &i, NULL, &swapPtr ),
                 RED_FAIL_ATOMIC_COMPARISON,
                 "redAtomicCompareAndSwapPointer( b, a, &a )" );
  RED_ASSERT( (swapPtr == NULL), "  swap did not occur" );

  RED_TEST_CALL( redAtomicCompareAndSwap32( swap32, 17, &swap32 ),
                 RED_SUCCESS, "redAtomicCompareAndSwap32( a, b, &a )" );
  RED_ASSERT( (swap32 == 17), "  swap occurred" );

  RED_TEST_CALL( redAtomicCompareAndSwap32( 16, 17, &swap32 ),
                 RED_FAIL_ATOMIC_COMPARISON,
                 "redAtomicCompareAndSwap32( b, a, &a )" );
  RED_ASSERT( (swap32 == 17), "  swap did not occur" );

  RED_TEST_CALL( redAtomicCompareAndSwap64( swap64, 17, &swap64 ),
                 RED_SUCCESS, "redAtomicCompareAndSwap64( a, b, &a )" );
  RED_ASSERT( (swap64 == 17), "  swap occurred" );

  RED_TEST_CALL( redAtomicCompareAndSwap64( 32, 17, &swap64 ),
                 RED_FAIL_ATOMIC_COMPARISON,
                 "redAtomicCompareAndSwap64( b, a, &a )" );
  RED_ASSERT( (swap64 == 17), "  swap did not occur" );

  redStreamPrint( conErr, 2, "\n" );

  for (j = 0; j < TEST_OPERATION_TERMINATOR; j++) {
    payload.operation = j;

    RedTime now, then;

    redTimeNow( &then );

    for (i = 0; i < TEST_THREAD_COUNT; i++) {
      RED_TEST_CALL_SILENTLY( redThreadCreate( &(threads[i]), NULL,
                                threadAtomicFunc, &payload, rCtx ),
                              RED_SUCCESS, "redThreadCreate( )" );
    }

    for (i = 0; i < TEST_THREAD_COUNT; i++) {
      RED_TEST_CALL_SILENTLY( redThreadJoin( threads[i] ), RED_SUCCESS,
                              "redThreadJoin( )" );
      RED_ASSERT_SILENTLY( (threads[i]->rc == RED_SUCCESS), "threads[i]->rc");

      RED_TEST_CALL_SILENTLY( redThreadDestroy( &(threads[i]) ), RED_SUCCESS,
                              "redThreadDestroy( )" );
    }

    redTimeNow( &now );

    redStreamPrint( conErr, 0, "Atomic %s operations [%d loops x %d threads]:"
                    " %lld usec\n", operationDescriptions[j],
                    TEST_THREAD_LOOPS, TEST_THREAD_COUNT,
                    RED_TIME_AS_USEC( now - then ) );

    redTimeNow( &then );

    for (i = 0; i < TEST_THREAD_COUNT; i++) {
      RED_TEST_CALL_SILENTLY( redThreadCreate( &(threads[i]), NULL,
                                threadLockFunc, &payload, rCtx ),
                              RED_SUCCESS, "redThreadCreate( )" );
    }

    for (i = 0; i < TEST_THREAD_COUNT; i++) {
      RED_TEST_CALL_SILENTLY( redThreadJoin( threads[i] ), RED_SUCCESS,
                              "redThreadJoin( )" );
      RED_ASSERT_SILENTLY( (threads[i]->rc == RED_SUCCESS), "threads[i]->rc");

      RED_TEST_CALL_SILENTLY( redThreadDestroy( &(threads[i]) ), RED_SUCCESS,
                              "redThreadDestroy( )" );
    }

    redTimeNow( &now );

    redStreamPrint( conErr, 0, "Locked %s operations [%d loops x %d threads]:"
                    " %lld usec\n", operationDescriptions[j],
                    TEST_THREAD_LOOPS, TEST_THREAD_COUNT,
                    RED_TIME_AS_USEC( now - then ) );
  }

  redStreamPrint( conErr, 2, "\n" );

  RED_ASSERT( (payload.add32 == TEST_THREAD_COUNT * TEST_THREAD_LOOPS * 10),
              "add32 counted correctly" );
  RED_ASSERT( (payload.inc32 == TEST_THREAD_COUNT * TEST_THREAD_LOOPS * 2),
              "inc32 counted correctly" );
  RED_ASSERT( (payload.dec32 == TEST_THREAD_COUNT * TEST_THREAD_LOOPS * -2),
              "dec32 counted correctly" );
  RED_ASSERT( (payload.add64 == TEST_THREAD_COUNT * TEST_THREAD_LOOPS * 10),
              "add64 counted correctly" );
  RED_ASSERT( (payload.inc64 == TEST_THREAD_COUNT * TEST_THREAD_LOOPS * 2),
              "inc64 counted correctly" );
  RED_ASSERT( (payload.dec64 == TEST_THREAD_COUNT * TEST_THREAD_LOOPS * -2),
              "dec64 counted correctly" );

RED_TEST_UNIT_IMPLEMENTATION_CLEANUP();

  redFree( rCtx, (void**)&threads, 0 );
  redLockDestroy( &(payload.lock) );

RED_TEST_UNIT_IMPLEMENTATION_END();

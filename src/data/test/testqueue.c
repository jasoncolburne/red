/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: testqueue.c 21 2010-04-28 17:38:52Z jason $
 */

#include "main.h"

#include "red_queue.h"
#include "red_thread.h"
#include "_he_thread.h"
#include "red_time.h"
#include "red_spinlock.h"


RED_TEST_UNIT_IDENTIFY( "testqueue.c" );

#define TEST_THREAD_COUNT_MAXIMUM 5
#define TEST_THREAD_LOOPS_START   0x00000800
#define TEST_MEMORY_CAP           0x800000



struct _Payload {
  RedQueue    queue;

  RedSpinlock lock;
  red_u32     mask;
  size_t      loops;
};
typedef struct _Payload* Payload;



static
int
threadFuncGive(
    RedThread  thread,
    void*      data,
    RedContext rCtx
    )
{
  int rc = RED_SUCCESS;

  red_u32 n;
  red_u32 prefix;

  Payload p = (Payload)data;

  rc = redSpinlockTake( p->lock );
  if (rc) return rc;

  prefix = p->mask;
  p->mask += 0x10000000;
  
  rc = redSpinlockGive( p->lock );
  if (rc) return rc;

  for (n = 0; n < p->loops; n++) {
    size_t s = (size_t)(prefix | n);
    rc = redQueueGive( p->queue, (void*)s );
    if (rc) return rc;
  }

  return RED_SUCCESS;
}

static
int
threadFuncTake(
    RedThread  thread,
    void*      data,
    RedContext rCtx
    )
{
  int rc = RED_SUCCESS;

  void*  d = NULL;
  size_t n = 0;

  Payload p = (Payload)data;

  for (n = 0; n < p->loops; n++) {
    rc = redQueueTake( p->queue, &d );
    if (rc) return rc;
  }

  return RED_SUCCESS;
}

static
int
threadFuncGiveTake(
    RedThread  thread,
    void*      data,
    RedContext rCtx
    )
{
  int rc = RED_SUCCESS;

  void*  d = NULL;
  size_t n = 0;

  Payload p = (Payload)data;

  for (n = 0; n < p->loops; n++) {
    rc = redQueueGive( p->queue, d );
    if (rc) return rc;

    rc = redQueueTake( p->queue, &d );
    if (rc) return rc;
  }

  return RED_SUCCESS;
}


static
int
createAndJoin(
    redfuncThread function,
    void*         data,
    size_t        threads,
    RedStream     conErr,
    RedThread*    redthreads,
    RedContext    rCtx
    )
{
  int rc = RED_SUCCESS;

  size_t i;

  for (i = 0; i < threads; i++) {
    RED_TEST_CALL_SILENTLY( redThreadCreate( &(redthreads[i]), NULL,
                              function, data, rCtx ),
                            RED_SUCCESS, "redThreadCreate( )" );
  }
  for (i = 0; i < threads; i++) {
    RED_TEST_CALL_SILENTLY( redThreadJoin( redthreads[i] ), RED_SUCCESS,
                            "redThreadJoin( )" );
  }

end:
  return rc;
}

static
int
assertAndDestroy(
    size_t      threads,
    RedStream   conErr,
    RedThread*  redthreads,
    RedContext  rCtx
    )
{
  int rc = RED_SUCCESS;

  size_t i;

  for (i = 0; i < threads; i++) {
    RED_ASSERT_SILENTLY( (RED_SUCCESS == redthreads[i]->rc),
                         "threads[n]->rc == n" );
    RED_ASSERT_SILENTLY( (redthreads[i]->done != 0),
                         "threads[n]->done != 0" );

    RED_TEST_CALL_SILENTLY( redThreadDestroy( &(redthreads[i]) ),
                            RED_SUCCESS, "redThreadDestroy( )" );
  }

end:
  return rc;
}

int
testQueue(
    RedStream  conErr,
    RedContext rCtx
    )
{
  int rc = RED_SUCCESS;

  RedThread redthreads[TEST_THREAD_COUNT_MAXIMUM] = { 0 };

  RedQueue queue = NULL;
  RedTime  now, then;

  struct _Payload p = { 0 };

  size_t loops   = 0;
  size_t threads = 0;

  size_t total;
  size_t i, j;

  size_t data[TEST_THREAD_COUNT_MAXIMUM] = { 0 };

  redStreamPrint( conErr, 0, "\nmemory usage capped at %ld bytes\n\n",
                  TEST_MEMORY_CAP );

  RED_TEST_CALL_SILENTLY( redSpinlockCreate( &(p.lock), rCtx ), RED_SUCCESS,
                          "redSpinlockCreate( )" );

  for (threads = 1; threads <= TEST_THREAD_COUNT_MAXIMUM; threads++) {

    loops = (TEST_MEMORY_CAP * 2) /
            (threads * sizeof(void*) * 3);
    total = threads * loops;

    for (j = 0; j < 2; j++) {
      RedTime give, take, givetake;

      if ( j ) {
        RED_TEST_CALL_SILENTLY( redQueueCreateNonblocking( &queue, total,
                                  total, rCtx ), RED_SUCCESS,
                                "redQueueCreateNonblocking( )" );
      }
      else {
        RED_TEST_CALL_SILENTLY( redQueueCreateBlocking( &queue, total, total,
                                  rCtx ), RED_SUCCESS,
                                "redQueueCreateBlocking( )" );
      }

      p.queue = queue;
      p.loops = loops;
      p.mask  = 0;

      redTimeNowPrecise( &then );
      RED_TEST_CALL_SILENTLY( createAndJoin( threadFuncGive, &p, threads,
                                conErr, redthreads, rCtx ), RED_SUCCESS,
                              "spawning and joining give threads" );
      redTimeNowPrecise( &now );
      give = now - then;

      RED_TEST_CALL_SILENTLY( assertAndDestroy( threads, conErr, redthreads,
                                rCtx ), RED_SUCCESS,
                              "asserting and destroying give threads" );

      RED_TEST_CALL_SILENTLY( redQueueLength( queue, &i ), RED_SUCCESS,
                              "redQueueLength( )" );

      RED_ASSERT_SILENTLY( (i == total), "queue length == total pushes" );

      redTimeNowPrecise( &then );
      RED_TEST_CALL_SILENTLY( createAndJoin( threadFuncTake, &p, threads,
                                conErr, redthreads, rCtx ), RED_SUCCESS,
                              "spawning and joining take threads" );
      redTimeNowPrecise( &now );
      take = now - then;

      RED_TEST_CALL_SILENTLY( assertAndDestroy( threads, conErr, redthreads,
                                rCtx ), RED_SUCCESS,
                              "asserting and destroying take threads" );


      RED_TEST_CALL_SILENTLY( redQueueLength( queue, &i ), RED_SUCCESS,
                              "redQueueLength( )" );
      RED_ASSERT_SILENTLY( (i == 0), "queue length == 0" );


      redTimeNowPrecise( &then );
      RED_TEST_CALL_SILENTLY( createAndJoin( threadFuncGiveTake, &p, threads,
                                conErr, redthreads, rCtx ), RED_SUCCESS,
                              "spawning and joining give/take threads" );
      redTimeNowPrecise( &now );
      givetake = now - then;

      RED_TEST_CALL_SILENTLY( assertAndDestroy( threads, conErr, redthreads,
                                rCtx ), RED_SUCCESS,
                              "asserting and destroying give/take threads" );


      RED_TEST_CALL_SILENTLY( redQueueLength( queue, &i ), RED_SUCCESS,
                              "redQueueLength( )" );
      RED_ASSERT_SILENTLY( (i == 0), "queue length == 0" );


      if (loops > TEST_THREAD_LOOPS_START * 2)
        redStreamPrint( conErr, 0, "%s : %d threads, %d loops @ avg %lld nsec/"
                        "give, %lld nsec/take, %lld nsec/give+take\n",
                        (j ? "nonblocking" : "blocking   "),
                        (int)threads, (int)loops, give / total, take / total,
                        givetake / total );

      RED_TEST_CALL_SILENTLY( redQueueDestroy( &queue ), RED_SUCCESS,
                              "redQueueDestroy( )" );
    }
  }

  threads = TEST_THREAD_COUNT_MAXIMUM;
  loops   = TEST_MEMORY_CAP / ((sizeof(void*) * 3 * threads) / 2);
  total   = threads * loops;

  redStreamPrint( conErr, 0, "\n" );

  RED_TEST_CALL( redQueueCreateNonblocking( &queue, 1, total, rCtx ),
                          RED_SUCCESS, "redQueueCreateNonblocking( )" );

  p.queue = queue;
  p.loops = loops;
  p.mask  = 0;

  for (i = 0; i < threads; i++) {
    data[i] =  i << 28;
  }

  RED_TEST_CALL_SILENTLY( createAndJoin( threadFuncGive, &p, threads,
                            conErr, redthreads, rCtx ), RED_SUCCESS,
                          "spawning and joining threads" );

  RED_TEST_CALL_SILENTLY( assertAndDestroy( threads, conErr, redthreads,
                            rCtx ), RED_SUCCESS,
                          "asserting and destroying threads" );


  RED_TEST_CALL( redQueueLength( queue, &i ), RED_SUCCESS, "redQueueLength( )" );
  RED_ASSERT( (i == total), "queue length == total pushes" );
  RED_TEST_CALL( redQueueGive( queue, NULL ), RED_FAIL_COLLECTION_FULL,
                 "redQueueGive( ) on full queue" );

  while (total--) {
    size_t value;
    void*  d;

    RED_TEST_CALL_SILENTLY( redQueueTake( queue, &d ), RED_SUCCESS,
                            "redQueueTake( )" );

    value = (size_t)d;
    i     = value >> 28;

    RED_ASSERT_SILENTLY( (data[i] == value), "queue data consistency" );
    data[i]++;
  }

  redStreamPrint( conErr, 0, "queue data consistency...pass\n" );
  redStreamPrint( conErr, 0, "queue grows to maximum...pass\n" );

  RED_TEST_CALL( redQueueTake( queue, (void**)&i ), RED_FAIL_COLLECTION_EMPTY,
                 "redQueueTake( ) on empty queue" );

  RED_TEST_CALL_SILENTLY( redQueueLength( queue, &i ), RED_SUCCESS,
                          "redQueueLength( )" );
  RED_ASSERT( (i == 0), "queue length == 0" );

  RED_TEST_CALL( redQueueDestroy( &queue ), RED_SUCCESS, "redQueueDestroy( )" );

  /********* blocking ********/

  redStreamPrint( conErr, 0, "\n" );

  total = loops * threads;
  
  RED_TEST_CALL( redQueueCreateBlocking( &queue, 1, total, rCtx ),
                          RED_SUCCESS, "redQueueCreateBlocking( )" );

  p.queue = queue;
  p.loops = loops;
  p.mask  = 0;

  for (i = 0; i < threads; i++) {
    data[i] =  i << 28;
  }

  RED_TEST_CALL_SILENTLY( createAndJoin( threadFuncGive, &p, threads,
                            conErr, redthreads, rCtx ), RED_SUCCESS,
                          "spawning and joining threads" );

  RED_TEST_CALL_SILENTLY( assertAndDestroy( threads, conErr, redthreads,
                            rCtx ), RED_SUCCESS,
                          "asserting and destroying threads" );


  RED_TEST_CALL( redQueueLength( queue, &i ), RED_SUCCESS, "redQueueLength( )" );
  RED_ASSERT( (i == total), "queue length == total pushes" );

  redTimeNowPrecise( &then );
  RED_TEST_CALL( redQueueGiveTimed( queue, NULL, RED_TIME_FROM_MSEC( 100 ) ),
                 RED_FAIL_TIMED_OUT,
                 "redQueueGiveTimed( 100ms ) on full queue" );
  redTimeNowPrecise( &now );

  RED_ASSERT( (now - then >= RED_TIME_FROM_MSEC(100)), "took > 100ms" );

  while (total--) {
    size_t value;
    void*  d;

    RED_TEST_CALL_SILENTLY( redQueueTake( queue, &d ), RED_SUCCESS,
                            "redQueueTake( )" );

    value = (size_t)d;
    i     = value >> 28;

    RED_ASSERT_SILENTLY( (data[i] == value), "queue data consistency" );
    data[i]++;
  }

  redStreamPrint( conErr, 0, "queue data consistency...pass\n" );
  redStreamPrint( conErr, 0, "queue grows to maximum...pass\n" );

  RED_TEST_CALL( redQueueTakeTimed( queue, (void**)&i,
                   RED_TIME_FROM_MSEC( 100 ) ), RED_FAIL_TIMED_OUT,
                 "redQueueTakeTimed( 100ms ) on empty queue" );

  RED_ASSERT( (now - then >= RED_TIME_FROM_MSEC(100)), "took > 100ms" );

  RED_TEST_CALL_SILENTLY( redQueueLength( queue, &i ), RED_SUCCESS,
                          "redQueueLength( )" );
  RED_ASSERT( (i == 0), "queue length == 0" );

  RED_TEST_CALL( redQueueDestroy( &queue ), RED_SUCCESS, "redQueueDestroy( )" );

end:

  redSpinlockDestroy( &(p.lock) );

  return rc;
}

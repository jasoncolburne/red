/* Copyright (C) 2010-2022, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: testthread.c 21 2010-04-28 17:38:52Z jason $
 */

#include "main.h"

#include "red_memory.h"
#include "_red_thread.h"

#define TEST_THREAD_COUNT 8
#define TEST_STACK_SIZE   0x4000

RED_TEST_UNIT_IDENTIFY( "thread.c" );

static RedStream stream;

static
int
threadFunc(
    RedThread  thread,
    void*      data,
    RedContext rCtx
    )
{
  int n = *(int*)data;

  redStreamPrint( stream, 0, "i am thread %i!\n", n );

  return n;
}



RED_TEST_UNIT_IMPLEMENTATION_BEGIN( testThread );

  int i;

  int* data = NULL;
  int* p;

  int deny = 0;
  int done = 0;

  size_t s = TEST_STACK_SIZE;

  RedThread*          threads    = NULL;
  RedThreadAttributes attributes = NULL;

  stream = conErr;

  RED_TEST_CALL_SILENTLY( redMalloc( rCtx, (void**)&threads,
                            sizeof(RedThread) * TEST_THREAD_COUNT),
                          RED_SUCCESS, "redMalloc( )" );

  RED_TEST_CALL_SILENTLY( redMalloc( rCtx, (void**)&data,
                            sizeof(int) * TEST_THREAD_COUNT ),
                          RED_SUCCESS, "redMalloc( )" );

  RED_TEST_CALL( redThreadAttributesCreate( &attributes, rCtx ),
                 RED_SUCCESS, "redThreadAttributesCreate()" );

  RED_TEST_CALL( redThreadAttributesAssign( attributes,
                   RED_ATTRIBUTE_THREAD_JOINABLE, NULL, 0 ),
                 RED_SUCCESS, "redThreadAttributesAssign( ..., "
                 "RED_ATTRIBUTE_THREAD_JOINABLE, ... )" );

  RED_TEST_CALL( redThreadAttributesAssign( attributes,
                   RED_ATTRIBUTE_THREAD_STACKSIZE, &s, sizeof(s) ),
                 RED_SUCCESS, "redThreadAttributesAssign( ..., "
                 "RED_ATTRIBUTE_THREAD_STACKSIZE, N, ... )" );

  /* negative tests */
  RED_TEST_CALL( redThreadCreate( NULL, NULL, NULL, NULL, rCtx ),
                 RED_ERR_NULL_POINTER, "redThreadCreate( NULL, ... )" );

  threads[0] = (RedThread)0x1;
  RED_TEST_CALL( redThreadCreate( &(threads[0]), attributes, NULL, NULL, rCtx ),
                 RED_ERR_INITIALIZED_POINTER,
                 "redThreadCreate( &thread, ... ), thread != NULL" );
  threads[0] = NULL;

  redStreamPrint( conErr, 0, "---spawning %d joinable threads---\n",
                  TEST_THREAD_COUNT );

  p = data;
  for (i = 0; i < TEST_THREAD_COUNT; i++) {
    *p = i;
    RED_TEST_CALL_SILENTLY( redThreadCreate( &(threads[i]), attributes,
                              threadFunc, p++, rCtx ),
                            RED_SUCCESS, "redThreadCreate( )" );
  }
  
  for (i = 0; i < TEST_THREAD_COUNT; i++) {
    RED_TEST_CALL_SILENTLY( redThreadJoin( threads[i] ), RED_SUCCESS,
                            "redThreadJoin( )" );
    RED_ASSERT_SILENTLY( (i == threads[i]->rc), "threads[n]->rc == n" );
    RED_ASSERT_SILENTLY( (threads[i]->done != 0), "threads[n]->done != 0" );
    
    RED_TEST_CALL_SILENTLY( redThreadDestroy( &(threads[i]) ), RED_SUCCESS,
                            "redThreadDestroy( )" );
  }
  redStreamPrint( conErr, 0, "---joined %d threads---\n", TEST_THREAD_COUNT );
  redStreamPrint( conErr, 0, "threads all finished...pass.\n" );
  redStreamPrint( conErr, 0, "correct return values...pass.\n" );

  RED_TEST_CALL( redThreadAttributesAssign( attributes,
                   RED_ATTRIBUTE_THREAD_DETACHED, NULL, 0 ),
                 RED_SUCCESS, "redThreadAttributesAssign( ..., "
                 "RED_ATTRIBUTE_THREAD_DETACHED, ... )" );

  redStreamPrint( conErr, 0, "---spawning %d detached threads---\n",
                  TEST_THREAD_COUNT );

  p = data;
  for (i = 0; i < TEST_THREAD_COUNT; i++) {
    *p = i;
    RED_TEST_CALL_SILENTLY( redThreadCreate( &(threads[i]), attributes,
                              threadFunc, p++, rCtx ), RED_SUCCESS,
                            "redThreadCreate( )" );
  }

  for (i = 0; i < TEST_THREAD_COUNT; i++) {
    rc = redThreadJoin( threads[i] );
    RED_ASSERT_SILENTLY( (rc != RED_SUCCESS), "redThreadJoin( detached ) != "
                         "RED_SUCCESS" );
    if (rc == RED_ERR_THREAD_DNE)          done++;
    if (rc == RED_ERR_THREAD_NOT_JOINABLE) deny++;
  }
  redStreamPrint( conErr, 0, "%d threads already finished, %d threads"
                  " unjoinable\n", done, deny);
  RED_ASSERT( (done + deny == TEST_THREAD_COUNT),
              "redThreadJoin( ) fails for all detached threads");
  
  for (i = 0; i < TEST_THREAD_COUNT; i++) {
    //rc = redThreadJoin( threads[i] );
    while (!(threads[i]->done));
    RED_ASSERT_SILENTLY( (i == threads[i]->rc), "threads[n]->rc == n" );
    RED_ASSERT_SILENTLY( (threads[i]->done != 0), "threads[n]->done != 0" );

    RED_TEST_CALL_SILENTLY( redThreadDestroy( &(threads[i]) ), RED_SUCCESS,
                            "redThreadDestroy( )" );
  }
  redStreamPrint( conErr, 0, "threads all finished...pass.\n" );
  redStreamPrint( conErr, 0, "correct return values...pass.\n" );

RED_TEST_UNIT_IMPLEMENTATION_CLEANUP();

  redFree( rCtx, (void**)&threads, 0 );
  redFree( rCtx, (void**)&data, 0 );
  redThreadAttributesDestroy( &attributes );

RED_TEST_UNIT_IMPLEMENTATION_END();

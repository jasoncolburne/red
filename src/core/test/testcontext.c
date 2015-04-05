/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: testcontext.c 18 2010-04-17 05:57:03Z jason $
 */

#include "main.h"

#include "red_memory.h"

RED_TEST_UNIT_IDENTIFY( "context.c" );

#define TEST_CONTEXT_ALLOC_SIZE (3 * 1024 * 1024)


RED_TEST_UNIT_IMPLEMENTATION_BEGIN( testContext );

  RedContext testRCtx;
  void*      data = NULL;

  /* null ptr */
  RED_TEST_CALL( redContextCreateDefault( NULL ), RED_ERR_NULL_POINTER,
                 "redContextCreateDefault( NULL )" );

  /* already initialized */
  testRCtx = (RedContext) 0x1;
  RED_TEST_CALL( redContextCreateDefault( &testRCtx ),
                 RED_ERR_INITIALIZED_POINTER,
                 "redContextCreateDefault( &rCtx ), rCtx != NULL" );

  /* normal init */
  testRCtx = NULL;
  RED_TEST_CALL( redContextCreateDefault( &testRCtx ), RED_SUCCESS,
                 "redContextCreateDefault( &rCtx ), rCtx == NULL" );
  RED_ASSERT( (testRCtx != NULL), "rCtx != NULL" );

  RED_TEST_CALL( redMalloc( testRCtx, NULL, TEST_CONTEXT_ALLOC_SIZE ),
                 RED_ERR_NULL_POINTER, "redMalloc( ..., NULL, ... )" );

  RED_TEST_CALL( redMalloc( NULL, &data, TEST_CONTEXT_ALLOC_SIZE ),
                 RED_ERR_NULL_CONTEXT, "redMalloc( NULL, ... )" );

  RED_TEST_CALL( redMalloc( testRCtx, &data, 0 ), RED_SUCCESS,
                 "redMalloc( ..., 0 )" );
  RED_ASSERT( (data == NULL), "data == NULL" );

  RED_TEST_CALL( redMalloc( testRCtx, &data, TEST_CONTEXT_ALLOC_SIZE ),
                 RED_SUCCESS, "redMalloc( ... )" );
  RED_ASSERT( (data != NULL), "data != NULL" );

  RED_TEST_CALL( redFree( testRCtx, &data, TEST_CONTEXT_ALLOC_SIZE ),
                 RED_SUCCESS, "redFree( ... )" );

  RED_ASSERT( (data == NULL), "data == NULL" );

  RED_TEST_CALL( redContextDestroy( &testRCtx ), RED_SUCCESS,
                 "redContextDestroy( &rCtx )" );
  RED_ASSERT( (testRCtx == NULL), "rCtx == NULL" );

RED_TEST_UNIT_IMPLEMENTATION_CLEANUP();

RED_TEST_UNIT_IMPLEMENTATION_END();

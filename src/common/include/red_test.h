/* Copyright (C) 2015, Redbeard Enterprises Ltd.
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_test.h 19 2010-04-22 08:27:08Z jason $
 */

#ifndef __RED_TEST_H__
#define __RED_TEST_H__


#include "red_context.h"
#include "red_initialize.h"
#include "red_return.h"
#include "red_stream.h"

/* welcome to macro city:
 *
 */


/**
 */
#define RED_TEST_MAIN_BEGIN( ) \
int main(void) {               \
  int rc = RED_SUCCESS;        \
                               \
  RedContext rCtx   = NULL;    \
  RedStream  conErr = NULL;    \
                               \
  rc = redContextCreateDefault( &rCtx ); \
  if (rc) goto end;            \
                               \
  rc = redInitialize( rCtx );  \
  if (rc) goto end;            \
                               \
  rc = redStreamCreateConsoleError( &conErr, rCtx ); \
  if (rc) goto end

#if 0

#endif

/**
 */
#define RED_TEST_MAIN_END( )   \
end:                           \
  redStreamDestroy( &conErr ); \
  redDeinitialize( );          \
  redContextDestroy( &rCtx );  \
                               \
  return rc;                   \
}                              \
                               \
void _dummydFASdfasdFoijq(void)
/* want to somehow handle the ; */



/**
 */
#define RED_TEST_UNIT_IDENTIFY( s ) static const char _redTestUnitId[] = s



/**
 */
#define RED_TEST_UNIT_CALL( func, desc ) \
  do {                          \
    redStreamPrint( conErr, 0, "=-- Testing %s --=\n", desc ); \
    rc = func( conErr, rCtx );                  \
    if (rc) goto end;           \
    redStreamPrint( conErr, 0, "\n" );    \
  } while (0)



/**
 */
#define RED_TEST_UNIT_PROTOTYPE( func ) \
int func( RedStream conErr, RedContext rCtx )



/**
 */
#define RED_TEST_UNIT_IMPLEMENTATION_BEGIN( func ) \
int func(                         \
    RedStream   conErr,           \
    RedContext  rCtx              \
    )                             \
{                                 \
  int rc = RED_SUCCESS



/**
 */
#define RED_TEST_UNIT_IMPLEMENTATION_CLEANUP( ) \
end:       \
  while(0)



/**
 */
#define RED_TEST_UNIT_IMPLEMENTATION_END( ) \
  return rc;                    \
}                               \
                                \
void _dummydFAWERPOuiurjq(void)



/**
 */
#define RED_TEST_CALL( call, expect, msg ) \
  redStreamPrint( conErr, 0, "calling %s...", msg ); \
                                           \
  rc = call;                               \
  if (rc != expect) {                      \
      redStreamPrint( conErr, 0, "fail [%s:%d]\n", _redTestUnitId, __LINE__ ); \
    redStreamPrint( conErr, 0, "returned 0x%08x [expected 0x%08x]\n", rc, expect ); \
    if (!rc)                               \
      rc = RED_FAIL_TEST_CASE;             \
    goto end;                              \
  }                                        \
  redStreamPrint( conErr, 0, "pass\n" )



/**
 */
#define RED_TEST_CALL_SILENTLY( call, expect, msg ) \
  rc = call;                                        \
  if (rc != expect) do {                               \
    redStreamPrint( conErr, 0, "calling %s...fail [%s:%d]\n", msg, _redTestUnitId, __LINE__ ); \
    redStreamPrint( conErr, 0, "returned 0x%08x [expected 0x%08x]\n", rc, expect ); \
    if (!rc)                                        \
      rc = RED_FAIL_TEST_CASE;                      \
    goto end;                                       \
  } while(0)



/**
 */
#define RED_ASSERT( cond, msg ) \
  do {                          \
    redStreamPrint( conErr, 0, "%s...", msg );  \
    if (cond)                   \
      redStreamPrint( conErr, 0, "pass\n" );    \
    else {                      \
      redStreamPrint( conErr, 0, "fail [%s:%d]\n", _redTestUnitId, __LINE__ );    \
          if(!rc)                 \
            rc = RED_FAIL_TEST_CASE;         \
      goto end;               \
    }                           \
  } while (0)



/**
 */
#define RED_ASSERT_SILENTLY( cond, msg ) \
  do {                          \
    if (!cond) {                \
        redStreamPrint( conErr, 0, "%s...fail [%s:%d]\n", msg, _redTestUnitId, __LINE__ );  \
          if(!rc)                 \
            rc = RED_FAIL_TEST_CASE;         \
      goto end;               \
    }                           \
  } while (0)



/**
 */

#endif /* __RED_TEST_H__ */

/* Copyright (C) 2010-2022, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_return.h 19 2010-04-22 08:27:08Z jason $
 */

#ifndef __RED_RETURN_H__
#define __RED_RETURN_H__


#define RED_SUCCESS                      0x0
#define RED_ERR_BASE                     0x9000
#define RED_ERR_BASE_STREAM              0x9100
#define RED_ERR_BASE_CONCURRENCY         0x9200
#define RED_ERR_BASE_DATA                0x9300


/* pointer errors */
#define RED_ERR_NULL_POINTER             (RED_ERR_BASE | 0x00)
#define RED_ERR_INITIALIZED_POINTER      (RED_ERR_BASE | 0x01)

/* context errors */
#define RED_ERR_NULL_CONTEXT             (RED_ERR_BASE | 0x10)

/* memory failures */
#define RED_FAIL_ALLOC                   (RED_ERR_BASE | 0x80)
#define RED_FAIL_OUT_OF_RESOURCES        (RED_ERR_BASE | 0x81)

/* general errors */
#define RED_NOT_IMPLEMENTED              (RED_ERR_BASE | 0xf0)
#define RED_ERR_INVALID_FLAG             (RED_ERR_BASE | 0xf1)
#define RED_ERR_INVALID_ARGUMENT         (RED_ERR_BASE | 0xf2)
#define RED_FAIL_TEST_CASE               (RED_ERR_BASE | 0xf3) /* should not see in production */
#define RED_ERR_NOT_SUPPORTED            (RED_ERR_BASE | 0xf4)
#define RED_FAIL_TIMED_OUT               (RED_ERR_BASE | 0xf5)
#define RED_ERR_LIBRARY_INITIALIZED      (RED_ERR_BASE | 0xf6)
#define RED_ERR_LIBRARY_NOT_INITIALIZED  (RED_ERR_BASE | 0xf7)

/* stream errors */
#define RED_ERR_UNSUPPORTED_BY_STREAM    (RED_ERR_BASE_STREAM | 0x00)

/* concurrency errors */
#define RED_ERR_THREAD_ATTR_SIZE         (RED_ERR_BASE_CONCURRENCY | 0x00)
#define RED_ERR_THREAD_DNE               (RED_ERR_BASE_CONCURRENCY | 0x01)
#define RED_ERR_THREAD_NOT_JOINABLE      (RED_ERR_BASE_CONCURRENCY | 0x02)

#define RED_FAIL_ATOMIC_COMPARISON       (RED_ERR_BASE_CONCURRENCY | 0x10)

/* data structure errors */
#define RED_FAIL_COLLECTION_END          (RED_ERR_BASE_DATA | 0x00)
#define RED_FAIL_COLLECTION_EMPTY        (RED_ERR_BASE_DATA | 0x01)
#define RED_FAIL_COLLECTION_FULL         (RED_ERR_BASE_DATA | 0x02)

#define RED_FAIL_OUTSTANDING_RESOURCES   (RED_ERR_BASE_DATA | 0x10)


#endif /* __RED_RETURN_H__ */

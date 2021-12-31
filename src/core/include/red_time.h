/* Copyright (C) 2010-2021, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_time.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef __RED_TIME_H__
#define __RED_TIME_H__

#include "red_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* units are nanoseconds, giving approx +/- 292 years range */
typedef red_i64 RedTime;

#define RED_TIME_AS_DAYS( t )    ((t) / 86400000000000LL)
#define RED_TIME_AS_HOURS( t )   ((t) / 3600000000000LL)
#define RED_TIME_AS_MINUTES( t ) ((t) / 60000000000LL)
#define RED_TIME_AS_SECONDS( t ) ((t) / 1000000000LL)
#define RED_TIME_AS_MSEC( t )    ((t) / 1000000LL)
#define RED_TIME_AS_USEC( t )    ((t) / 1000LL)
#define RED_TIME_AS_NSEC( t )     (t)

#define RED_TIME_FROM_DAYS( t )    (RedTime)(86400000000000LL * (t))
#define RED_TIME_FROM_HOURS( t )   (RedTime)( 3600000000000LL * (t))
#define RED_TIME_FROM_MINUTES( t ) (RedTime)(   60000000000LL * (t))
#define RED_TIME_FROM_SECONDS( t ) (RedTime)(    1000000000LL * (t))
#define RED_TIME_FROM_MSEC( t )    (RedTime)(       1000000LL * (t))
#define RED_TIME_FROM_USEC( t )    (RedTime)(          1000LL * (t))
#define RED_TIME_FROM_NSEC( t )    (RedTime)(t)

#define RED_TIME_RATE_PER_DAY( time, trials )    (RED_TIME_FROM_DAYS( 1 ) * \
                                                  trials / time)
#define RED_TIME_RATE_PER_HOUR( time, trials )   (RED_TIME_FROM_HOURS( 1 ) * \
                                                  trials / time)
#define RED_TIME_RATE_PER_MINUTE( time, trials ) (RED_TIME_FROM_MINUTES( 1 ) * \
                                                  trials / time)
#define RED_TIME_RATE_PER_SECOND( time, trials ) (RED_TIME_FROM_SECONDS( 1 ) * \
                                                  trials / time)
#define RED_TIME_RATE_PER_MSEC( time, trials )   (RED_TIME_FROM_MSEC( 1 ) * \
                                                  trials / time)
#define RED_TIME_RATE_PER_USEC( time, trials )   (RED_TIME_FROM_USEC( 1 ) * \
                                                  trials / time)

extern
int
redSleep(
    RedTime duration
    );

/* time since epoch (1970) */
extern
int
redTimeNow(
    RedTime* time
    );

/* not gauranateed to be relative to epoch, but should be nsec precision
 * relative to other redTimeNowPrecision() calls */
extern
int
redTimeNowPrecise(
    RedTime* time
    );

#ifdef __cplusplus
}
#endif


#endif /* __RED_TIME_H__ */

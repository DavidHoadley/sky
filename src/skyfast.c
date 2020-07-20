/*==============================================================================
 * sunfast.c - <one-line description>
 *
 * Author:  David Hoadley
 *          Loco2Gen
 *          ABN 22 957 381 638
 *
 * Description: (see sunfast.h)
 * 
 *
 * Copyright (C) 2020 David Hoadley <vcrumble@westnet.com.au>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *==============================================================================
 */

/* ANSI includes etc. */
#include <math.h>
// /+
#include <stdio.h>
#include "astc1.h"
#include "rdwrite.h"
#include "test.h"
// /-

/* Local and project includes */
#include "sunfast.h"

#include "sky0.h"
#include "astron.h"
#include "general.h"
#include "sun.h"

/*
 * Local #defines and typedefs
 */
DEFINE_THIS_FILE;                       // For use by REQUIRE() - assertions.

/*      --- Definitions that you may need to or wish to modify --- */
//#define BARE_METAL
#ifdef BARE_METAL
#define startCriticalSection()      disableInterrupts()
#define endCriticalSection()        enableInterrupts()

#else
#include <pthread.h>

#define startCriticalSection()      pthread_mutex_lock(&mutex)
#define endCriticalSection()        pthread_mutex_unlock(&mutex)
#endif

#if 0
///#define SLOW_RECALC_RATE_mins       60  // Time between recalculations (minutes)
///#define SLOW_RECALC_RATE_mins       750  // Time between recalculations (minutes)
#define SLOW_RECALC_RATE_mins       1440  /* Time between recalcs (minutes) */


/*      --- Definitions that are calculated from the above #define(s).
            Don't edit these directly --- */

/*      Derived from the above. Note, because I use integer arithmetic here, I
        am assuming that you have chosen a value of SLOW_RECALC_RATE_mins that
        divides exactly into one day. */
#define RECALCS_PER_DAY            (1440 / SLOW_RECALC_RATE_mins)
#define SLOW_RECALC_RATE_cy        (SLOW_RECALC_RATE_mins / (1440.0 * JUL_CENT))
#endif


/*
 * Prototypes for local functions (not called from other modules)
 */


/*
 * Global variables accessible by other modules
 */


/*
 * Local variables (not accessed by other modules)
 */
LOCAL Sky_PosEq     lfiA, lfiB, lfiC;
LOCAL Sky_PosEq     *last = &lfiA;      // items calculated for time in past
LOCAL Sky_PosEq     *next = &lfiB;      // items calculated for time ahead
LOCAL Sky_PosEq     *oneAfter = &lfiC;  // items calculated for time after next
LOCAL volatile bool oneAfterIsValid = false;
LOCAL pthread_mutex_t mutex;

LOCAL void (*callback)(double j2kTT_cy, Sky_PosEq *pos);
LOCAL double        recalcInterval_cy = 0.0; // time between full recalculations

/*
 *==============================================================================
 *
 * Implementation
 *
 *==============================================================================
 *
 * Global functions callable by other modules
 *
 *------------------------------------------------------------------------------
 */
GLOBAL void skyfast_init(double mjdUtc,
                         int    fullRecalcInterval_mins,
                         const Asttime_DeltaTs *deltas,
                         void (*getApparent)(double j2kTT_cy,
                                             Sky_PosEq *pos)
                         )
/*! Initialise those items that take a long time to calculate, but which do not
    need to be recalculated frequently. This routine calls a function that you
    supply to calculate the apparent coordinates of a celestial object, its
    distance, and the Equation of the Equinoxes, as derived from nutation
    calculations. This routine calls that function
        1.  for the time specified by \a mjdUtc,
        2.  for time \a mjdUtc + \a fullRecalcalcInterval_mins, and
        3.  for time \a mjdUtc + 2 x \a fullRecalcalcInterval_mins.
        .
    For example, to track the Sun, specify the function sun_nrelApparent() when
    calling this routine. To track the Moon, specify the function 
    moon_nrelApparent(). 

    The routine skyfast_getApprox() can then be called
    (at a high frequency if required) to calculate the current position of the
    object, using these values to do it.
 \param[in]  mjdUtc  Modified Julian Date (=JD - 2 400 000.5), UTC timescale
 \param[in]  fullRecalcInterval_mins
                     Interval of time between full recalculation of the object's
                     position using the function supplied to \a getApparent
                     (minutes). This value must be greater than zero.
 \param[in]  deltas  Delta T values, as set by the asttime_init() (or 
                     asttime_initSimple() or asttime_initDetailed()) routines
 \param      getApparent
                     Function to get the position of a celestial object in
                     apparent coordinates (i.e. referred to the true equinox and
                     equator at the time), in rectangular form.

 \par When to call this function
    At program initialisation time.
 \note
    Although the parameter name \a getApparent suggests that you must supply a
    function which calculates an object's position in apparent coordinates, it
    is also possible to supply a function which returns the position in
    Celestial Intermediate coordinates (i.e. referred to the true equator and
    the Celestial Intermediate Origin (CIO) at time \a t_cy) instead. If so,
    the function does not need to fill in the \a eqEq_rad field of struct
    Sky_PosEq.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
{
    double  calcTimeTT_d;
    double  calcTimeTT_cy;
    int     ret;

    REQUIRE_NOT_NULL(deltas);
    REQUIRE_NOT_NULL(getApparent);
    REQUIRE(fullRecalcInterval_mins > 0);

#ifndef BARE_METAL
    ret = pthread_mutex_init(&mutex, NULL);
    ASSERT(ret == 0);   // There is no possible recovery from an error here.
#endif
    /* Save the function address for later call by skyfast_backgroundUpdate() */
    callback = getApparent;
    
    /* Save the recalculation rate, converted from minutes to centuries. */
    recalcInterval_cy = fullRecalcInterval_mins / (1440.0 * JUL_CENT);

    calcTimeTT_d = mjdUtc + deltas->deltaTT_d;
    /* Round down to nearest SLOW_RECALC_RATE_mins (e.g. nearest hour) */
//    calcTimeTT_d = floor(calcTimeTT_d * RECALCS_PER_DAY) / RECALCS_PER_DAY;
    calcTimeTT_cy = (calcTimeTT_d - MJD_J2000) / JUL_CENT;
    getApparent(calcTimeTT_cy, last);
    
    /* Now do the same for the next time (e.g. next hour) */
    calcTimeTT_cy += recalcInterval_cy;
    getApparent(calcTimeTT_cy, next);

    /* And again for the time after */
    calcTimeTT_cy += recalcInterval_cy;
    getApparent(calcTimeTT_cy, oneAfter);
    oneAfterIsValid = true;
}


GLOBAL void skyfast_backgroundUpdate(void)
/*! Recalculation of the low frequency quantities.
    Checks whether the calculations of the celestial object's apparent
    coordinates and the equation of the equinoxes have been performed for the
    time called "oneAfter" (i.e. the time after the time called "next"). If they
    have not, this function performs those calculations.
 
    This function calls the function that you previously supplied to function
    skyfast_init() (parameter \a getApparent).
   
 \par When to call this function
    This function is designed to be called in a low priority loop, or at
    background level, using any available leftover processor time to slowly
    precalculate values for the "time after next". It needs to have been called
    before the high frequency routine skyfast_getApprox() (which is
    interpolating between time "last" and time "next") arrives at time "next"
    and therefore needs to access the data for time "oneAfter".
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
{
    double  t_cy;

    REQUIRE(recalcInterval_cy > 0.0);   /* skyfast_init() must be called before now */

    if (!oneAfterIsValid) {
        // /+
        printf("Updating the low frequency items\n");
        // /-
        t_cy = next->timestamp_cy + recalcInterval_cy;
        callback(t_cy, oneAfter);

        startCriticalSection();
        oneAfterIsValid = true;
        endCriticalSection();
    }
}



GLOBAL void skyfast_getApprox(double t_cy,
                              Sky_PosEq *approx)
/*! Get the best approximation to the celestial object's apparent coordinates
    and distance, and the equation of the equinoxes, based on an interpolation
    between two sets of such data that we have previously calculated.
 \param[in]  t_cy     Julian centuries since J2000.0, TT timescale. This must
 *                    specify a time no earlier than the time specified in
 *                    argument \a mjdUtc in the call to skyfast_init(), and
 *                    preferably within the range
 *                    [\a mjdUtc, \a mjdUtc + \a fullRecalcalcInterval_mins].
 \param[out] approx   position vector, distance, etc, obtained by interpolation

 \note
    This function will return an approximate position in apparent coordinates
    (i.e. referred to the true equator and equinox at time \a t_cy) if the
    function that you passed to the skyfast_init() function returned apparent
    coordinates. This function will return an approximate position in
    Celestial Intermediate coordinates (i.e. referred to the true equator and
    the Celestial Intermediate Origin (CIO)) at time \a t_cy) if the
    function that you passed to the skyfast_init() function returned CIRS
    coordinates.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
{
    Sky_PosEq *temp;
    double a;
    double b;

    if (t_cy > next->timestamp_cy) {
        /* Time t_cy is no longer between last and next, so we need to make next
           and oneAfter become the new last and next respectively. But this
           requires that our low frequency/low priority routine has completed
           filling in all the data for oneAfter. */
        REQUIRE(oneAfterIsValid);
        
        startCriticalSection();
        temp = last;
        last = next;
        next = oneAfter;
        oneAfter = temp;
        oneAfterIsValid = false;
        endCriticalSection();
    }

    /* It is a programming error if time t_cy is not between last and next */
    REQUIRE(t_cy >= last->timestamp_cy);
    REQUIRE(next->timestamp_cy >= t_cy);

    if ((next->timestamp_cy - last->timestamp_cy) < SFA) {
        a = 0.0;
        b = 1.0;
    } else {
        a = (t_cy - last->timestamp_cy)
            / (next->timestamp_cy - last->timestamp_cy);
        b = 1.0 - a;
    }
    /* Do a simple linear interpolation between the two appCirsV position
     * vectors last and next. Unlike the two appCirsV vectors, the resulting
     * vector will not be exactly of unit magnitude. But if the two appCirsV
     * vectors are less than one degree apart, the resulting position error is
     * very small (< 0.3′). If the two appCirsV are a few arcminutes
     * apart, the magnitude error of the resulting vector is negligible.  */
    approx->appCirsV.a[0] =  a * next->appCirsV.a[0] + b * last->appCirsV.a[0];
    approx->appCirsV.a[1] =  a * next->appCirsV.a[1] + b * last->appCirsV.a[1];
    approx->appCirsV.a[2] =  a * next->appCirsV.a[2] + b * last->appCirsV.a[2];
    /* And a linear interpolation of the other two quantities also. */
    approx->distance_au = a * next->distance_au + b * last->distance_au;
    approx->eqEq_rad    = a * next->eqEq_rad + b * last->eqEq_rad;
}


/*
 *------------------------------------------------------------------------------
 *
 * Local functions (not called from other modules)
 *
 *------------------------------------------------------------------------------
 */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef SKYFAST_H
#define SKYFAST_H
/*============================================================================*/
/*! \file
 * \brief
 * skyfast.h - set up and use interpolation for rapid calculation of a celestial
 *             object's apparent coordinates.
 *
 * \author  David Hoadley
 *
 * \details
 *          Routines to set up interpolation for celestial tracking, get the
 *          interpolated position at a given time, and to update the endpoints
 *          used by the interpolation algorithm. The error introduced by using
 *          interpolation rather than fully calculating each and every position
 *          can be very small - see \ref page-interpolation (the end of this
 *          source file)
 * 
 *==============================================================================
 */
/*
 * Copyright (c) 2020, David Hoadley <vcrumble@westnet.com.au>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "sky.h"

/*
 * Global #defines and typedefs
 */


#ifdef __cplusplus
extern "C" {
#endif
/*
 * Global functions available to be called by other modules
 */
void skyfast_init(double            tStartUtc_d,
                  int               fullRecalcInterval_mins,
                  const Sky_DeltaTs *deltas,
                  void (*getApparent)(double j2kTT_cy, Sky_TrueEquatorial *pos)
                  );
void skyfast_backgroundUpdate(void);
void skyfast_getApprox(double t_cy, Sky_TrueEquatorial *approx);

/*
 * Global variables accessible by other modules
 */

#ifdef __cplusplus
}
#endif

/*! \page page-interpolation Interpolation and its errors
 *  The interpolation process obtains an estimate of the apparent position (or 
 *  alternatively, the celestial intermediate position) of a celestial object
 *  by interpolating between previously calculated position vectors. The amount
 *  of error introduced by this process depends of course on how much the
 *  position changed between the two previously calculated times. Here is a
 *  table of the errors when this process is used to obtain the position of the
 *  Sun. The errors are calculated from
 *      sqrt((azimuth_error * cos(elevation))^2 + elevation_error^2)
 *
 *  The interpolation interval (in hours) is the time between the two previously
 *  fully calculated vectors. It is the value supplied to parameter \a 
 *  
 *  ###Maximum absolute position error (arcseconds) for different interpolation intervals (hours)
 * hours|1      |2     |3     |4     |5     |6     |8     |10    |12    |15    |16    |18    |20    |22    |24    |
 * :----|------:|-----:|-----:|-----:|-----:|-----:|-----:|-----:|-----:|-----:|-----:|-----:|-----:|-----:|-----:|
 * Sun  |0.0005 |0.002 |0.005 |0.008 |0.013 |0.018 |0.033 |0.051 |0.074 |0.115 |0.131 |0.166 |0.205 |0.248 |0.296 |
 *
 *  As can be seen, the errors are very small for the Sun even with 24 hours
 *  between full calculations (i.e. a value of 1440 minutes supplied to
 *  parameter \a fullRecalcInterval_mins of function skyfast_init()).
 *  Actually a full twenty-four hours of tracking of the Sun can be obtained by
 *  supplying 720 minutes to this parameter, without ever having to call the
 *  function skyfast_backgroundUpdate(). This is because skyfast_init()
 *  calculates three apparent positions, not just two.
 * 
 */

#endif /* SKYFAST_H */

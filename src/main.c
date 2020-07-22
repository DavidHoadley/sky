/*============================================================================*/
/*!\file
 * 
 * \brief main.c - Simple demo program for Sun position using rectangular
 *                   coordinates
 *
 * \author  David Hoadley <vcrumble@westnet.com.au>
 *
 * \details
 *      See [the main page](@ref index) (or the end of this source file) for a
 *      description of this file.
 * 
 * \copyright
 * \parblock
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
 * \endparblock
 *
 *==============================================================================
 */

/* ANSI includes etc. */
#include <stdio.h>
#include <time.h>
#include <unistd.h>

/* Local and project includes */
#include "general.h"
#include "sky.h"
#include "skyio.h"
#include "sun.h"
#include "skyfast.h"
#include "vectors3d.h"

#define SITE_LATITUDE_deg      -37.8236        /* Richmond, Melbourne */
#define SITE_LONGITUDE_deg     144.9913
#define SITE_HEIGHT_m          10.0
#define SITE_PRESSURE_hPa      1013.0
#define SITE_TEMPERATURE_degC  15.0
#define SITE_TIMEZONE_h        10.0


void demo1(void);
void demo2(void);
void sunTopocentricFast(double             j2kUtc_d,
                        const Sky_DeltaTs  *dTs,
                        const Sky_SiteProp *site,
                        Sky_SiteHorizon *topo);



int main(void)
{
    demo1();
    demo2();    
}



void sunTopocentricFast(double             j2kUtc_d,
                        const Sky_DeltaTs  *dTs,
                        const Sky_SiteProp *site,
                        Sky_SiteHorizon *topo)
/*! Uses previously calculated values of the Sun's apparent coordinates and
    interpolation to quickly calculate the Sun's topocentric position.
 \param[in]  j2kUtc_d  Days since Greenwich noon, 1-Jan-2000, UTC timescale
 \param[in]  deltas    Delta T values, as set by the sky_initTime() (or 
                       sky_initTimeSimple() or sky_initTimeDetailed()) routines
 \param[in]  site      Properties of the observing site, particularly its
                       geometric location on the surface of the Earth, as set by
                       the sky_setSiteLocation() function (or sky_setSiteLoc2())
 \param[out] topo      Topocentric position, in both rectangular (unit vector)
                       form, and as Azimuth and Elevation (altitude) angles

 \par When to call this function
    This function is designed to be called at high frequency (more than once
    per second, even up to, say, 20 Hz).
 \par
    Use this function when you have made a previous call to skyfast_init().
    Depending on how long a period you want to track the sun using this
    routine, you may also need to run the function skyfast_backgroundUpdate() as
    a low frequency, low priority routine, to update the interpolation ranges.
    If you have not done this, call sun_nrelTopocentric() instead.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
{
    Sky_Times           atime;
    Sky_TrueEquatorial  approx;
    V3D_Vector          terInterV;// unit vector in Terrestrial Intermed Ref Sys
 
    REQUIRE_NOT_NULL(dTs);
    REQUIRE_NOT_NULL(site);
    REQUIRE_NOT_NULL(topo);

    sky_updateTimes(j2kUtc_d, dTs, &atime);

    /* Get the approximate apparent coordinates by interpolation */
    skyfast_getApprox(atime.j2kTT_cy, &approx);

    /* Convert apparent coordinates to topocentric coordinates at the site */
    sky0_appToTirs(&approx.appCirsV,
                   atime.j2kUT1_d,
                   approx.eqEq_rad,
                   &terInterV);
    sky_siteTirsToTopo(&terInterV, approx.distance_au, site, topo);
}



void demo1(void)
/*! Sample code demonstrating the calculation of the position of the Sun using
 *  rectangular coordinates
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
{
    Sky_DeltaTs     deltaTs;        /* Differences between timescales */
    Sky_SiteProp    site;           /* Properties of the site */
    Sky_SiteHorizon topo;           /* Sun position, as seen from the site */
    double          j2kUtc_d;       /* Time, expressed as a count of days */
    char            azStr[24], elStr[24];

    /* Set up the various delta T values */
    sky_initTime(37, 0, &deltaTs);

    /* Set up the fixed site data */
    sky_setSiteLocation(SITE_LATITUDE_deg,
                        SITE_LONGITUDE_deg,
                        SITE_HEIGHT_m,
                        &site);
    sky_setSiteTempPress(SITE_TEMPERATURE_degC,
                         SITE_PRESSURE_hPa,
                         &site);
    sky_setSiteTimeZone(SITE_TIMEZONE_h, &site);

    /* Get the current time in days since 2000-01-01 Greenwich noon */
    j2kUtc_d = sky_unixTimeToJ2kd(time(NULL));

    /* Get sun position */
    sun_nrelTopocentric(j2kUtc_d, &deltaTs, &site, &topo);

    /* Print out time and place */
    printf("Full sun calculation\n");
    skyio_printJ2kd(j2kUtc_d + site.timeZone_d);
    skyio_radToDmsStr(azStr, sizeof(azStr), topo.azimuth_rad, 2);
    skyio_radToDmsStr(elStr, sizeof(elStr), topo.elevation_rad, 2);
    printf(" Sun Azimuth: %s, Elevation: %s\n", azStr, elStr);
}



void demo2(void)
/*! Sample code using the greatly simplified calculation of the position
 *  of the Sun using interpolated coordinates.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
{
    Sky_DeltaTs     deltaTs;        /* Differences between timescales */
    Sky_SiteProp    site;           /* Properties of the site */
    Sky_SiteHorizon topo;           /* Sun position, as seen from the site */
    double          j2kUtc_d;       /* Time, expressed as a count of days */
    char            azStr[24], elStr[24];
    int             i;

    /* Set up the various delta T values. Values are taken from Bulletin A of
     * 16-Jul-2020, using the approximate "DUT1" value for deltaUT_s */
    sky_initTime(37, -0.2, &deltaTs);

    /* Set up the fixed site data */
    sky_setSiteLocation(SITE_LATITUDE_deg,
                        SITE_LONGITUDE_deg,
                        SITE_HEIGHT_m,
                        &site);
    sky_setSiteTempPress(SITE_TEMPERATURE_degC,
                         SITE_PRESSURE_hPa,
                         &site);
    sky_setSiteTimeZone(SITE_TIMEZONE_h, &site);

    /* Get the current time in days since 2000-01-01 Greenwich noon */
    j2kUtc_d = sky_unixTimeToJ2kd(time(NULL));

    /* Set up the interpolation algorithm to calculate Sun positions  */
    skyfast_init(j2kUtc_d, 720, &deltaTs, &sun_nrelApparent);

    printf("Fast sun calculation\n");
    for (i = 0; i < 100; i++) {
        /* Get sun position using interpolation */
        j2kUtc_d = sky_unixTimeToJ2kd(time(NULL));
        sunTopocentricFast(j2kUtc_d, &deltaTs, &site, &topo);

        skyio_printJ2kd(j2kUtc_d + site.timeZone_d);
        skyio_radToDmsStr(azStr, sizeof(azStr), topo.azimuth_rad, 2);
        skyio_radToDmsStr(elStr, sizeof(elStr), topo.elevation_rad, 2);
        printf(" Sun Azimuth: %s, Elevation: %s\n", azStr, elStr);

        sleep(1);       /* Just use a simple delay for this demo. */
    }
}

/*!
 *  \mainpage
 *      This program calls two demo routines to calculate the Sun's position.
 *      The first (called demo1()) calculates it using the NREL SPA algorithm 
 *      (see reference), but implemented in rectangular coordinates.
 *      The second (called demo2()) runs a loop to repeatedly calculate the
 *      position, but using interpolation between two previously calculated
 *      positions.
 *
 *      The interpolation will introduce a very small error in position. For the
 *      value of 720 minutes supplied to routine skyfast_init(), the maximum
 *      error that is introduced is less than 0.075 arcseconds.
 *      Given that the NREL-SPA algorithm itself is only
 *      accurate to approximately 1 arcsecond, this is pretty good.
 *
 *      This demo shows some of the use of the routines in sky.h and sun.h,
 *      and for interpolation, skyfast.h.
 *
 *      To view the source code of these demos, go to main.c and click on the
 *      link "Go to the source code of this file"
 *
 * \par Reference:
 *          Reda, Ibrahim and Andreas, Afshin.
 *          _Solar Position Algorithm for Solar Radiation Applications._
 *          National Renewable Energy Laboratory, publication no.
 *          NREL/TP-560-34302, June 2003, revised January 2008
 */

#ifndef SUN_H
#define SUN_H
/*============================================================================*/
/*!\file
 * \brief sun.h - routines to calculate the Sun's position
 *
 * \author  David Hoadley
 *          Loco2Gen
 *          ABN 22 957 381 638
 *
 * \details
 *      Alternative routines which calculate the sun's position with differing
 *      levels of accuracy. The alternatives are
 *      - a low precision routine, taken from the _Astronomical Almanac_
 *      - a much more detailed, high precision routine, based on the Solar
 *        Position Algorithm, as published by the National Renewable Energy
 *        Laboratory (referred to as the NREL SPA throughout this software)
 *      - an implementation of the NREL SPA suitable for tracking applications,
 *        in which the full precision calculations are performed infrequently,
 *        and interpolations are used to get intermediate positions.
 * 
 *      Rectangular coordinates are used wherever possible, to minimise the
 *      unnecessary recalculation of trigonometric functions.
 *
 * \par References:
 *  The _Astronomical Almanac_, 2007, page C24\n
 *  Reda, Ibrahim and Andreas, Afshin,
 *  _Solar Position Algorithm for Solar Radiation Applications._
 *  National Renewable Energy Laboratory publication no. NREL/TP-560-34302,
 *  June 2003, revised 2008.
 *
 *==============================================================================
 */
#include "astron.h"
#include "sky0.h"
#include "astsite.h"
#include "asttime.h"
#include "vectors3d.h"

/*
 * Global #defines and typedefs
 */


/*
 * Global functions available to be called by other modules
 */
void sun_aaApparentApprox(double n,
                          V3D_Vector *appV,
                          double     *dist_au);

void sun_nrelApp2(double              t_cy,
                  const Sky0_Nut1980 *nut,
                  V3D_Vector *appV,
                  double     *dist_au);
void sun_nrelApparent(double j2kTT_cy, Sky_PosEq *pos);
void sun_nrelTopocentric(double                j2kUtc_d,
                         const Asttime_DeltaTs *deltas,
                         const Astsite_Prop    *site,
                         Astsite_Horizon *topo);

double sun_solarNoon(int                   year,
                     int                   month,
                     int                   day,
                     const Asttime_DeltaTs *deltas,
                     const Astsite_Prop    *site,
                     Astsite_Horizon *topo);
double sun_riseSet(int                   year,
                   int                   month,
                   int                   day,
                   bool                  getSunrise,
                   const Asttime_DeltaTs *deltas,
                   const Astsite_Prop    *site,
                   Astsite_Horizon *topo);

/*
 * Global variables accessible by other modules
 */
/*      (none) */

#endif /*SUN_H*/


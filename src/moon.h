#ifndef MOON_H
#define MOON_H
/*============================================================================*/
/*!\file
 * \brief moon.h - routines to calculate the Moon's position
 *
 * \author  David Hoadley
 *          Loco2Gen
 *          ABN 22 957 381 638
 *
 * \details
 *      An implementation of the SAMPA (Moon Position Algorithm) from the
 *      National Renewable Energy Laboratory
 * 
 *      Rectangular coordinates are used wherever possible, to minimise the
 *      unnecessary recalculation of trigonometric functions.
 * \par Reference:
 *          Reda, I., _Solar Eclipse Monitoring for Solar Energy Applications
 *          Using the Solar and Moon Position Algorithms_. National Renewable
 *          Energy Laboratory Technical Report NREL/TP-3B0-47681, March 2010
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
#ifdef __cplusplus
extern "C" {
#endif

void moon_nrelApp2(double t_cy,
                   const Sky0_Nut1980   *nut,
                   V3D_Vector *appV,
                   double     *dist_au);
void moon_nrelApparent(double j2kTT_cy, Sky_PosEq *pos);
void moon_nrelTopocentric(double                j2kdUtc,
                          const Asttime_DeltaTs *deltas,
                          const Astsite_Prop    *site,
                          Astsite_Horizon *topo);

double moon_riseSet(int year,
                    int month,
                    int day,
                    bool   getMoonrise,
                    const Asttime_DeltaTs *deltas,
                    const Astsite_Prop    *site,
                    Astsite_Horizon *topo);

/*
 * Global variables accessible by other modules
 */
/*      (none)  */

#ifdef __cplusplus
}
#endif

#endif /* MOON_H */


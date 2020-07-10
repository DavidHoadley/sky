#ifndef SKY0_H
#define SKY0_H
/*============================================================================*/
/*!\file
 * \brief sky0.h - astronomical coordinate conversion for NREL Sun Position
 *                  Algorithm
 *
 * \author  David Hoadley
 *          Loco2Gen
 *          ABN 22 957 381 638
 *
 * \details
 *          This is one of three alternative modules: sky0.h / sky0.c,
 *          sky1.h / sky1.c and sky2.h / sky2.c. They contain routines for
 *          transforming astronomical positions from frame to another:
 *          precession, nutation, sidereal time etc. and they reflect changes
 *          in the International Astronomical Union's precession and nutation
 *          theory.
 *          The differences are:
 *          - sky0.h / sky0.c: nutation, obliquity  and sidereal time routines
 *            from the NREL Solar Position Algorithm document. These are based
 *            on the IAU 1980 nutation theory.
 *          - sky1.h / sky1.c: precession, nutation and their associated
 *            rotation matrices, obliquity and sidereal time. These are the
 *            IAU 1980 precession and nutation theory.
 *          - sky2.h / sky2.c: precession, nutation and their associated
 *            rotation matrices, obliquity and sidereal time. These are the
 *            newer IAU 2000 precession and nutation theory.
 * 
 *          This module (sky0.h / sky0.c) contains only those routines necessary
 *          for supporting the NREL Solar Position Algorithm and Moon Position
 *          algorithms. Basically this is a subset of the routines in the
 *          sky1.h / sky1.c module. Precession is omitted, and the nutation
 *          routine uses only the largest 63 terms of the IAU 1980 nutation
 *          algorithm. 
 *
 *==============================================================================
 */
/*
 * Notes
 * Character set: UTF-8. (Non-ASCII characters appear in this file)
 */
#include "vectors3d.h"

/*
 * Global #defines and typedefs
 */
/*!      Nutation angles and obliquity */
typedef struct {
    double  dPsi_rad;       //!< Nutation in longitude (ΔΨ) (radian)
    double  dEps_rad;       //!< Nutation in obliquity (Δε) (radian)
    double  eps0_rad;       //!< Mean obliquity of ecliptic at date (ε0)(radian)
    double  eqEq_rad;       //!< Equation of the Equinoxes (radian)
} Sky0_Nut1980;


/*
 * Global functions available to be called by other modules
 */
#ifdef __cplusplus
extern "C" {
#endif

void sky0_nutationSpa(double t_cy, Sky0_Nut1980 *nut);
void sky0_epsilonSpa(double t_cy, Sky0_Nut1980 *nut);

double sky0_gmSiderealTimeSpa(double du);

void sky0_appToTirs(const V3D_Vector *appV,
                    double           j2kUT1_d,
                    double           eqEq_rad,
                    V3D_Vector  *terInterV);
/*
 * Global variables accessible by other modules
 */

#ifdef __cplusplus
}
#endif

#endif /* SKY0_H */

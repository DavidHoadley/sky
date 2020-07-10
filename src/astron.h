#ifndef ASTRON_H
#define ASTRON_H
/*============================================================================*/
/*!\file
 * \brief astron.h - assorted definitions useful for astronomy
 *
 * \author  David Hoadley
 *          Loco2Gen
 *          ABN 22 957 381 638
 *
 * Description:
 *          Some astronomical constants, and angle conversions that are used in
 *          astronomical software.
 *==============================================================================
 */
#include "general.h"        /* For PI, degToRad(), radToDeg() etc., C standard*/
#include "vectors3d.h"


/*
 * Global #defines and typedefs
 */
/* Useful astronomical constants */
#define MJD_B1900   15019.81352         /* MJD of Besselian std epoch B1900.0 */
#define MJD_B1950   33281.92346         /* MJD of Besselian std epoch B1950.0 */
#define MJD_J2000   51544.5             /* MJD of Fundamental Epoch J2000.0 */
#define TROP_CENT   36524.2198781       /* Length of Tropical Century in days */
#define JUL_CENT    36525.0             /* Length of Julian Century in days */

#define ARCSEC2RAD  (PI / 648000.0)     /* arcseconds to radian */
#define RAD2ARCSEC  (648000.0 / PI)     /* radian to arcseconds */
#define SEC2RAD     (PI / 43200.0)      /* seconds(time) to radian */
#define RAD2SEC     (43200.0 / PI)      /* radian to seconds(time) */
#define HRS2RAD     (PI / 12.0)         /* hours to radians */
#define RAD2HRS     (12.0 / PI)         /* radians to hours */

/*
 * Functions
 */
/*      Convert angle from one unit to another */
#ifdef PREDEF_STANDARD_C_1999
/*          Compiler supports inline functions */
static inline double arcsecToRad(double angle_arcsec) {
                                            return angle_arcsec*ARCSEC2RAD; };
static inline double radToArcsec(double angle_rad) {
                                            return angle_rad * RAD2ARCSEC; };

static inline double secToRad(double angle_s)   { return angle_s * SEC2RAD; };
static inline double radToSec(double angle_rad) { return angle_rad * RAD2SEC; };

static inline double hrsToRad(double angle_h)   { return angle_h * HRS2RAD; };
static inline double radToHrs(double angle_rad) { return angle_rad * RAD2HRS; };

#else
/*          C89/C90 compiler only - no inline functions. Need macros instead */
#define arcsecToRad(angle_arcsec__) ((angle_arcsec__) * ARCSEC2RAD)
#define radToArcsec(angle_rad__)    ((angle_rad__) * RAD2ARCSEC)

#define secToRad(angle_s__)         ((angle_s__) * SEC2RAD)
#define radToSec(angle_rad__)       ((angle_rad__) * RAD2SEC)

#define hrsToRad(angle_h__)         ((angle_h__) * HRS2RAD)
#define radToHrs(angle_rad__)       ((angle_rad__) * RAD2HRS)

#endif

/*! Struct used for holding an object's coordinates in equatorial apparent or
 *  CIRS coordinates. Apparent coordinates are those referred to the true
 *  equator and equinox of the time indicated in field #timestamp_cy. CIRS
 *  coordinates are in the Celestial Intermediate Reference System, referred
 *  to the true equator of time #timestamp_cy and to the Celestial Intermediate
 *  Origin (CIO) instead of the equinox. If the object is in Apparent
 *  coordinates, the Equation of the Equinoxes (#eqEq_rad) field is required as
 *  part of converting to topocentric coordinates. If the object is in CIRS
 *  coordinates, field #eqEq_rad can be ignored. */
typedef struct {
    double      timestamp_cy;   /*!< Time applying to the other figures in
                                     this struct (centuries since J2000.0, TT
                                     timescale) */
    V3D_Vector  appCirsV;       /*!< Direction of object in apparent or CIRS 
                                     coordinates (effectively a unit vector). */
    double      distance_au;    /*!< Distance to object (Astronomical Units) or
                                     0.0 for far distant objects (that is, those
                                     with negligible diurnal parallax) */
    double      eqEq_rad;       /*!< Equation of the Equinoxes (radian). */
} Sky_PosEq;

#endif /*ASTRON_H*/


#ifndef MISSING_MATHS_H
#define MISSING_MATHS_H
/*============================================================================*/
/*!\file
 * \brief missing-maths.h - routines missing from math.h
 *
 * \author  David Hoadley
 *          Loco2Gen
 *          ABN 22 957 381 638
 *
 * \details
 *      Contains routines that are missing from math.h altogether, or missing
 *      from some implementations of math.h. So far, this includes
 *      - sincos()      Calculate sin() and cos() together, more efficiently
 *      - normalize()   A proper modulo function. Unlike fmod(), result is
 *                      always positive. Used to to normalize a cyclic variable
 *                      to a range, e.g. an angle in radian to the range
 *                      [0, 2Pi), or an angle in degrees to the range [0, 360.0)
 * 
 *==============================================================================
 */
#include "general.h"        /* Is this a C99 or later compiler? */

/*
 * Global #defines and typedefs
 */

/*
 * Global functions available to be called by other modules
 */

/* ---------------- The possibly missing sincos() function ----------------- */
#if defined(__APPLE__)
    /* This is the CLANG compiler on an Apple computer (probably a mac). There
       is a sincos() function available, but it is called __sincos() instead.
       Re-direct all calls to sincos() to use __sincos() instead. */
#  if defined (PREDEF_STANDARD_C_1999)
    /*          Compiler supports inline functions */
    static inline void sincos(double angle_rad, double *sinA, double *cosA)
    {
        __sincos(angle_rad, sinA, cosA);
    }
#  else
    /*          C89/C90 compiler - no inline functions. Need macros instead */
    #define sincos(angle_rad_, sinA_, cosA_) __sincos(angle_rad_, sinA_, cosA_)
#  endif

#elif defined(__GNUC__)
    /* This is the GNU C compiler. There is a sincos() function available in
       the GNU C library, but it is only accessible if the following macro is
       defined BEFORE including <math.h>. So do it here. */
#   define _GNU_SOURCE

#else
    /* No sincos() function available. Oh well. Get sine and cosine separately.
       This is less efficient than a true sincos(), but at least it will work.*/
#  if defined (PREDEF_STANDARD_C_1999)
    /*          Compiler supports inline functions */
    static inline void sincos(double angle_rad, double *sinA, double *cosA)
    {
        *sinA = sin(angle_rad);
        *cosA = cos(angle_rad);
    }
#  else
    /*          C89/C90 compiler - no inline functions. Need macros instead */
    #define sincos(angle_rad_, sinA_, cosA_) \
            { *sinA_ = sin(angle_rad_);      \
              *cosA_ = cos(angle_rad_); }
#  endif
#endif

/* -------------- The definitely missing normalize() function -------------- */
#if defined (PREDEF_STANDARD_C_1999)
/*          Compiler supports inline functions */
static inline double normalize(double x, double range)
/*! Normalizes a cyclic double precision floating point variable \a x
    to the interval [0, range), assuming \a range is > 0. (If \a range is
    negative, this will normalize \a to the interval (range, 0], but this is not
    the main use case for this function.)
 \returns The value of x, within the range [0, range)
 \param[in] x       The variable (e.g. an angle in degrees or radian)
 \param[in] range   The range to normalize \a x to (e.g. 2Pi or 360.0). It must
                    be non-zero.

    This function returns the same results as \c fmod() for positive \a x and
    \a range. Where it differs is in its handling of negative values of \a x.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
{
    return x - floor(x / range) * range;
}

#else
 /*          C89/C90 compiler - no inline functions. Need macros instead */
 #define normalize(x__, range__)  ((x__) - floor((x__) / (range__)) * (range__))
#endif


#endif /*MISSING_MATHS_H*/

#ifndef BEFORE_MATH_H
#define BEFORE_MATH_H
/*============================================================================*/
/*!\file
 * \brief before-math.h - sincos(). Needs to be included before including
 *                        the math.h header from the standard C library.
 *
 * \author  David Hoadley <vcrumble@westnet.com.au>
 *          Loco2Gen
 *          ABN 22 957 381 638
 *
 * Description:
 *          The sincos() routine is missing, or possibly missing from the
 *          standard C library math.h. Actually, the GNU C library does have
 *          it, if you give the correct incantation before including math.h.
 *          Also, the Apple Clang library also has it, but under a different
 *          name. So include this header to make it available if possible, or
 *          to define a substitute.
 * 
 *          Because of the way the GNU library works, you must include this file
 *          before you include math.h in any module that you need it. This is
 *          why it is called before-math.h
 *
 *
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


/*
 * Global variables accessible by other modules
 */

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /* BEFORE_MATH_H */


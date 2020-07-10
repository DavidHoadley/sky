#ifndef SUNFAST_H
#define SUNFAST_H
/*============================================================================*/
/*! \file
 * sunfast.h - set up and use interpolation for rapid calculation of a celestial
 *             object's apparent coordinates.
 *
 * Author:  David Hoadley <vcrumble@westnet.com.au>
 *          Loco2Gen
 *          ABN 22 957 381 638
 *
 * Description:
 *          <more detailed description>
 * 
 * \copyright
 * \parblock
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
 * \endparblock
 *
 *==============================================================================
 */
#include "astron.h"
#include "asttime.h"
#include "astsite.h"

/*
 * Global #defines and typedefs
 */


#ifdef __cplusplus
extern "C" {
#endif
/*
 * Global functions available to be called by other modules
 */
void skyfast_init(double mjdUtc,
                  int    fullRecalcInterval_mins,
                  const Asttime_DeltaTs *deltas,
                  void (*getApparent)(double j2kTT_cy, Sky_PosEq *pos)
                  );
void skyfast_backgroundUpdate(void);
void skyfast_getApprox(double t_cy,
                         Sky_PosEq *approx);

/*
 * Global variables accessible by other modules
 */

#ifdef __cplusplus
}
#endif

#endif /* SUNFAST_H */


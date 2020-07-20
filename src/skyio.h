#ifndef RDWRITE_H
#define RDWRITE_H
/*============================================================================*/
/*!\file
 * \brief rdwrite.h - output and formatting routines and a read routine
 *
 * \author  David Hoadley
 *          Loco2Gen
 *          ABN 22 957 381 638
 *
 * \details
 *          Routines for reading and writing out assorted astronomical things,
 *          such as:
 *          - formatting a radian angle into Hours Minutes Seconds form,
 *            something only an astronomer would think of doing.
 *          - formatting a radian angle into Degrees Minutes Seconds form, and
 *            to do it correctly. Quite a few people need this one.
 *          - reading an angle from a string
 *          - write out a Modified Julian Date as a calendar date and time.
 *          - write out the contents of matrices and vectors.
 * 
 *          These routines were mainly developed for debugging.
 * 
 *==============================================================================
 */
#include "vectors3d.h"

/*
 * Global #defines and typedefs
 */
#define NO_ANGLE                (-1)
#define INVALID_ANGLE           (-2)

/*
 * Global functions available to be called by other modules
 */

#ifdef __cplusplus
extern "C" {
#endif


char *fmtDms(char destSt[],
             int destStrLen,
             double angle_rad,
             int decimals);
char *fmtHms(char destSt[],
             int destStrLen,
             double angle_rad,
             int decimals);
double readSexagesimal(const char angleStr[],
                       int strLen,
                       const char **endPtr,
                       int *error);

void printMatrix(const V3D_Matrix *m);
void printVector(const V3D_Vector *v);
void printJ2kd(double j2kd);
#ifdef INCLUDE_MJD_ROUTINES
void printMjd(double mjd);
#endif

#ifdef __cplusplus
}
#endif

/*
 * Global variables accessible by other modules
 */


#endif /* RDWRITE_H */


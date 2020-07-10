#ifndef VECTORS3D_H
#define VECTORS3D_H
/*============================================================================*/
/*!\file
 * \brief vectors3d.h - Three dimensional geometry, vectors and matrices
 *
 * \author  David Hoadley
 *
 * \details
 *          Data types and operations for handling three-dimensional positions
 *          and performing operations upon them, such as adding or subtracting
 *          vectors, coordinate rotation, conversion to and from polar coords.
 *
 *==============================================================================
 */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Global #defines and typedefs
 */
/*! 3x3 matrix. See the note \ref page-why-struct-array (at the end of file 
 *  vectors3.c) for the reason that this has been implemented in this fashion */
typedef struct {
    double  a[3][3];
} V3D_Matrix;

/*! 3x1 vector.  See the note \ref page-why-struct-array (at the end of file 
 *  vectors3.c) for the reason that this has been implemented in this fashion */
typedef struct {
    double  a[3];
} V3D_Vector;

/*! Enumeration to be used by the v3d_createRotationMatrix() function */
typedef enum /* V3D_AxisNames_tag */ {
    Xaxis,
    Yaxis,
    Zaxis
} V3D_AxisNames;


/*
 * Global functions available to be called by other modules
 */
/*      Most of the functions below return vector or matrix results in their
        first argument, but also return a pointer to that same variable as the
        function result. */

/*          Vector arithmetic */
V3D_Vector *v3d_addV(V3D_Vector *destV,
                     const V3D_Vector *srcV1,
                     const V3D_Vector *srcV2);
V3D_Vector *v3d_addToV(V3D_Vector *modV1, const V3D_Vector *srcV2);
V3D_Vector *v3d_subtractV(V3D_Vector *destV,
                          const V3D_Vector *srcV1,
                          const V3D_Vector *srcV2);
V3D_Vector *v3d_subFromV(V3D_Vector *modV1, const V3D_Vector *srcV2);
double      v3d_dotProductV(const V3D_Vector *srcV1, const V3D_Vector *srcV2);
V3D_Vector *v3d_crossProductV(V3D_Vector *destV,
                              const V3D_Vector *srcV1,
                              const V3D_Vector *srcV2);
/*          Two special versions of v3d_addToV() for unit vectors */
V3D_Vector *v3d_addToUV(V3D_Vector *modV1, const V3D_Vector *srcV2);
V3D_Vector *v3d_addToUVfast(V3D_Vector *modV1, const V3D_Vector *srcV2);

double v3d_magVSq(const V3D_Vector *srcV);
double v3d_magV(const V3D_Vector *srcV);

/*          Polar <==> Rectangular coordinate conversions */
V3D_Vector *v3d_polarToRect(V3D_Vector *destV,
                            double alpha_rad,
                            double delta_rad);
void v3d_rectToPolar(double *alpha_rad, 
                     double *delta_rad,
                     const V3D_Vector *srcV);
#if 0
/*              deprecated older versions */
void v3d_polar2Rect(double alpha_rad, double delta_rad, V3D_Vector *R);
void v3d_rect2Polar(const V3D_Vector *R, 
                    bool  alphaNonNegative,
                    double *alpha_rad, 
                    double *delta_rad);
#endif

/*          Matrices for coordinate rotation, and matrix operations */
V3D_Matrix *v3d_createRotationMatrix(V3D_Matrix *destM,
                                     V3D_AxisNames axis, 
                                     double        angle_rad);
V3D_Vector *v3d_multMxV(V3D_Vector *destV,
                        const V3D_Matrix *srcM,
                        const V3D_Vector *srcV);
V3D_Vector *v3d_multMtransxV(V3D_Vector *destV,
                             const V3D_Matrix *srcM,
                             const V3D_Vector *srcV);
V3D_Matrix *v3d_multMxM(V3D_Matrix *destM,
                        const V3D_Matrix *srcM1,
                        const V3D_Matrix *srcM2);


#ifdef __cplusplus
}
#endif

#endif /*VECTORS3D_H*/


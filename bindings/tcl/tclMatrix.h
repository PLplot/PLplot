/* $Id$
 * $Log$
 * Revision 1.3  1994/06/24 20:37:55  mjl
 * Changed name of struct to tclMatrix to avoid conflicts with C++ Matrix
 * classes.  Put in ifdef-ed extern "C" linkage statements for C++.
 *
 * Revision 1.2  1994/06/16  21:57:11  mjl
 * Added matrix operator name to command data struct.
 *
 * Revision 1.1  1994/06/16  19:49:05  mjl
 * Header file for new Tcl matrix command.
 */

/*----------------------------------------------------------------------*\
 *
 * tclMatrix.h --
 *
 *	Contains declarations for Tcl "Matrix" command.
 *	C functions that need access to the matrix data will need
 *	to include this file.
 *
 * Maurice LeBrun
 * IFS, University of Texas at Austin
 * 13-Jun-1994
\*----------------------------------------------------------------------*/

#ifndef __TCLMATRIX_H__
#define __TCLMATRIX_H__

#ifdef DOUBLE
typedef double Mat_float;
#else
typedef float  Mat_float;
#endif

typedef int    Mat_int;

/* Arrays are column dominant (normal C ordering) */
/* Array elements are stored contiguously */
/* Require dimension <= 3, floats for simplicity */

#define MAX_ARRAY_DIM 3
#define TYPE_FLOAT 1

/* Macros used in index calculations */

#define I3D(i,j,k)	k + matPtr->n[2] * (I2D(i,j))
#define I2D(i,j)	j + matPtr->n[1] * (I1D(i))
#define I1D(i)		i

#define M3D(i,j,k)	matPtr->fdata[I3D(i,j,k)]
#define M2D(i,j)	matPtr->fdata[I2D(i,j)]
#define M1D(i)		matPtr->fdata[I1D(i)]

/* Matrix operator data */

typedef struct {
    int type;			/* Data type */
				/* For now only float/double are supported */
    int len;			/* Total length of array */
    int dim;			/* Number of dimensions */
    int n[MAX_ARRAY_DIM];	/* Holds array length in each dimension */
    char name[20];		/* Matrix operator name */

    Mat_float *fdata;		/* Floating point data */
    Mat_int   *idata;		/* Integer data */
} tclMatrix;

/* Function prototypes */

#ifdef __cplusplus
extern "C" {
#endif

#include <tcl.h>

/* This procedure is invoked to process the "matrix" Tcl command. */

int
Tcl_MatrixCmd(ClientData clientData, Tcl_Interp *interp,
	      int argc, char **argv);

/* Returns a pointer to the specified matrix operator's data. */

tclMatrix *
Tcl_GetMatrixPtr(Tcl_Interp *interp, char *matName);

#ifdef __cplusplus
}
#endif

#endif	/* __TCLMATRIX_H__ */

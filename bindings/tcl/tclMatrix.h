/* -*-C++-*-
 * $Id$
 * $Log$
 * Revision 1.10  1995/03/21 19:40:32  mjl
 * Added interpreter to tclMatrix struct for more robust resource recovery.
 *
 * Revision 1.9  1994/08/11  01:43:07  furnish
 * Implement redim methods for the C++ shadow class.  Somebody still
 * needs to do it in the Tcl code :-).
 *
 * Revision 1.8  1994/08/05  17:45:31  furnish
 * More on the C++ shadow class.
 *
 * Revision 1.7  1994/06/30  18:38:49  mjl
 * Changed typedef for Mat_int back to an int, and eliminated M?D macros in
 * favor of directly addressing the array.
 *
 * Revision 1.6  1994/06/30  05:45:21  furnish
 * Cobbled together an extension facility which allows a user to define
 * their own subcommands for tclMatricies.  The idea is that you can use
 * this to implement your own matrix processing commands entirely on the
 * compiled side.  fft's, matrix multiplication, inversion, etc.
 *
 * Revision 1.5  1994/06/26  05:16:15  furnish
 * Implemented C++ wrapper class for tclMatrix, enabling easy use of a
 * tclMatrix from compiled extension commands.  Needs to have sibling
 * classes created by someone who cares.
 *
 * Revision 1.4  1994/06/25  20:35:49  mjl
 * Changed typedef for Mat_int to long.  Maybe I should have a Mat_long
 * instead?  Added put/get function handlers to matrix struct (determined
 * when created, dependent on type).
 *
 * Revision 1.3  1994/06/24  20:37:55  mjl
 * Changed name of struct to tclMatrix to avoid conflicts with C++ Matrix
 * classes.  Put in ifdef-ed extern "C" linkage statements for C++.
 *
 * Revision 1.2  1994/06/16  21:57:11  mjl
 * Added matrix operator name to command data struct.
 *
 * Revision 1.1  1994/06/16  19:49:05  mjl
 * Header file for new Tcl matrix command.
 */

/*
    tclMatrix.h

    Copyright 1994, 1995
    Maurice LeBrun			mjl@dino.ph.utexas.edu
    Institute for Fusion Studies	University of Texas at Austin

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	
    Contains declarations for Tcl "Matrix" command.
    C functions that need access to the matrix data will need
    to include this file.
*/

#ifndef __TCLMATRIX_H__
#define __TCLMATRIX_H__

#include <tcl.h>

#ifdef DOUBLE
typedef double Mat_float;
#else
typedef float  Mat_float;
#endif

#if defined(MSDOS)
typedef long  Mat_int;
#else
typedef int   Mat_int;
#endif

enum { TYPE_FLOAT, TYPE_INT };

/* Arrays are column dominant (normal C ordering) */
/* Array elements are stored contiguously */
/* Require dimension <= 3 for simplicity */

#define MAX_ARRAY_DIM 3

/* Useful macros for index calculations */

#define I3D(i,j,k)	k + matPtr->n[2] * (I2D(i,j))
#define I2D(i,j)	j + matPtr->n[1] * (I1D(i))
#define I1D(i)		i

/* Matrix operator data */

typedef struct {
    int type;			/* Data type */
    int len;			/* Total length of array */
    int dim;			/* Number of dimensions */
    int n[MAX_ARRAY_DIM];	/* Holds array length in each dimension */
    int tracing;		/* Set if not persistent */

    char *name;			/* Matrix operator name, malloc'ed */

    Mat_float *fdata;		/* Floating point data, malloc'ed */
    Mat_int   *idata;		/* Integer data, malloc'ed */

    Tcl_Interp *interp;		/* Interpreter where command is installed */

/* These do the put/get operations for each supported type */

    void (*put) (ClientData clientData, int index, char *string);
    void (*get) (ClientData clientData, int index, char *string);

} tclMatrix;

/* Function prototypes */

#ifdef __cplusplus
/*---------------------------------------------------------------------------//
// Since C++ does not generally have a per-platform ABI the way C
// does, we stick to a totally inline class declaration and
// definition.  That way you don't have to keep a separate version of
// libplplot*.a for each compiler you'd like to use.

// Start by setting up some important macros.

// HP C++ has exceptions, but most other compilers don't.  Will embellish
// this as necessary.
*/

#ifdef throw
#define TCL_NO_UNDEF
#endif

#ifndef throw
#ifdef __hpux
#if defined(__GCC__) || defined(__lucid) || defined(__Centerline)
#define NO_XCPT
#endif
#else
#define NO_XCPT
#endif

#ifdef NO_XCPT
#define try
#define throw(a) \
{ cerr << "THROW: " << #a << " from " << __FILE__ \
       << " line " << __LINE__ << endl << flush; abort(); }
#define catch(a) if (0)
#define Throw
#else
#define Throw throw
#endif
#endif

#define tMat_Assert(a,b) if (!(a)) \
{ cerr << "Assertion " << #a << " failed in " << __FILE__ \
       << " at line " << __LINE__ << endl << flush; \
  throw(b); }

/*---------------------------------------------------------------------------//
// class TclMatFloat

// This class provides a convenient way to access the data of a
// tclMatrix from within compiled code.  Someone should make clones of
// this class for the other tclMatrix supported data types.
//---------------------------------------------------------------------------*/

class TclMatFloat {
    tclMatrix *matPtr;
  public:
    TclMatFloat( tclMatrix *ptm )
	: matPtr(ptm)
    {
	tMat_Assert( matPtr->type == TYPE_FLOAT, "Type mismatch" );
    }

    int Dimensions() { return matPtr->dim; }

    void redim( int nx )
    {
	free( matPtr->fdata );
	matPtr->dim = 1;
	matPtr->n[0] = nx;
	matPtr->len = nx;
	matPtr->fdata = (float *) malloc( matPtr->len * sizeof(float) );
    }

    void redim( int nx, int ny )
    {
	free( matPtr->fdata );
	matPtr->dim = 2;
	matPtr->n[0] = nx;
	matPtr->n[1] = ny;
	matPtr->len = nx * ny;
	matPtr->fdata = (float *) malloc( matPtr->len * sizeof(float) );
    }

    void redim( int nx, int ny, int nz )
    {
	free( matPtr->fdata );
	matPtr->dim = 3;
	matPtr->n[0] = nx;
	matPtr->n[1] = ny;
	matPtr->n[2] = nz;
	matPtr->len = nx * ny * nz;
	matPtr->fdata = (float *) malloc( matPtr->len * sizeof(float) );
    }

    Mat_float& operator()( int i )
    {
	tMat_Assert( matPtr->dim == 1, "Wrong number of indicies." );
	tMat_Assert( i >= 0 && i < matPtr->n[0],
		     "Out of bounds reference" );

	return matPtr->fdata[i];
    }

    Mat_float& operator()( int i, int j )
    {
	tMat_Assert( matPtr->dim == 2, "Wrong number of indicies." );
	tMat_Assert( i >= 0 && i < matPtr->n[0] &&
		    j >= 0 && j < matPtr->n[1],
		    "Out of bounds reference" );
		
	return matPtr->fdata[I2D(i,j)];
    }

    Mat_float& operator()( int i, int j, int k )
    {
	tMat_Assert( matPtr->dim = 3, "Wrong number of indicies." );
	tMat_Assert( i >= 0 && i < matPtr->n[0] &&
		    j >= 0 && j < matPtr->n[1] &&
		    k >= 0 && k < matPtr->n[2],
		    "Out of bounds reference" );

	return matPtr->fdata[I3D(i,j,k)];
    }
};

#ifndef TCL_NO_UNDEF

#ifdef NO_XCPT
#undef NO_XCPT
#undef try
#undef throw
#undef Throw
#undef catch
#endif

#endif

#undef tMat_Assert

extern "C" {
/*---------------------------------------------------------------------------*/
#endif

#include <tcl.h>

/* This procedure is invoked to process the "matrix" Tcl command. */

int
Tcl_MatrixCmd(ClientData clientData, Tcl_Interp *interp,
	      int argc, char **argv);

/* Returns a pointer to the specified matrix operator's data. */

tclMatrix *
Tcl_GetMatrixPtr(Tcl_Interp *interp, char *matName);

/* Some stuff for handling extension subcommands. */

typedef int (*tclMatrixXtnsnProc) ( tclMatrix *pm, Tcl_Interp *interp,
				    int argc, char *argv[] );

typedef struct tclMatrixXtnsnDescr {
    char *cmd;
    tclMatrixXtnsnProc cmdproc;
    struct tclMatrixXtnsnDescr *next;
} tclMatrixXtnsnDescr;

int Tcl_MatrixInstallXtnsn( char *cmd, tclMatrixXtnsnProc proc );

#ifdef __cplusplus
}
#endif

#endif	/* __TCLMATRIX_H__ */

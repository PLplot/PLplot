/*
Copyright (C) 2010  Alan W. Irwin
This file is part of PLplot.

PLplot is free software; you can redistribute it and/or modify
it under the terms of the GNU Library General Public License as published by
the Free Software Foundation; version 2 of the License.

PLplot is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Library General Public License for more details.

You should have received a copy of the GNU Library General Public License
along with the file PLplot; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

/*
A SWIG interface to PLplot for Octave. This wrapper does the following:

   1) it strictly provides the C-API with the usual change of not
      requiring lengths for arrays,

   2) it attempts to provide the entire PLplot API (excluding for now
   any special arguments that require special callbacks).
   
   3) it works only with the double-precision version of the
      PLplot library.

This is known to work with swig-1.3.36.

*/
%module plplot_octave

%{
#include "plplotP.h"
%}

/* type definitions */
typedef double PLFLT;
typedef int PLINT;
typedef unsigned int PLUNICODE;
typedef PLINT PLBOOL;

%{
// I hate global variables but this is the best way I can think of
// to manage consistency checking among function arguments.
static PLINT Alen = 0;
static PLINT Xlen = 0, Ylen = 0;
%}

%{
// Convenience functions copied from matwrap-based approach (currently
// stored in bindings/octave/matwrap/wrap_octave.pl) to take care of the
// tricky scalar case and also adopted so that the resulting
// swig-generated source code will look similar to the matwrap-generated
// source code.

inline int max(int a, int b) { return a >= b ? a : b; }
inline int min(int a, int b) { return a >= b ? a : b; }

//
// Function to get the total length (rows*columns) of an octave object of
// arbitrary type.
// Arguments:
// 1) The octave object.
//
// If the object is a scalar, the array length is 1.
//
static int
_arraylen(const octave_value &o_obj)
{
  return max(o_obj.rows(),1) * max(o_obj.columns(),1); // Return the size.
                                // max is necessary because sometimes
                                // rows() or columns() return -1 or 0 for
                                // scalars.
}

//
// Function to get the number of dimensions of an object.
//
static int
_n_dims(const octave_value &o_obj)
{
  if (max(o_obj.columns(),1) > 1)
    return 2;
                                // max is necessary because sometimes
                                // rows() or columns() return -1 or 0 for
                                // scalars.
  else if (max(o_obj.rows(),1) > 1)
    return 1;
  else
    return 0;
}

//
// Return the n'th dimension of an object.  Dimension 0 is the 1st dimension.
//
static inline int
_dim(const octave_value &o_obj, int dim_idx)
{
  if (dim_idx == 0)
    return max(o_obj.rows(), 0);
                                // max is necessary because sometimes
                                // rows() or columns() return -1 or 0 for
                                // scalars.
  else if (dim_idx == 1)
    return max(o_obj.columns(), 0);
  else
    return 1;
}
 
//
// The following function converts an array of doubles into some other
// numeric type.  Arguments:
// 1) Where to store the result.  The type is determined from the type of
//    this pointer.
// 2) A vector of doubles to convert.
// 3) The number of doubles.
//
template <class FLOAT>
static inline void
_cvt_double_to(FLOAT *out_arr, double *in_arr, unsigned n_el)
{
  while (n_el-- > 0)
    *out_arr++ = (FLOAT)(*in_arr++);
}

template void _cvt_double_to(int *, double *, unsigned);
template void _cvt_double_to(unsigned *, double *, unsigned);
template void _cvt_double_to(long *, double *, unsigned);
template void _cvt_double_to(unsigned long *, double *, unsigned);
template void _cvt_double_to(short *, double *, unsigned);
template void _cvt_double_to(unsigned short *, double *, unsigned);
template void _cvt_double_to(float *, double *, unsigned);
                                // Instantiate our templates.  Octave uses
                                // manual template instantiation.

//
// Convert an array of some other type into an array of doubles.  Arguments:
// 1) The array of objects of other type.
// 2) The output array of doubles.
// 3) The number of elements to convert.
//
template <class FLOAT>
static inline void
_cvt_to_double(FLOAT *arr, double *d_arr, unsigned n_el)
{
  while (n_el-- > 0)
    *d_arr++ = double(*arr++);
}

template void _cvt_to_double(int *, double *, unsigned);
template void _cvt_to_double(unsigned *, double *, unsigned);
template void _cvt_to_double(long *, double *, unsigned);
template void _cvt_to_double(unsigned long *, double *, unsigned);
template void _cvt_to_double(short *, double *, unsigned);
template void _cvt_to_double(unsigned short *, double *, unsigned);
template void _cvt_to_double(float *, double *, unsigned);
                                // Instantiate our templates.  Octave uses
                                // manual template instantiation.
%}

/* The following typemaps take care of marshaling values into and out of PLplot functions. The
Array rules are trickly because of the need for length checking. These rules manage
some global variables (above) to handle consistency checking amoung parameters. 

Naming rules:
	Array 		(sets Alen to dim[0])
	ArrayCk 	(tests that dim[0] == Alen)
	ArrayX 		(sets Xlen to dim[0]
	ArrayCkX 	(tests dim[0] == Xlen)
	ArrayY 		(sets Ylen to dim[1])
	ArrayCkY 	(tests dim[1] == Ylen)
	Matrix 		(sets Xlen to dim[0], Ylen to dim[1])
	MatrixCk 	(test Xlen == dim[0] && Ylen == dim[1])
*/

/* typemaps */
%include <typemaps.i>

/**********************************************************************************
			 PLINT arrays
**********************************************************************************/

// With preceding count and remember size to check others
%typemap(in) (PLINT n, PLINT *Array) (Matrix temp) {
  if ( _n_dims($input) > 1 )
      { error("argument must be a scalar or vector"); SWIG_fail; }
  $1 = Alen = (PLINT)(_dim($input, 0));
  $2 = new PLINT[Alen];
  temp = $input.matrix_value();
  _cvt_double_to($2, &temp(0,0), Alen);
}
%typemap(freearg) (PLINT n, PLINT *Array) {delete [] $2;}

// With trailing count and check consistency with previous
%typemap(in) (PLINT *ArrayCk, PLINT n) (Matrix temp) {
  if ( _n_dims($input) > 1 )
      { error("argument must be a scalar or vector"); SWIG_fail; }
  if ( _dim($input, 0) != Alen )
      { error("argument vectors must be same length"); SWIG_fail; }
  temp = $input.matrix_value();
  $1 = new PLINT[Alen];
  _cvt_double_to($1, &temp(0,0), Alen);
  $2 = Alen;
}
%typemap(freearg) (PLINT *ArrayCk, PLINT n) {delete [] $1;}

// No count but check consistency with previous
%typemap(in) PLINT *ArrayCk (Matrix temp) {
  if ( _n_dims($input) > 1 )
      { error("argument must be a scalar or vector"); SWIG_fail; }
  if ( _dim($input, 0) != Alen )
      { error("argument vectors must be same length"); SWIG_fail; }
  temp = $input.matrix_value();
  $1 = new PLINT[Alen];
  _cvt_double_to($1, &temp(0,0), Alen);
}
%typemap(freearg) PLINT *ArrayCk {delete [] $1;}

// No count but remember size to check others
%typemap(in) PLINT *Array (Matrix temp) {
  if ( _n_dims($input) > 1 )
      { error("argument must be a scalar or vector"); SWIG_fail; }
  Alen = (PLINT)(_dim($input, 0));
  temp = $input.matrix_value();
  $1 = new PLINT[Alen];
  _cvt_double_to($1, &temp(0,0), Alen);
}
%typemap(freearg) (PLINT *Array) {delete [] $1;}

// No count but check consistency with previous
// Variation to allow argument to be one shorter than others.
%typemap(in) PLINT *ArrayCkMinus1 (Matrix temp) {
  if ( _n_dims($input) > 1 )
      { error("argument must be a scalar or vector"); SWIG_fail; }
  if ( ! (_dim($input, 0) == Alen || _dim($input, 0) == Alen-1) )
      { error("argument vector must be same length or one less"); SWIG_fail; }
  temp = $input.matrix_value();
  $1 = new PLINT[Alen];
  _cvt_double_to($1, &temp(0,0), Alen);
}
%typemap(freearg) PLINT *ArrayCkMinus1 {delete [] $1;}

// For octave there is no provision for dropping the last argument
// so this typemap is identical to the previous one.
%typemap(in) PLINT *ArrayCkMinus1Null (Matrix temp) {
  if ( _n_dims($input) > 1 )
      { error("argument must be a scalar or vector"); SWIG_fail; }
  if ( ! (_dim($input, 0) == Alen || _dim($input, 0) == Alen-1) )
      { error("argument vector must be same length or one less"); SWIG_fail; }
  temp = $input.matrix_value();
  $1 = new PLINT[Alen];
  _cvt_double_to($1, &temp(0,0), Alen);
}
%typemap(freearg) PLINT *ArrayCkMinus1Null {delete [] $1;}


/******************************************************************************
				 PLFLT Arrays 
******************************************************************************/

// With preceding count and remember size to check others
%typemap(in) (PLINT n, PLFLT *Array) (Matrix temp) {
  if ( _n_dims($input) > 1 )
      { error("argument must be a scalar or vector"); SWIG_fail; }
  $1 = Alen = (PLINT)(_dim($input, 0));
  temp = $input.matrix_value();
  $2 = &temp(0,0);
}
%typemap(freearg) (PLINT n, PLFLT *Array) {}

// With trailing count and check consistency with previous
%typemap(in) (PLFLT *ArrayCk, PLINT n) (Matrix temp) {
  if ( _n_dims($input) > 1 )
      { error("argument must be a scalar or vector"); SWIG_fail; }
  if ( _dim($input, 0) != Alen )
      { error("argument vectors must be same length"); SWIG_fail; }
  temp = $input.matrix_value();
  $1 = &temp(0,0);
  $2 = (PLINT)(_dim($input, 0));
}
%typemap(freearg) (PLFLT *ArrayCk, PLINT n) {}

// No count but check consistency with previous
%typemap(in) PLFLT *ArrayCk (Matrix temp) {
  if ( _n_dims($input) > 1 )
      { error("argument must be a scalar or vector"); SWIG_fail; }
  if ( _dim($input, 0) != Alen )
      { error("argument vectors must be same length"); SWIG_fail; }
  temp = $input.matrix_value();
  $1 = &temp(0,0);
}
%typemap(freearg) PLFLT *ArrayCk {}

// No count but remember size to check others
%typemap(in) PLFLT *Array (Matrix temp) {
  if ( _n_dims($input) > 1 )
      { error("argument must be a scalar or vector"); SWIG_fail; }
  Alen = (PLINT)(_dim($input, 0));
  temp = $input.matrix_value();
  $1 = &temp(0,0);
}
%typemap(freearg) (PLFLT *Array) {}

// With trailing count but remember size to check others
%typemap(in) (PLFLT *Array, PLINT n) (Matrix temp) {
  if ( _n_dims($input) > 1 )
      { error("argument must be a scalar or vector"); SWIG_fail; }
  temp = $input.matrix_value();
  $1 = &temp(0,0);
  $2 = Alen = (PLINT)(_dim($input, 0));
}
%typemap(freearg) (PLFLT *Array, PLINT n) {}

// No X count but check consistency with previous
%typemap(in) PLFLT *ArrayCkX {}
%typemap(freearg) PLFLT *ArrayCkX {}

// No Y count but check consistency with previous
%typemap(in) PLFLT *ArrayCkY {}
%typemap(freearg) PLFLT *ArrayCkY {}

// With trailing X count but remember X size to check others
%typemap(in) (PLFLT *ArrayX, PLINT nx) {}
%typemap(freearg) (PLFLT *ArrayX, PLINT nx) {}

// No X count but remember X size to check others
%typemap(in) PLFLT *ArrayX {}
%typemap(freearg) PLFLT *ArrayX {}

// With trailing Y count but remember Y size to check others
%typemap(in) (PLFLT *ArrayY, PLINT ny) {}
%typemap(freearg) (PLFLT *ArrayY, PLINT ny) {}

// No Y count but remember Y size to check others
%typemap(in) PLFLT *ArrayY {}
%typemap(freearg) (PLFLT *ArrayY) {}

/* 2D array with trailing dimensions, check consistency with previous */
%typemap(in) (PLFLT **MatrixCk, PLINT nx, PLINT ny) (int ii) {}
%typemap(freearg) (PLFLT **MatrixCk, PLINT nx, PLINT ny) {}


/* 2D array with trailing dimensions, set the X, Y size for later checking */
%typemap(in) (PLFLT **Matrix, PLINT nx, PLINT ny) (int ii) {}
%typemap(freearg) (PLFLT **Matrix, PLINT nx, PLINT ny) {}


/* 2D array with no dimensions, set the X, Y size for later checking */
%typemap(in) PLFLT **Matrix (int ii) {}
%typemap(freearg) PLFLT **Matrix {}


/* 2D array, check for consistency */
%typemap(in) PLFLT **MatrixCk (int ii) {}
%typemap(freearg) PLFLT **MatrixCk {}


/* Set Y length for later consistency checking, with trailing count */
/* and 2D array, check for consistency input / output version */
%typemap(in) (PLFLT *ArrayY, PLINT ny, PLFLT **OutMatrixCk) {}
%typemap(argout) (PLFLT *ArrayY, PLINT ny, PLFLT **OutMatrixCk) {}
%typemap(freearg) (PLFLT *ArrayY, PLINT ny, PLFLT **OutMatrixCk) {}

//-----------------------------------------------------------------------------
//				 String returning functions
//-----------------------------------------------------------------------------

// This currently just used for plgdev, plgfnam, and plgver which
// apparently have a limit of 80 bytes.  N.B. This works, but it
// copies what was done by Rafael for matwrap with no deep knowledge
// of octave and uses swig internals (note the use of retval1 as the
// return value).  Thus, it needs to be redone by somebody who knows
// what they are doing.
%typemap(in, numinputs=0) char *OUTPUT (octave_value_list retval){
  retval(0) = octave_value(charMatrix(80, 1), true);
  $1 = (char *)retval(0).char_matrix_value().data();
}
%typemap(argout) char *OUTPUT {
  return retval1;
 } 

typedef PLINT (*defined_func)(PLFLT, PLFLT);
typedef void (*fill_func)(PLINT, PLFLT*, PLFLT*);
typedef void (*pltr_func)(PLFLT, PLFLT, PLFLT *, PLFLT*, PLPointer);
typedef void (*mapform_func)(PLINT, PLFLT*, PLFLT*);
typedef PLFLT (*f2eval_func)(PLINT, PLINT, PLPointer);
typedef void (*label_func)(PLINT, PLFLT, char*, PLINT, PLPointer);

%{
typedef PLINT (*defined_func)(PLFLT, PLFLT);
typedef void (*fill_func)(PLINT, PLFLT*, PLFLT*);
typedef void (*pltr_func)(PLFLT, PLFLT, PLFLT *, PLFLT*, PLPointer);
typedef void (*mapform_func)(PLINT, PLFLT *, PLFLT*);
typedef PLFLT (*f2eval_func)(PLINT, PLINT, PLPointer);
typedef void (*label_func)(PLINT, PLFLT, char*, PLINT, PLPointer);
%}
// For historical reasons our octave bindings use the name plSetOpt for
// the PLplot function, plsetopt, and use the plsetopt name for a different
// purpose (plsetopt.m).  We implement that here using the rename directive.
%rename(plSetOpt) plsetopt;
/* swig compatible PLplot API definitions from here on. */
%include plplotcapi.i

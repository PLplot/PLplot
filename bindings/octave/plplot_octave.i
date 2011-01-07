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

// The following typemaps take care of marshaling values into and out
// of PLplot functions. The Array rules are trickly because of the
// need for length checking. These rules manage some global variables
// (above) to handle consistency checking amoung parameters. 

// Naming rules:
//      Array 	        (sets Alen to dim[0])
//      ArrayCk         (tests that dim[0] == Alen)
//      ArrayX 	        (sets Xlen to dim[0]
//      ArrayCkX        (tests dim[0] == Xlen)
//      ArrayY 	        (sets Ylen to dim[1])
//      ArrayCkY        (tests dim[1] == Ylen)
//      Matrix 	        (sets Xlen to dim[0], Ylen to dim[1])
//      MatrixCk        (test Xlen == dim[0] && Ylen == dim[1])

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

// The Matrix typemaps below here are a special form (with **Matrix
// replaced by *Matrix) that is only suitable for special octave
// wrappers that are defined later in the file while the usual
// PLplot functions that use **Matrix are %ignored.

// No X count but check consistency with previous
%typemap(in) PLFLT *ArrayCkX (Matrix temp) {
  if ( _n_dims($input) > 1 )
      { error("argument must be a scalar or vector"); SWIG_fail; }
  if ( _dim($input, 0) != Xlen )
      { error("argument vectors must be same length"); SWIG_fail; }
  temp = $input.matrix_value();
  $1 = &temp(0,0);
}
%typemap(freearg) PLFLT *ArrayCkX {}

// No Y count but check consistency with previous
%typemap(in) PLFLT *ArrayCkY (Matrix temp) {
  if ( _n_dims($input) > 1 )
      { error("argument must be a scalar or vector"); SWIG_fail; }
  if ( _dim($input, 0) != Ylen )
      { error("argument vectors must be same length"); SWIG_fail; }
  temp = $input.matrix_value();
  $1 = &temp(0,0);
}
%typemap(freearg) PLFLT *ArrayCkY {}

// With trailing X count but remember X size to check others
%typemap(in) (PLFLT *ArrayX, PLINT nx) (Matrix temp) {
  if ( _n_dims($input) > 1 )
      { error("argument must be a scalar or vector"); SWIG_fail; }
  temp = $input.matrix_value();
  $1 = &temp(0,0);
  $2 = Xlen = (PLINT)(_dim($input, 0));
}
%typemap(freearg) (PLFLT *ArrayX, PLINT nx) {}

// No X count but remember X size to check others
%typemap(in) PLFLT *ArrayX (Matrix temp) {
  if ( _n_dims($input) > 1 )
      { error("argument must be a scalar or vector"); SWIG_fail; }
  temp = $input.matrix_value();
  $1 = &temp(0,0);
  Xlen = (PLINT)(_dim($input, 0));
}
%typemap(freearg) PLFLT *ArrayX {}

// With trailing Y count but remember Y size to check others
%typemap(in) (PLFLT *ArrayY, PLINT ny) (Matrix temp) {
  if ( _n_dims($input) > 1 )
      { error("argument must be a scalar or vector"); SWIG_fail; }
  temp = $input.matrix_value();
  $1 = &temp(0,0);
  $2 = Ylen = (PLINT)(_dim($input, 0));
}
%typemap(freearg) (PLFLT *ArrayY, PLINT ny) {}

// No Y count but remember Y size to check others
%typemap(in) PLFLT *ArrayY (Matrix temp) {
  if ( _n_dims($input) > 1 )
      { error("argument must be a scalar or vector"); SWIG_fail; }
  temp = $input.matrix_value();
  $1 = &temp(0,0);
  Ylen = (PLINT)(_dim($input, 0));
}
%typemap(freearg) (PLFLT *ArrayY) {}

// 2D array with trailing dimensions, check consistency with previous
%typemap(in) (PLFLT *MatrixCk, PLINT nx, PLINT ny) (Matrix temp) {
  if ( _n_dims($input) >2 )
      { error("argument must be a scalar, vector, or 2D matrix."); SWIG_fail; }
  if ( _dim($input, 0) != Xlen )
      { error("argument matrix must have same X length as X vector"); SWIG_fail; }
  if ( _dim($input, 1) != Ylen )
      { error("argument matrix must have same Y length as Y vector"); SWIG_fail; }
  temp = $input.matrix_value();
  $1 = &temp(0,0);
  $2 = (PLINT)(_dim($input, 0));
  $3 = (PLINT)(_dim($input, 1));
}
%typemap(freearg) (PLFLT *MatrixCk, PLINT nx, PLINT ny) {}

// 2D array with trailing dimensions but set the X, Y size for later checking
%typemap(in) (PLFLT *Matrix, PLINT nx, PLINT ny) (Matrix temp) {
  if ( _n_dims($input) > 2 )
      { error("argument must be a scalar, vector, or 2D matrix."); SWIG_fail; }
  temp = $input.matrix_value();
  $1 = &temp(0,0);
  $2 = Xlen = (PLINT)(_dim($input, 0));
  $3 = Ylen = (PLINT)(_dim($input, 1));
}
%typemap(freearg) (PLFLT *Matrix, PLINT nx, PLINT ny) {}


// 2D array with no count but set the X, Y size for later checking
%typemap(in) PLFLT *Matrix (Matrix temp) {
  if ( _n_dims($input) > 2 )
      { error("argument must be a scalar, vector, or 2D matrix."); SWIG_fail; }
  temp = $input.matrix_value();
  $1 = &temp(0,0);
  Xlen = (PLINT)(_dim($input, 0));
  Ylen = (PLINT)(_dim($input, 1));
}
%typemap(freearg) PLFLT *Matrix {}


// 2D array with no count but check for consistency
%typemap(in) PLFLT *MatrixCk (Matrix temp) {
  if ( _n_dims($input) > 2 )
      { error("argument must be a scalar, vector, or 2D matrix."); SWIG_fail; }
  if ( _dim($input, 0) != Xlen )
      { error("argument matrix must have same X length as X vector"); SWIG_fail; }
  if ( _dim($input, 1) != Ylen )
      { error("argument matrix must have same Y length as Y vector"); SWIG_fail; }
  temp = $input.matrix_value();
  $1 = &temp(0,0);
}
%typemap(freearg) PLFLT *MatrixCk {}


// Set Y length for later consistency checking, with trailing count
// and 2D array, check for consistency input / output version
%typemap(in) (PLFLT *ArrayY, PLINT ny, PLFLT *OutMatrixCk) (Matrix temp, octave_value_list retval) {
  if ( _n_dims($input) > 1 )
      { error("argument must be a scalar or vector"); SWIG_fail; }
  temp = $input.matrix_value();
  $1 = &temp(0,0);
  $2 = Ylen = (PLINT)(_dim($input, 0));
  retval(0)=octave_value(Matrix(Xlen, Ylen));
  $3 = (PLFLT *)retval(0).matrix_value().data();

}
%typemap(argout) (PLFLT *ArrayY, PLINT ny, PLFLT *OutMatrixCk) {
  $result = SWIG_Octave_AppendOutput($result, retval$argnum(0));
}
%typemap(freearg) (PLFLT *ArrayY, PLINT ny, PLFLT *OutMatrixCk) {}


//-----------------------------------------------------------------------------
//				 String returning functions
//-----------------------------------------------------------------------------

// This currently used for character string output of less than 80
// bytes (e.g., plgdev, plgfnam, and plgver).  N.B. This works, but it
// was inspired by what was done by Rafael for matwrap with no deep
// knowledge of octave, i.e., no knowledge of exactly what is meant by
// charMatrix, etc.
%typemap(in, numinputs=0) char *OUTPUT (octave_value_list retval){
  retval(0) = octave_value(charMatrix(80, 1), true);
  $1 = (char *)retval(0).char_matrix_value().data();
}
%typemap(argout) char *OUTPUT {
  $result = SWIG_Octave_AppendOutput($result, retval$argnum(0)) ;
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
// The octave bindings started out as an independent project with a
// historical API that does not match up that well with the PLplot API
// function names and numbers and types of arguments.  So there are a
// lot of swig contortions here to preserve that historical octave
// bindings API.  At some point we may want to change to
// an Octave bindings API that is a better match with the normal PLplot API.
// This change would require less special documentation for the Octave case
// and would require fewer swig contortions, but it has the obvious downside
// of a massive octave bindings API breakage.

// N.B. For now we drop the vectorized form of arguments that the
// traditional matwrap-wrapped octave bindings have for plbtime,
// plcalc_world, plctime, plgcol0, plgcol0a, plhlsrgb, plrgbhls,
// plxormod, and plTranslateCursor for simplicity and because the
// unvectorized form works fine if you use explicit loops (as in the
// standard examples).  Note, this decision means there is an octave
// bindings API breakage with the swig-generated approach, but my
// (AWI's) judgement is this was a little used feature since typically
// (except for example 29) this functionality has not been used in the
// standard examples. We may want to vectorize the arguments of these
// octave functions at a later date or change the standard PLplot API
// to vectors for all of these if it really is a worthwhile API
// change.

// Our octave bindings use the name plSetOpt for the PLplot function,
// plsetopt, and use the plsetopt name for a different purpose
// (plsetopt.m).  We implement that here using the rename directive.
%rename(plSetOpt) plsetopt;

// Special octave form of plGetCursor.
%ignore plGetCursor;
%rename(plGetCursor) my_plGetCursor;
%{
static int my_plGetCursor( int *state, int *keysym, int *button, char *string, int *pX, int *pY, PLFLT *dX, PLFLT *dY, PLFLT *wX, PLFLT *wY, int *subwin )
{
    PLGraphicsIn gin;
    int          status; status = plGetCursor( &gin );
    *subwin = gin.subwindow; *state = gin.state; *keysym = gin.keysym; *button = gin.button;
    strncpy( string, gin.string, PL_MAXKEY-1);
    string[PL_MAXKEY-1] = '\0';
    
    *pX     = gin.pX; *pY = gin.pY; *dX = gin.dX; *dY = gin.dY; *wX = gin.wX; *wY = gin.wY;
    return status;
}
%}

int my_plGetCursor( int *OUTPUT, int *OUTPUT, int *OUTPUT, char *OUTPUT, int *OUTPUT, int *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT, int *OUTPUT );

// Special octave form of plTranslateCursor.
// Untested by any of our octave examples but should work.
%ignore plTranslateCursor;
%rename(plTranslateCursor) my_plTranslateCursor;

%{
// Translates relative device coordinates to world coordinates.
static int my_plTranslateCursor( PLFLT *x, PLFLT *y, PLFLT x_in, PLFLT y_in )
{
    PLGraphicsIn gin;
    int          st;
    gin.dX = x_in; gin.dY = y_in;
    st     = plTranslateCursor( &gin );
    *x     = gin.wX; *y = gin.wY;
    return st;
}
%}

int my_plTranslateCursor( PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT x_in, PLFLT y_in );

// Special octave form of plstripc.
%ignore plstripc;
%rename(plstripc) my_plstripc;
%{
// Create 1d stripchart

void my_plstripc( PLINT *id, const char *xspec, const char *yspec,
                  PLFLT xmin, PLFLT xmax, PLFLT xjump, PLFLT ymin, PLFLT ymax,
                  PLFLT xlpos, PLFLT ylpos,
                  PLBOOL y_ascl, PLBOOL acc,
                  PLINT colbox, PLINT collab,
                  PLINT *colline, PLINT *styline,
                  const char *legline1, const char *legline2, const char *legline3, const char *legline4,
                  const char *labx, const char *laby, const char *labtop )
{
    const char *legline[4];
    legline[0] = legline1; legline[1] = legline2;
    legline[2] = legline3; legline[3] = legline4;
    c_plstripc( id, xspec, yspec, xmin, xmax, xjump, ymin, ymax,
                xlpos, ylpos, y_ascl, acc, colbox, collab, colline, styline, legline,
        labx, laby, labtop );
}
%}

void my_plstripc( PLINT *OUTPUT, const char *xspec, const char *yspec,
                  PLFLT xmin, PLFLT xmax, PLFLT xjump, PLFLT ymin, PLFLT ymax,
                  PLFLT xlpos, PLFLT ylpos,
                  PLBOOL y_ascl, PLBOOL acc,
                  PLINT colbox, PLINT collab,
                  PLINT *Array, PLINT *ArrayCk,
                  const char *legline1, const char *legline2, const char *legline3, const char *legline4,
                  const char *labx, const char *laby, const char *labtop );

// Various special wrappers for plcont.

%ignore plcont;
%rename(plcont) my_plcont;
%rename(plcont0) my_plcont0;
%rename(plcont1) my_plcont1;
%rename(plcont2) my_plcont2;
%rename(plcont2p) my_plcont2p;

%{
// One more hack. As it is not possible (and would not be desirable) to pass
// an Octave function to plcont(), I have defined three plcont():
//      plcont uses a defined here xform()
//      plcont0 uses pltr0()
//      plcont1 uses pltr1()
//      plcont2 uses pltr2()
//      plcont2p uses pltr2p()
//
// Also, as plplot expect vectorized bidimensional arrays, I provided a
// f2c, which is a #define that does the necessary conversion.
//

void xform( PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data )
{
    *tx = *( (PLFLT *) pltr_data + 0 ) * x + *( (PLFLT *) pltr_data + 1 ) * y + *( (PLFLT *) pltr_data + 2 );
    *ty = *( (PLFLT *) pltr_data + 3 ) * x + *( (PLFLT *) pltr_data + 4 ) * y + *( (PLFLT *) pltr_data + 5 );
}

// convert from Fortran like arrays (one vector), to C like 2D arrays

#define  f2c( f, ff, nx, ny )                              \
    PLFLT * *ff;                                           \
    ff = (PLFLT **) alloca( nx * sizeof ( PLFLT * ) );     \
    for ( int i = 0; i < nx; i++ ) {                       \
        ff[i] = (PLFLT *) alloca( ny * sizeof ( PLFLT ) ); \
        for ( int j = 0; j < ny; j++ )                     \
            *( ff[i] + j ) = *( f + nx * j + i );}

// simpler plcont() for use with xform()

void my_plcont( PLFLT *f, PLINT nx, PLINT ny, PLINT kx, PLINT lx, PLINT ky,
                PLINT ly, PLFLT *clevel, PLINT nlevel, PLFLT *tr )
{
    f2c( f, ff, nx, ny );
    c_plcont( ff, nx, ny, kx, lx, ky, ly, clevel, nlevel, xform, tr );
}

// plcont() for use with pltr0() NOT TESTED

void my_plcont0( PLFLT *f, PLINT nx, PLINT ny, PLINT kx, PLINT lx, PLINT ky,
                 PLINT ly, PLFLT *clevel, PLINT nlevel )
{
    f2c( f, ff, nx, ny );
    c_plcont( ff, nx, ny, kx, lx, ky, ly, clevel, nlevel, pltr0, NULL );
}

// plcont() for use with pltr1()

void my_plcont1( PLFLT *f, PLINT nx, PLINT ny, PLINT kx, PLINT lx, PLINT ky,
                 PLINT ly, PLFLT *clevel, PLINT nlevel, PLFLT *xg, PLFLT *yg )
{
    PLcGrid grid1;
    grid1.nx = nx;  grid1.ny = ny;
    grid1.xg = xg;  grid1.yg = yg;
    f2c( f, ff, nx, ny );
    c_plcont( ff, nx, ny, kx, lx, ky, ly, clevel, nlevel, pltr1, &grid1 );
}

// plcont() for use with pltr2()
void my_plcont2( PLFLT *f, PLINT nx, PLINT ny, PLINT kx, PLINT lx, PLINT ky,
                 PLINT ly, PLFLT *clevel, PLINT nlevel, PLFLT *xg, PLFLT *yg )
{
    PLcGrid2 grid2;
    f2c( xg, xgg, nx, ny ); f2c( yg, ygg, nx, ny );
    grid2.nx = nx;  grid2.ny = ny;
    grid2.xg = xgg; grid2.yg = ygg;
    f2c( f, ff, nx, ny );
    c_plcont( ff, nx, ny, kx, lx, ky, ly, clevel, nlevel, pltr2, &grid2 );
}

// plcont() for use with pltr2p()

void my_plcont2p( PLFLT *f, PLINT nx, PLINT ny, PLINT kx, PLINT lx, PLINT ky,
                  PLINT ly, PLFLT *clevel, PLINT nlevel, PLFLT *xg, PLFLT *yg )
{
    PLcGrid2 grid2;
    f2c( xg, xgg, nx, ny ); f2c( yg, ygg, nx, ny );
    grid2.nx = nx;  grid2.ny = ny;
    grid2.xg = xgg; grid2.yg = ygg;
    f2c( f, ff, nx, ny );
    c_plcont( ff, nx, ny, kx, lx, ky, ly, clevel, nlevel, pltr2, &grid2 );
}
%}

void my_plcont( PLFLT *Matrix, PLINT nx, PLINT ny, PLINT kx, PLINT lx, PLINT ky,
                PLINT ly, PLFLT *Array, PLINT n, PLFLT *Array );
void my_plcont0( PLFLT *Matrix, PLINT nx, PLINT ny, PLINT kx, PLINT lx, PLINT ky,
                 PLINT ly, PLFLT *Array, PLINT n );
void my_plcont1( PLFLT *Matrix, PLINT nx, PLINT ny, PLINT kx, PLINT lx, PLINT ky,
                 PLINT ly, PLFLT *Array, PLINT n, PLFLT *ArrayCkX, PLFLT *ArrayCkY );
void my_plcont2( PLFLT *Matrix, PLINT nx, PLINT ny, PLINT kx, PLINT lx, PLINT ky,
                 PLINT ly, PLFLT *Array, PLINT n, PLFLT *MatrixCk, PLFLT *MatrixCk );
void my_plcont2p( PLFLT *Matrix, PLINT nx, PLINT ny, PLINT kx, PLINT lx, PLINT ky,
                  PLINT ly, PLFLT *Array, PLINT n, PLFLT *MatrixCk, PLFLT *MatrixCk );

// plgriddata wrapper.
%ignore plgriddata;
%rename(plgriddata) my_plgriddata;

%{
void my_plgriddata( PLFLT *x, PLFLT *y, PLFLT *z, int npts,
                    PLFLT *xg, int nptsx, PLFLT *yg, int nptsy,
                    PLFLT *zg, int type, PLFLT data )
{
    f2c( zg, zgg, nptsx, nptsy );
    plgriddata( x, y, z, npts, xg, nptsx, yg, nptsy, zgg, type, data );
    for ( int i = 0; i < nptsx; i++ )
        for ( int j = 0; j < nptsy; j++ )
            *( zg + nptsx * j + i ) = zgg[i][j];
}
%}

void my_plgriddata( PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk, PLINT n,
            PLFLT *ArrayX, PLINT nx, PLFLT *ArrayY, PLINT ny,
            PLFLT *OutMatrixCk, PLINT type, PLFLT data );

// plmesh-related wrappers.
%ignore plmesh;
%rename(plmesh) my_plmesh;
%ignore plmeshc;
%rename(plmeshc) my_plmeshc;

%{
// Plots a mesh representation of the function z[x][y].

void my_plmesh( PLFLT *x, PLFLT *y, PLFLT *z, PLINT nx, PLINT ny, PLINT opt )
{
    f2c( z, zz, nx, ny );
    c_plmesh( x, y, zz, nx, ny, opt );
}

// Plots a mesh representation of the function z[x][y] with contour

void my_plmeshc( PLFLT *x, PLFLT *y, PLFLT *z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel )
{
    f2c( z, zz, nx, ny );
    c_plmeshc( x, y, zz, nx, ny, opt, clevel, nlevel );
}
%}

void my_plmesh( PLFLT *ArrayX, PLFLT *ArrayY, PLFLT *MatrixCk,
        PLINT nx, PLINT ny, PLINT opt );

void my_plmeshc( PLFLT *ArrayX, PLFLT *ArrayY, PLFLT *MatrixCk,
         PLINT nx, PLINT ny, PLINT opt, PLFLT *Array, PLINT n );

// plot3d-related wrappers.

%ignore plot3d;
%rename(plot3d) my_plot3d;
%ignore plot3dc;
%rename(plot3dc) my_plot3dc;
%ignore plot3dcl;
//unimplemented: %rename(plot3dcl) my_plot3dcl;

%{
// Plots a 3-d representation of the function z[x][y].
void my_plot3d( PLFLT *x, PLFLT *y, PLFLT *z,
                PLINT nx, PLINT ny, PLINT opt, PLINT side )
{
    f2c( z, zz, nx, ny )
    c_plot3d( x, y, zz, nx, ny, opt, side );
}

// Plots a 3-d representation of the function z[x][y] with contour
void my_plot3dc( PLFLT *x, PLFLT *y, PLFLT *z,
                 PLINT nx, PLINT ny, PLINT opt,
                 PLFLT *clevel, PLINT nlevel )
{
    f2c( z, zz, nx, ny )
    c_plot3dc( x, y, zz, nx, ny, opt, clevel, nlevel );
}
%}

void my_plot3d( PLFLT *ArrayX, PLFLT *ArrayY, PLFLT *MatrixCk,
        PLINT nx, PLINT ny, PLINT opt, PLBOOL side );

void my_plot3dc( PLFLT *ArrayX, PLFLT *ArrayY, PLFLT *MatrixCk,
         PLINT nx, PLINT ny, PLINT opt, PLFLT *Array, PLINT n );

// plsurf3d-related wrappings:
%ignore plsurf3d;
%rename(plsurf3d) my_plsurf3d;
%ignore plsurf3dl;
//unimplemented: %rename(plsurf3d) my_plsurf3d;

%{
void my_plsurf3d( PLFLT *x, PLFLT *y, PLFLT *z,
                  PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel )
{
    f2c( z, zz, nx, ny )
    c_plsurf3d( x, y, zz, nx, ny, opt, clevel, nlevel );
}
%}

void my_plsurf3d( PLFLT *ArrayX, PLFLT *ArrayY, PLFLT *MatrixCk,
          PLINT nx, PLINT ny, PLINT opt, PLFLT *Array, PLINT n );

// plshade-related wrappers.

%ignore plshade;
%rename(plshade) my_plshade;
// plshade1 and plshade2 are untested by the standard examples.
%rename(plshade1) my_plshade1;
%rename(plshade2) my_plshade2;

%{
// The same as in plcont. I have hardcoded the first function pointer
// to plfill(). The second function pointer will use the same convention
// as in plcont().
//

// the simpler plshade()
void my_plshade( PLFLT *a, PLINT nx, PLINT ny, PLFLT *defined,
                 PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
                 PLFLT shade_min, PLFLT shade_max,
                 PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
                 PLINT min_color, PLINT min_width,
                 PLINT max_color, PLINT max_width,
                 PLINT rectangular, PLFLT *tr )
{
    f2c( a, aa, nx, ny );
    c_plshade( aa, nx, ny, NULL, left, right, bottom, top,
        shade_min, shade_max, sh_cmap, sh_color, sh_width,
        min_color, min_width, max_color, max_width,
        plfill, rectangular, xform, tr );
}

//  plshade() for use with pltr1
void my_plshade1( PLFLT *a, PLINT nx, PLINT ny, const char *defined,
                  PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
                  PLFLT shade_min, PLFLT shade_max,
                  PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
                  PLINT min_color, PLINT min_width,
                  PLINT max_color, PLINT max_width,
                  PLINT rectangular, PLFLT *xg, PLFLT *yg )
{
    PLcGrid grid1;
    grid1.nx = nx;  grid1.ny = ny;
    grid1.xg = xg;  grid1.yg = yg;
    f2c( a, aa, nx, ny );
    c_plshade( aa, nx, ny, NULL, left, right, bottom, top,
        shade_min, shade_max, sh_cmap, sh_color, sh_width,
        min_color, min_width, max_color, max_width,
        plfill, rectangular, pltr1, &grid1 );
}

//  plshade() for use with pltr2
void my_plshade2( PLFLT *a, PLINT nx, PLINT ny, const char *defined,
                  PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
                  PLFLT shade_min, PLFLT shade_max,
                  PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
                  PLINT min_color, PLINT min_width,
                  PLINT max_color, PLINT max_width,
                  PLINT rectangular, PLFLT *xg, PLFLT *yg )
{
    PLcGrid2 grid2;
    f2c( xg, xgg, nx, ny ); f2c( yg, ygg, nx, ny );
    grid2.nx = nx;  grid2.ny = ny;
    grid2.xg = xgg; grid2.yg = ygg;
    f2c( a, aa, nx, ny );
    c_plshade( aa, nx, ny, NULL, left, right, bottom, top,
        shade_min, shade_max, sh_cmap, sh_color, sh_width,
        min_color, min_width, max_color, max_width,
        plfill, rectangular, pltr2, &grid2 );
}

%}

// The defined functionality is completely unused, but through
// a historical anomaly is typed differently between my_plshade and
// my_plshade1 (or my_plshade2) which is why we use PLFLT *Array for
// the fourth argument of my_plshade, but const char * defined
// for the other fourth arguments.  FIXME.  I (AWI) recommend an API break
// with this fourth (unused) argument completely eliminated, but
// that needs discussion.
// my_plshade1 and my_plshade2 are completely untested by the standard examples.

void my_plshade( PLFLT *Matrix, PLINT nx, PLINT ny, PLFLT *Array,
         PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
         PLFLT shade_min, PLFLT shade_max,
         PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
         PLINT min_color, PLINT min_width,
         PLINT max_color, PLINT max_width,
         PLBOOL rectangular, PLFLT *Array );

void my_plshade1( PLFLT *Matrix, PLINT nx, PLINT ny, const char *defined,
         PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
         PLFLT shade_min, PLFLT shade_max,
         PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
         PLINT min_color, PLINT min_width,
         PLINT max_color, PLINT max_width,
         PLBOOL rectangular, PLFLT *ArrayCkX, PLFLT *ArrayCkY);

void my_plshade2( PLFLT *Matrix, PLINT nx, PLINT ny, const char *defined,
         PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
         PLFLT shade_min, PLFLT shade_max,
         PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
         PLINT min_color, PLINT min_width,
         PLINT max_color, PLINT max_width,
         PLBOOL rectangular, PLFLT *Matrix, PLFLT *Matrix);

// plshades-related wrappers.

%ignore plshades;
%rename(plshades) my_plshades;
// plshadesx untested by examples.
%rename(plshadesx) my_plshadesx;
%rename(plshades1) my_plshades1;
%rename(plshades2) my_plshades2;

%{
void my_plshades( PLFLT *a, PLINT nx, PLINT ny,
                  PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
                  PLFLT *clevel, PLINT nlevel, PLINT fill_width,
                  PLINT cont_color, PLINT cont_width,
                  PLINT rectangular )
{
    f2c( a, aa, nx, ny );
    c_plshades( aa, nx, ny, NULL, left, right, bottom, top,
        clevel, nlevel, fill_width, cont_color, cont_width,
        plfill, rectangular, NULL, NULL );
}

void my_plshadesx( PLFLT *a, PLINT nx, PLINT ny,
                   PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
                   PLFLT *clevel, PLINT nlevel, PLINT fill_width,
                   PLINT cont_color, PLINT cont_width,
                   PLINT rectangular, PLFLT *tr )
{
    f2c( a, aa, nx, ny );
    c_plshades( aa, nx, ny, NULL, left, right, bottom, top,
        clevel, nlevel, fill_width, cont_color, cont_width,
        plfill, rectangular, xform, tr );
}

void my_plshades1( PLFLT *a, PLINT nx, PLINT ny,
                   PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
                   PLFLT *clevel, PLINT nlevel, PLINT fill_width,
                   PLINT cont_color, PLINT cont_width,
                   PLINT rectangular, PLFLT *xg, PLFLT *yg )
{
    PLcGrid grid1;
    grid1.nx = nx;  grid1.ny = ny;
    grid1.xg = xg;  grid1.yg = yg;

    f2c( a, aa, nx, ny );
    c_plshades( aa, nx, ny, NULL, left, right, bottom, top,
        clevel, nlevel, fill_width, cont_color, cont_width,
        plfill, rectangular, pltr1, &grid1 );
}

void my_plshades2( PLFLT *a, PLINT nx, PLINT ny,
                   PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
                   PLFLT *clevel, PLINT nlevel, PLINT fill_width,
                   PLINT cont_color, PLINT cont_width,
                   PLINT rectangular, PLFLT *xg, PLFLT *yg )
{
    PLcGrid2 grid2;
    f2c( xg, xgg, nx, ny ); f2c( yg, ygg, nx, ny );
    grid2.nx = nx;  grid2.ny = ny;
    grid2.xg = xgg; grid2.yg = ygg;
    f2c( a, aa, nx, ny );
    c_plshades( aa, nx, ny, NULL, left, right, bottom, top,
        clevel, nlevel, fill_width, cont_color, cont_width,
        plfill, rectangular, pltr2, &grid2 );
}
%}

void my_plshades( PLFLT *Matrix, PLINT nx, PLINT ny,
          PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
          PLFLT *Array, PLINT n, PLINT fill_width,
          PLINT cont_color, PLINT cont_width,
          PLBOOL rectangular);

void my_plshadesx( PLFLT *Matrix, PLINT nx, PLINT ny,
          PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
          PLFLT *Array, PLINT n, PLINT fill_width,
          PLINT cont_color, PLINT cont_width,
          PLBOOL rectangular, PLFLT *Array);

void my_plshades1( PLFLT *Matrix, PLINT nx, PLINT ny,
          PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
          PLFLT *Array, PLINT n, PLINT fill_width,
          PLINT cont_color, PLINT cont_width,
          PLBOOL rectangular, PLFLT *ArrayCkX, PLFLT *ArrayCkY);

void my_plshades2( PLFLT *Matrix, PLINT nx, PLINT ny,
          PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
          PLFLT *Array, PLINT n, PLINT fill_width,
          PLINT cont_color, PLINT cont_width,
          PLBOOL rectangular, PLFLT *Matrix, PLFLT *Matrix);

// plvect-related wrappers.

%ignore plvect;
// plvect and plvect1 untested by standard examples.
%rename(plvect) my_plvect;
%rename(plvect1) my_plvect1;
%rename(plvect2) my_plvect2;

%{
// Plot an array of vector arrows - uses the same function pointer
// convention as plcont

void my_plvect( PLFLT *u, PLFLT *v, PLINT nx, PLINT ny, PLFLT scale, PLFLT *tr )
{
    f2c( u, uu, nx, ny );
    f2c( v, vv, nx, ny );
    c_plvect( uu, vv, nx, ny, scale, xform, tr );
}

// plvect() for use with pltr1
void my_plvect1( PLFLT *u, PLFLT *v, PLINT nx, PLINT ny, PLFLT scale, PLFLT *xg, PLFLT *yg )
{
    PLcGrid grid1;
    grid1.nx = nx;  grid1.ny = ny;
    grid1.xg = xg;  grid1.yg = yg;
    f2c( u, uu, nx, ny );
    f2c( v, vv, nx, ny );
    c_plvect( uu, vv, nx, ny, scale, pltr1, &grid1 );
}

// plvect() for use with pltr2
void my_plvect2( PLFLT *u, PLFLT *v, PLINT nx, PLINT ny, PLFLT scale, PLFLT *xg, PLFLT *yg )
{
    PLcGrid2 grid2;
    f2c( xg, xgg, nx, ny );  f2c( yg, ygg, nx, ny );
    grid2.nx = nx;  grid2.ny = ny;
    grid2.xg = xgg;  grid2.yg = ygg;
    f2c( u, uu, nx, ny );
    f2c( v, vv, nx, ny );
    c_plvect( uu, vv, nx, ny, scale, pltr2, &grid2 );
}
%}

void my_plvect( PLFLT *Matrix, PLFLT *MatrixCk, PLINT nx, PLINT ny, PLFLT scale,
        PLFLT *Array );

void my_plvect1( PLFLT *Matrix, PLFLT *MatrixCk, PLINT nx, PLINT ny, PLFLT scale,
         PLFLT *ArrayCkX, PLFLT *ArrayCkY );

void my_plvect2( PLFLT *Matrix, PLFLT *MatrixCk, PLINT nx, PLINT ny, PLFLT scale,
         PLFLT *Matrix, PLFLT *Matrix);

// plimage-related wrappers.
%ignore plimage;
%rename(pplimage) my_plimage;
%ignore plimagefr;
%rename(plimagefr) my_plimagefr;
%rename(plimagefrx) my_plimagefrx;
%rename(plimagefr1) my_plimagefr1;
%rename(plimagefr2) my_plimagefr2;

%{
// Plot an image with distortion - uses the same function pointer
void my_plimage( PLFLT *a, PLINT nx, PLINT ny,
                 PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
                 PLFLT zmin, PLFLT zmax,
                 PLFLT dxmin, PLFLT dxmax, PLFLT dymin, PLFLT dymax )
{
    f2c( a, aa, nx, ny );
    plimage( aa, nx, ny, xmin, xmax, ymin, ymax, zmin, zmax, dxmin, dxmax, dymin, dymax );
}

// Plot an image with distortion - uses the same function pointer
// convention as plcont
void my_plimagefr( PLFLT *a, PLINT nx, PLINT ny,
                   PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
                   PLFLT zmin, PLFLT zmax,
                   PLFLT valuemin, PLFLT valuemax )
{
    f2c( a, aa, nx, ny );
    plimagefr( aa, nx, ny, xmin, xmax, ymin, ymax, zmin, zmax, valuemin, valuemax, NULL, NULL );
}

void my_plimagefrx( PLFLT *a, PLINT nx, PLINT ny,
                    PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
                    PLFLT zmin, PLFLT zmax,
                    PLFLT valuemin, PLFLT valuemax, PLFLT *tr )
{
    f2c( a, aa, nx, ny );
    plimagefr( aa, nx, ny, xmin, xmax, ymin, ymax, zmin, zmax, valuemin, valuemax, xform, tr );
}

// plimagefr() for use with pltr1
void my_plimagefr1( PLFLT *a, PLINT nx, PLINT ny,
                    PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
                    PLFLT zmin, PLFLT zmax,
                    PLFLT valuemin, PLFLT valuemax, PLFLT *xg, PLFLT *yg )
{
    PLcGrid grid1;
    grid1.nx = nx + 1;  grid1.ny = ny + 1;
    grid1.xg = xg;  grid1.yg = yg;
    f2c( a, aa, nx, ny );
    c_plimagefr( aa, nx, ny, xmin, xmax, ymin, ymax, zmin, zmax, valuemin, valuemax, pltr1, &grid1 );
}

// plimagefr() for use with pltr2
void my_plimagefr2( PLFLT *a, PLINT nx, PLINT ny,
                    PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
                    PLFLT zmin, PLFLT zmax,
                    PLFLT valuemin, PLFLT valuemax, PLFLT *xg, PLFLT *yg )
{
    PLcGrid2 grid2;
    f2c( xg, xgg, ( nx + 1 ), ( ny + 1 ) );  f2c( yg, ygg, ( nx + 1 ), ( ny + 1 ) );
    grid2.nx = nx + 1;  grid2.ny = ny + 1;
    grid2.xg = xgg;  grid2.yg = ygg;
    f2c( a, aa, nx, ny );
    c_plimagefr( aa, nx, ny, xmin, xmax, ymin, ymax, zmin, zmax, valuemin, valuemax, pltr2, &grid2 );
}

%}

void my_plimage( PLFLT *Matrix, PLINT nx, PLINT ny,
         PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT zmin, PLFLT zmax,
         PLFLT Dxmin, PLFLT Dxmax, PLFLT Dymin, PLFLT Dymax );

void my_plimagefr( PLFLT *Matrix, PLINT nx, PLINT ny,
         PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT zmin, PLFLT zmax,
                PLFLT valuemin, PLFLT valuemax );

void my_plimagefrx( PLFLT *Matrix, PLINT nx, PLINT ny,
         PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT zmin, PLFLT zmax,
         PLFLT valuemin, PLFLT valuemax, PLFLT *Array );

void my_plimagefr1( PLFLT *Matrix, PLINT nx, PLINT ny,
         PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT zmin, PLFLT zmax,
         PLFLT valuemin, PLFLT valuemax, PLFLT *ArrayCkX, PLFLT * ArrayCkY );

void my_plimagefr2( PLFLT *Matrix, PLINT nx, PLINT ny,
         PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT zmin, PLFLT zmax,
         PLFLT valuemin, PLFLT valuemax, PLFLT *Matrix, PLFLT *Matrix );

// This test function should be removed when we are confident of our
// dealings with all types of octave string arrays.
%{
void testppchar(PLINT nlegend, const PLINT *opt_array, const char ** text) {
  PLINT i;
  printf("nlegend = %d\n", nlegend);
  for(i=0;i<nlegend;i++) {
    printf("opt_array[%d] = %d\n", i, opt_array[i]);
    printf("strlen(text[%d]) = %d\n", i, strlen(text[i]));
    printf("text[%d] = %s\n", i, text[i]);
  }
}
%}

// No count but check consistency with previous
%typemap(in) char **ArrayCk (charMatrix temp) {
  int i, max_length, non_blank_length;
  if ( _n_dims($input) > 2 )
      { error("argument must be a scalar or vector or matrix"); SWIG_fail; }
  if ( _dim($input, 0) != Alen )
      { error("first dimension must be same length as previous vector"); SWIG_fail; }
  // Allow one extra space for null termination.
  max_length= _dim($input, 1) + 1;
  $1 = new char*[Alen];
  temp = $input.char_matrix_value();
  for(i=0; i<Alen; i++) {
    // Must copy string to "permanent" location because the string
    // location corresponding to tmp_cstring gets
    // overwritten for each iteration of loop.
    const char *tmp_cstring = temp.row_as_string(i).c_str();
    $1[i] = new char[max_length];
    strncpy( $1[i], tmp_cstring, max_length-1 );
    $1[i][max_length-1] = '\0';
    // remove trailing-blank padding that is used by the
    // charMatrix class to insure all strings in a given
    // charMatrix instance have the same length.
    // This transformation also removes legitimate trailing
    // blanks but there is nothing we can do about that
    // for the charMatrix class.

    // Look for trailing nulls first (just in case, although that
    // shouldn't happen if charMatrix implemented as documented)
    // before looking for trailing blanks.
    non_blank_length = max_length-2;
    while(non_blank_length >= 0 && $1[i][non_blank_length] == '\0') {non_blank_length--;}
    while(non_blank_length >= 0 && $1[i][non_blank_length] == ' ') {non_blank_length--;}
    $1[i][non_blank_length+1] = '\0';
  }
  
}
%typemap(freearg) char **ArrayCk {
  int i;
  for(i=0; i<Alen; i++) {
    delete[] $1[i];
  }
  delete[] $1;
}

// This test function should be removed when we are confident of our
// dealings with all types of octave string arrays.
void testppchar(PLINT n, const PLINT *Array, const char **ArrayCk);

// Probably never deal with this one.
%ignore plMinMax2dGrid;
// swig-compatible common PLplot API definitions from here on.
%include plplotcapi.i

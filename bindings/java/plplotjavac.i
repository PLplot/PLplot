/* 
Copyright 2002 Gary Bishop
Copyright 2002 Alan W. Irwin
This file is part of PLplot.

This file is free software; you can redistribute it and/or modify
it under the terms of the GNU Library General Public License as published by
the Free Software Foundation; version 2 of the License.

This file is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Library General Public License for more details.

You should have received a copy of the GNU Library General Public License
along with the file; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*/

/* 
A SWIG interface to PLplot for Java. This wrapper does the following:

   1) it strictly provides the C-API with the usual change of not
      requiring lengths for arrays,

   2) it attempts to provide the entire API including callbacks for
      plcont and plshade.
      
   3) it works both with the single and double-precision versions of the
      PLplot library.

This is known to work with swig-1.3.17 on Linux.

*/
%module plplotjavac
%include typemaps.i

%{
#include "plplotP.h"      
%}

#ifdef PL_DOUBLE
typedef double PLFLT;
#else
typedef float PLFLT;
#endif

typedef int PLINT;

/***************************
	A trick for docstrings
****************************/

%define DOC(func, string) 
%wrapper %{#define _doc_ ## func string %}
%enddef

/* Infrastructure for handling swig compatible plplot API definitions. */

#ifdef PL_DOUBLE
#define setup_array_1d_PLFLT setup_array_1d_d
#define setup_array_2d_PLFLT setup_array_2d_d
#define jPLFLTArray "jdoubleArray"
#define jPLFLTbracket "double[]"
#define jPLFLTbracket2 "double[][]"
#define GetPLFLTArrayElements GetDoubleArrayElements
#define ReleasePLFLTArrayElements ReleaseDoubleArrayElements
#define jPLFLT jdouble
#else
#define setup_array_1d_PLFLT setup_array_1d_f
#define setup_array_2d_PLFLT setup_array_2d_f
#define jPLFLTArray "jfloatArray"
#define jPLFLTbracket "float[]"
#define jPLFLTbracket2 "float[][]"
#define GetPLFLTArrayElements GetFloatArrayElements
#define ReleasePLFLTArrayElements ReleaseFloatArrayElements
#define jPLFLT jfloat
#endif

%{
/*---------------------------------------------------------------------------//
// Array allocation & copy helper routines.  Note because of swig limitations
// it is necessary to release the java array memory right after calling these
// routines.  Thus it is necessary to allocate and copy the arrays  even if 
// the java and plplot arrays are of the same type.  Note, because of this 
// change to Geoffrey's original versions, caller must always free memory 
// afterwards.  Thus, the must_free_buffers logic is gone as well.
//---------------------------------------------------------------------------*/

/* 1d array of jints */

static void 
setup_array_1d_i( PLINT **pa, jint *adat, int n )
{
   int i;
   *pa = (PLINT *) malloc( n * sizeof(PLINT) );
   for( i=0; i < n; i++ ) {
      (*pa)[i] = adat[i];
   }
}

/* 1d array of jfloats */

static void 
setup_array_1d_f( PLFLT **pa, jfloat *adat, int n )
{
   int i;
   *pa = (PLFLT *) malloc( n * sizeof(PLFLT) );
   for( i=0; i < n; i++ ) {
      (*pa)[i] = adat[i];
   }
}

/* 1d array of jdoubles */

static void
setup_array_1d_d( PLFLT **pa, jdouble *adat, int n )
{
   int i;
   *pa = (PLFLT *) malloc( n * sizeof(PLFLT) );
   for( i=0; i < n; i++ ) {
      (*pa)[i] = adat[i];
   }
}

/* 2d array of floats */
/* Here caller must free(a[0]) and free(a) (in that order) afterward */

static void 
setup_array_2d_f( PLFLT ***pa, jfloat **adat, int nx, int ny )
{
   int i, j;

   *pa = (PLFLT **) malloc( nx * sizeof(PLFLT *) );
   (*pa)[0] = (PLFLT *) malloc( nx * ny * sizeof(PLFLT) );

   for( i=0; i < nx; i++ )
     {
	(*pa)[i] = (*pa)[0] + i*ny;
	for( j=0; j < ny; j++ )
	  (*pa)[i][j] = adat[i][j];
     }

}

/* 2d array of doubles */
/* Here caller must free(a[0]) and free(a) (in that order) afterward */

static void
setup_array_2d_d( PLFLT ***pa, jdouble **adat, int nx, int ny )
{
   int i, j;

   *pa = (PLFLT **) malloc( nx * sizeof(PLFLT *) );
   (*pa)[0] = (PLFLT *) malloc( nx * ny * sizeof(PLFLT) );

   for( i=0; i < nx; i++ )
     {
	(*pa)[i] = (*pa)[0] + i*ny;
	for( j=0; j < ny; j++ )
	  (*pa)[i][j] = adat[i][j];
     }

}
%}



/* I hate global variables but this is the best way I can think of to manage consistency
   checking among function arguments. */
%{
   static PLINT Alen = 0;
   static PLINT Xlen = 0, Ylen = 0;
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

/**********************************************************************************
			 PLINT arrays
**********************************************************************************/

/* with preceding count */
%typemap(in) (PLINT n, PLINT* Array) {
   jint *jxdata = (*jenv)->GetIntArrayElements( jenv, $input, 0 );
   $1 = (*jenv)->GetArrayLength( jenv, $input);
   Alen = $1;
   setup_array_1d_i( &$2, jxdata, Alen);
   /* Could find no easy way to do this as part of freearg so I modified
    * the previous function so it ALWAYS mallocs and copies so that
    * the java array can be released immediately. */
   (*jenv)->ReleaseIntArrayElements( jenv, $input, jxdata, 0 );
}
%typemap(freearg) (PLINT n, PLINT* Array) {
   free($2);
}
%typemap(jni) (PLINT n, PLINT* Array) "jintArray"
%typemap(jtype) (PLINT n, PLINT* Array) "int[]"
%typemap(jstype) (PLINT n, PLINT* Array) "int[]"
%typemap(javain) (PLINT n, PLINT* Array) "$javainput"
%typemap(javaout) (PLINT n, PLINT* Array) {
   return $jnicall;
}
  
/* check consistency with previous */
%typemap(in) PLINT* ArrayCk {
   jint *jydata = (*jenv)->GetIntArrayElements( jenv, $input, 0 );
   if((*jenv)->GetArrayLength( jenv, $input ) != Alen) {
      printf("Vectors must be same length.\n");
      return;
   }
   setup_array_1d_i( &$1, jydata, Alen);
   (*jenv)->ReleaseIntArrayElements( jenv, $input, jydata, 0 );
}
%typemap(freearg) PLINT* ArrayCk {
   free($1);
}
%typemap(jni) PLINT* ArrayCk "jintArray"
%typemap(jtype) PLINT* ArrayCk "int[]"
%typemap(jstype) PLINT* ArrayCk "int[]"
%typemap(javain) PLINT* ArrayCk "$javainput"
%typemap(javaout) PLINT* ArrayCk {
   return $jnicall;
}
  
/* Weird case to allow argument to be one shorter than others */
%typemap(in) PLINT* ArrayCkMinus1 {
   jint *jydata = (*jenv)->GetIntArrayElements( jenv, $input, 0 );
   if((*jenv)->GetArrayLength( jenv, $input ) < Alen-1) {
      printf("Vector must be at least length of others minus 1.\n");
      return;
   }
   setup_array_1d_i( &$1, jydata, Alen);
   (*jenv)->ReleaseIntArrayElements( jenv, $input, jydata, 0 );
}
%typemap(freearg) PLINT* ArrayCkMinus1 {
   free($1);
}
%typemap(jni) PLINT* ArrayCkMinus1 "jintArray"
%typemap(jtype) PLINT* ArrayCkMinus1 "int[]"
%typemap(jstype) PLINT* ArrayCkMinus1 "int[]"
%typemap(javain) PLINT* ArrayCkMinus1 "$javainput"
%typemap(javaout) PLINT* ArrayCkMinus1 {
   return $jnicall;
}
  
/* No length but remember size to check others */
%typemap(in) PLINT *Array {
   jint *jydata = (*jenv)->GetIntArrayElements( jenv, $input, 0 );
   Alen = (*jenv)->GetArrayLength( jenv, $input);
   setup_array_1d_i( &$1, jydata, Alen);
   (*jenv)->ReleaseIntArrayElements( jenv, $input, jydata, 0 );
}
%typemap(freearg) PLINT *Array {
   free($1);
}
%typemap(jni) PLINT *Array "jintArray"
%typemap(jtype) PLINT *Array "int[]"
%typemap(jstype) PLINT *Array "int[]"
%typemap(javain) PLINT *Array "$javainput"
%typemap(javaout) PLINT *Array {
   return $jnicall;
}
  
/* Trailing count */
%typemap(in) (PLINT *ArrayCk, PLINT n) {
   jint *jydata = (*jenv)->GetIntArrayElements( jenv, $input, 0 );
   if((*jenv)->GetArrayLength( jenv, $input ) != Alen) {
      printf("Vectors must be same length.\n");
      return;
   }
   setup_array_1d_i( &$1, jydata, Alen);
   (*jenv)->ReleaseIntArrayElements( jenv, $input, jydata, 0 );
}
%typemap(freearg) (PLINT *ArrayCk, PLINT n) {
   free($1);
}
%typemap(jni) (PLINT *ArrayCk, PLINT n) "jintArray"
%typemap(jtype) (PLINT *ArrayCk, PLINT n) "int[]"
%typemap(jstype) (PLINT *ArrayCk, PLINT n) "int[]"
%typemap(javain) (PLINT *ArrayCk, PLINT n) "$javainput"
%typemap(javaout) (PLINT *ArrayCk, PLINT n) {
   return $jnicall;
}

/******************************************************************************
				 PLFLT Arrays 
******************************************************************************/

//temporary
#if 0
#ifndef PL_DOUBLE
%wrapper %{
/* some really twisted stuff to allow calling a single precision library from python */
PyArrayObject* myArray_ContiguousFromObject(PyObject* in, int type, int mindims, int maxdims)
{
  PyArrayObject* tmp = (PyArrayObject*)PyArray_ContiguousFromObject(in, PyArray_FLOAT,
								    mindims, maxdims);
  if (!tmp) {
    /* could be an incoming double array which can't be "safely" converted, do it anyway */
    if(PyArray_Check(in)) {
      PyErr_Clear();
      tmp = (PyArrayObject*)PyArray_Cast((PyArrayObject*)in, PyArray_FLOAT);
    }
  }
  return tmp;
}
 %}
#else
%wrapper %{
#define myArray_ContiguousFromObject PyArray_ContiguousFromObject
  %}
#endif

/* temporary*/
#endif
/* with preceding count */
%typemap(in) (PLINT n, PLFLT* Array) {
   jPLFLT *jxdata = (*jenv)->GetPLFLTArrayElements( jenv, $input, 0 );
   $1 = (*jenv)->GetArrayLength( jenv, $input);
   Alen = $1;
   setup_array_1d_PLFLT( &$2, jxdata, Alen );
   /* Could find no easy way to do this as part of freearg so I modified
    * the previous function so it ALWAYS mallocs and copies so that
    * the java array can be released immediately. */
   (*jenv)->ReleasePLFLTArrayElements( jenv, $input, jxdata, 0 );
}
%typemap(freearg) (PLINT n, PLFLT* Array) {
   free($2);
}
%typemap(jni) (PLINT n, PLFLT* Array) jPLFLTArray
%typemap(jtype) (PLINT n, PLFLT* Array) jPLFLTbracket
%typemap(jstype) (PLINT n, PLFLT* Array) jPLFLTbracket
%typemap(javain) (PLINT n, PLFLT* Array) "$javainput"
%typemap(javaout) (PLINT n, PLFLT* Array) {
   return $jnicall;
}
  
/* check consistency with previous */
%typemap(in) PLFLT* ArrayCk {
   jPLFLT *jydata = (*jenv)->GetPLFLTArrayElements( jenv, $input, 0 );
   if((*jenv)->GetArrayLength( jenv, $input ) != Alen) {
      printf("Vectors must be same length.\n");
      return;
   }
   setup_array_1d_PLFLT( &$1, jydata, Alen );
   (*jenv)->ReleasePLFLTArrayElements( jenv, $input, jydata, 0 );
}
%typemap(freearg) PLFLT* ArrayCk {
   free($1);
}
%typemap(jni) PLFLT* ArrayCk jPLFLTArray
%typemap(jtype) PLFLT* ArrayCk jPLFLTbracket
%typemap(jstype) PLFLT* ArrayCk jPLFLTbracket
%typemap(javain) PLFLT* ArrayCk "$javainput"
%typemap(javaout) PLFLT* ArrayCk{
   return $jnicall;
}

/* set X length for later consistency checking */
%typemap(in) PLFLT *ArrayX {
   jPLFLT *jxdata = (*jenv)->GetPLFLTArrayElements( jenv, $input, 0 );
   Xlen = (*jenv)->GetArrayLength( jenv, $input);
   setup_array_1d_PLFLT( &$1, jxdata, Xlen);
   (*jenv)->ReleasePLFLTArrayElements( jenv, $input, jxdata, 0 );
}
%typemap(freearg) PLFLT *ArrayX {
   free($1);
}
%typemap(jni) PLFLT *ArrayX jPLFLTArray
%typemap(jtype) PLFLT *ArrayX jPLFLTbracket
%typemap(jstype) PLFLT *ArrayX jPLFLTbracket
%typemap(javain) PLFLT *ArrayX "$javainput"
%typemap(javaout) PLFLT *ArrayX {
   return $jnicall;
}

/* set Y length for later consistency checking */
%typemap(in) PLFLT *ArrayY {
   jPLFLT *jydata = (*jenv)->GetPLFLTArrayElements( jenv, $input, 0 );
   Ylen = (*jenv)->GetArrayLength( jenv, $input);
   setup_array_1d_PLFLT( &$1, jydata, Ylen);
   (*jenv)->ReleasePLFLTArrayElements( jenv, $input, jydata, 0 );
}
%typemap(freearg) PLFLT *ArrayY {
   free($1);
}
%typemap(jni) PLFLT *ArrayY jPLFLTArray
%typemap(jtype) PLFLT *ArrayY jPLFLTbracket
%typemap(jstype) PLFLT *ArrayY jPLFLTbracket
%typemap(javain) PLFLT *ArrayY "$javainput"
%typemap(javaout) PLFLT *ArrayY {
   return $jnicall;
}

/* with trailing count */
%typemap(in) (PLFLT *Array, PLINT n) {
   jPLFLT *jxdata = (*jenv)->GetPLFLTArrayElements( jenv, $input, 0 );
   $2 = (*jenv)->GetArrayLength( jenv, $input );
   setup_array_1d_PLFLT( &$1, jxdata, $2);
   (*jenv)->ReleasePLFLTArrayElements( jenv, $input, jxdata, 0 );
}
%typemap(freearg) (PLFLT *Array, PLINT n) {
   free($1);
}
%typemap(jni) (PLFLT *Array, PLINT n) jPLFLTArray
%typemap(jtype) (PLFLT *Array, PLINT n) jPLFLTbracket
%typemap(jstype) (PLFLT *Array, PLINT n) jPLFLTbracket
%typemap(javain) (PLFLT *Array, PLINT n) "$javainput"
%typemap(javaout) (PLFLT *Array, PLINT n) {
   return $jnicall;
}

/* check consistency with X dimension of previous */
%typemap(in) PLFLT* ArrayCkX {
   jPLFLT *jxdata = (*jenv)->GetPLFLTArrayElements( jenv, $input, 0 );
   if((*jenv)->GetArrayLength( jenv, $input ) != Xlen) {
      printf("Vectors must be same length.\n");
      return;
   }
   setup_array_1d_PLFLT( &$1, jxdata, Xlen );
   (*jenv)->ReleasePLFLTArrayElements( jenv, $input, jxdata, 0 );
}
%typemap(freearg) PLFLT* ArrayCkX {
   free($1);
}
%typemap(jni) PLFLT* ArrayCkX jPLFLTArray
%typemap(jtype) PLFLT* ArrayCkX jPLFLTbracket
%typemap(jstype) PLFLT* ArrayCkX jPLFLTbracket
%typemap(javain) PLFLT* ArrayCkX "$javainput"
%typemap(javaout) PLFLT* ArrayCkX{
   return $jnicall;
}

/* check consistency with Y dimension of previous */
%typemap(in) PLFLT* ArrayCkY {
   jPLFLT *jydata = (*jenv)->GetPLFLTArrayElements( jenv, $input, 0 );
   if((*jenv)->GetArrayLength( jenv, $input ) != Ylen) {
      printf("Vectors must be same length.\n");
      return;
   }
   setup_array_1d_PLFLT( &$1, jydata, Ylen );
   (*jenv)->ReleasePLFLTArrayElements( jenv, $input, jydata, 0 );
}
%typemap(freearg) PLFLT* ArrayCkY {
   free($1);
}
%typemap(jni) PLFLT* ArrayCkY jPLFLTArray
%typemap(jtype) PLFLT* ArrayCkY jPLFLTbracket
%typemap(jstype) PLFLT* ArrayCkY jPLFLTbracket
%typemap(javain) PLFLT* ArrayCkY "$javainput"
%typemap(javaout) PLFLT* ArrayCkY{
   return $jnicall;
}

/* 2D array with trailing dimensions, check consistency with previous */
%typemap(in) (PLFLT **MatrixCk, PLINT nx, PLINT ny) {
   jPLFLT **adat;
   jobject *ai;
   int nx = (*jenv)->GetArrayLength( jenv, $input );
   int ny = -1;
   int i, j;
   ai = (jobject *) malloc( nx * sizeof(jobject) );
   adat = (jPLFLT **) malloc( nx * sizeof(jPLFLT *) );

   for( i=0; i < nx; i++ )
     {
	ai[i] = (*jenv)->GetObjectArrayElement( jenv, $input, i );
	adat[i] = (*jenv)->GetPLFLTArrayElements( jenv, ai[i], 0 );

	if (ny == -1)
	  ny = (*jenv)->GetArrayLength( jenv, ai[i] );
	else if (ny != (*jenv)->GetArrayLength( jenv, ai[i] )) {
	   printf( "Misshapen a array.\n" );
	   for( j=0; j <= i; j++ )
	     (*jenv)->ReleasePLFLTArrayElements( jenv, ai[j], adat[j], 0 );
	   free(ai);
	   free(adat);
	   return;
	}
     }

   if( nx != Xlen || ny != Ylen ) {
      printf( "Vectors must match matrix.\n" );
      for( i=0; i < nx; i++ )
	(*jenv)->ReleasePLFLTArrayElements( jenv, ai[i], adat[i], 0 );
      free(ai);
      free(adat);
      return;
   }
   setup_array_2d_PLFLT( &$1, adat, nx, ny );
   $2 = nx;
   $3 = ny;
   for( i=0; i < nx; i++ )
     (*jenv)->ReleasePLFLTArrayElements( jenv, ai[i], adat[i], 0 );

   free(ai);
   free(adat);

}
%typemap(freearg) (PLFLT **MatrixCk, PLINT nx, PLINT ny) {
   free($1[0]);
   free($1);
}
%typemap(jni) (PLFLT **MatrixCk, PLINT nx, PLINT ny) "jobjectArray"
%typemap(jtype) (PLFLT **MatrixCk, PLINT nx, PLINT ny) jPLFLTbracket2
%typemap(jstype) (PLFLT **MatrixCk, PLINT nx, PLINT ny) jPLFLTbracket2
%typemap(javain) (PLFLT **MatrixCk, PLINT nx, PLINT ny) "$javainput"
%typemap(javaout) (PLFLT **MatrixCk, PLINT nx, PLINT ny) {
   return $jnicall;
}

//temporary
#if 0
/* 2D array with trailing dimensions, set the X, Y size for later checking */
%typemap(in) (PLFLT **Matrix, PLINT nx, PLINT ny) (PyArrayObject* tmp) {
  int i, size;
  tmp = (PyArrayObject *)myArray_ContiguousFromObject($input, PyArray_PLFLT, 2, 2);
  if(tmp == NULL) return NULL;
  Xlen = $2 = tmp->dimensions[0];
  Ylen = $3 = tmp->dimensions[1];
  size = sizeof(PLFLT)*$3;
  $1 = (PLFLT**)malloc(sizeof(PLFLT*)*$2);
  for(i=0; i<$2; i++)
    $1[i] = (PLFLT*)(tmp->data + i*size);
}
%typemap(freearg) (PLFLT **Matrix, PLINT nx, PLINT ny) {
  Py_DECREF(tmp$argnum);
  free($1);
}

/* for plshade1, note the difference in the type for the first arg */
%typemap(in) (PLFLT *Matrix, PLINT nx, PLINT ny) (PyArrayObject* tmp) {
  tmp = (PyArrayObject *)myArray_ContiguousFromObject($input, PyArray_PLFLT, 2, 2);
  if(tmp == NULL) return NULL;
  Xlen = $2 = tmp->dimensions[0];
  Ylen = $3 = tmp->dimensions[1];
  $1 = (PLFLT*)tmp->data;
}
%typemap(freearg) (PLFLT *Matrix, PLINT nx, PLINT ny) {
  Py_DECREF(tmp$argnum);
}

/* 2D array, check for consistency */
%typemap(in) PLFLT **MatrixCk (PyArrayObject* tmp) {
  int i, size;
  tmp = (PyArrayObject *)myArray_ContiguousFromObject($input, PyArray_PLFLT, 2, 2);
  if(tmp == NULL) return NULL;
  if(tmp->dimensions[0] != Xlen || tmp->dimensions[1] != Ylen) {
    PyErr_SetString(PyExc_ValueError, "Vectors must match matrix.");
    return NULL;
  }
  size = sizeof(PLFLT)*Ylen;
  $1 = (PLFLT**)malloc(sizeof(PLFLT*)*Xlen);
  for(i=0; i<Xlen; i++)
    $1[i] = (PLFLT*)(tmp->data + i*size);
}
%typemap(freearg) PLFLT **MatrixCk {
  Py_DECREF(tmp$argnum);
  free($1);
}
/***************************
	String returning functions
****************************/

/* This currently just used for plgdev, plgfnam, and plgver which apparently
 * have a limit of 80 bytes.  But to (hopefully) be safe for any future use
 * have a 1000 byte limit here. */
%typemap(ignore) char* OUTPUT ( char buff[1000] ) {
  $1 = buff;
}
%typemap(argout) char* OUTPUT {
  $result = $1;
}
#endif

/* swig compatible PLplot API definitions from here on. */
%include plplotcapi.i

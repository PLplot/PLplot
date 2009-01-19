/*
Copyright (C) 2009  Werner Smekal
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
A SWIG interface to PLplot for LUA. This wrapper does the following:

   1) it strictly provides the C-API with the usual change of not
      requiring lengths for arrays,

   2) it attempts to provide the entire API *excluding* callbacks for
      plcont and plshade(s) (for now).

   3) it works only with the double-precision version of the
      PLplot library.

This is known to work with swig-1.3.36.

*/
%module plplotluac

%{
#include "plplotP.h"
%}

/* type definitions */
typedef double PLFLT;
typedef int PLINT;
typedef unsigned int PLUNICODE;
typedef PLINT PLBOOL;

/* Assign the module a shorter name */
%luacode {
  pl=plplotluac
}

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

/* typemaps */
%include <typemaps.i>

/**********************************************************************************
			 PLINT arrays
**********************************************************************************/

/* With preceding count */
%typemap(in) (PLINT n, PLINT *Array) {
  $2 = (PLINT*)SWIG_get_int_num_array_var(L, $input, &$1);
  if(!$2) SWIG_fail;
  Alen = $1;
}
%typemap(freearg) (PLINT n, PLINT *Array) { SWIG_FREE_ARRAY($2); }


/* Trailing count and check consistency with previous */
%typemap(in) (PLINT *ArrayCk, PLINT n) (int temp) {
  $1 = (PLINT*)SWIG_get_int_num_array_var(L, $input, &temp);
  if(!$1) SWIG_fail;
  if(temp != Alen) {
    lua_pushfstring(L, "Tables must be of same length.");
    SWIG_fail;
  }
  $2 = temp;
}
%typemap(freearg) (PLINT *ArrayCk, PLINT n) { SWIG_FREE_ARRAY($1); }


/* No count but check consistency with previous */
%typemap(in) PLINT *ArrayCk (int temp) {
  $1 = (PLINT*)SWIG_get_int_num_array_var(L, $input, &temp);
  if(!$1) SWIG_fail;
  if(temp != Alen) {
    lua_pushfstring(L, "Tables must be of same length.");
    SWIG_fail;
  }
}
%typemap(freearg) PLINT *ArrayCk { SWIG_FREE_ARRAY($1); }


/* Weird case to allow argument to be one shorter than others */
%typemap(in) PLINT *ArrayCkMinus1 (int temp) {
  $1 = (PLINT*)SWIG_get_int_num_array_var(L, $input, &temp);
  if(!$1) SWIG_fail;
  if(temp < Alen-1) {
    lua_pushfstring(L, "Tables must be at least length of others minus 1.");
    SWIG_fail;
  }
}
%typemap(freearg) PLINT *ArrayCkMinus1 { SWIG_FREE_ARRAY($1); }

%typemap(in) PLINT *ArrayCkMinus1Null (int temp) {
  $1 = (PLINT*)SWIG_get_int_num_array_var(L, $input, &temp);
  if(!$1) SWIG_fail;
  if(temp < Alen-1) {
    lua_pushfstring(L, "Tables must be at least length of others minus 1.");
    SWIG_fail;
  }
}
%typemap(freearg) PLINT *ArrayCkMinus1Null { SWIG_FREE_ARRAY($1); }


/* No length but remember size to check others */
%typemap(in) PLINT *Array (int temp) {
  $1 = (PLINT*)SWIG_get_int_num_array_var(L, $input, &temp);
  if(!$1) SWIG_fail;
  Alen = temp;
}
%typemap(freearg) (PLINT *Array) { SWIG_FREE_ARRAY($1); }


/******************************************************************************
				 PLFLT Arrays 
******************************************************************************/

/* with preceding count */
%typemap(in) (PLINT n, PLFLT *Array) {
  $2 = (PLFLT*)SWIG_get_double_num_array_var(L, $input, &$1);
  if(!$2) SWIG_fail;
  Alen = $1;
}
%typemap(freearg) (PLINT n, PLFLT *Array) { SWIG_FREE_ARRAY($2); }


/* Trailing count and check consistency with previous */
%typemap(in) (PLFLT *ArrayCk, PLINT n) (int temp) {
  $1 = (PLFLT*)SWIG_get_double_num_array_var(L, $input, &temp);
  if(!$1) SWIG_fail;
  if(temp != Alen) {
    lua_pushfstring(L, "Tables must be of same length.");
    SWIG_fail;
  }
  $2 = temp;
}
%typemap(freearg) (PLFLT *ArrayCk, PLINT n) { SWIG_FREE_ARRAY($1); }


/* no count, but check consistency with previous */
%typemap(in) PLFLT *ArrayCk (int temp) {
  $1 = (PLFLT*)SWIG_get_double_num_array_var(L, $input, &temp);
  if(!$1) SWIG_fail;
  if(temp != Alen) {
    lua_pushfstring(L, "Tables must be of same length.");
    SWIG_fail;
  }
}
%typemap(freearg) PLFLT *ArrayCk { SWIG_FREE_ARRAY($1); }


/* No length but remember size to check others */
%typemap(in) PLFLT *Array (int temp) {
  $1 = (PLFLT*)SWIG_get_double_num_array_var(L, $input, &temp);
  if(!$1) SWIG_fail;
  Alen = temp;
}
%typemap(freearg) (PLFLT *Array) { SWIG_FREE_ARRAY($1); }


/******************************************************************************
				 Function calls
******************************************************************************/

/* lua functions 
%include <lua_fnptr.i>

%{
void mapform_fn(PLINT n, PLFLT* x, PLFLT* y)
{
  SWIGLUA_FN_GET(fn);
  lua_pushnumber(fn.L, n);
  SWIG_write_double_num_array(L, x, n)
  SWIG_write_double_num_array(L, y, n)
  lua_call(fn.L,3,0);    // 2 in, 1 out
}
%} */

typedef PLINT (*defined_func)(PLFLT, PLFLT);
typedef void (*fill_func)(PLINT, PLFLT*, PLFLT*);
typedef void (*pltr_func)(PLFLT, PLFLT, PLFLT *, PLFLT*, PLPointer);
typedef void (*mapform_func)(PLINT, PLFLT *, PLFLT*);
typedef PLFLT (*f2eval_func)(PLINT, PLINT, PLPointer);

%{
typedef PLINT (*defined_func)(PLFLT, PLFLT);
typedef void (*fill_func)(PLINT, PLFLT*, PLFLT*);
typedef void (*pltr_func)(PLFLT, PLFLT, PLFLT *, PLFLT*, PLPointer);
typedef void (*mapform_func)(PLINT, PLFLT *, PLFLT*);
typedef PLFLT (*f2eval_func)(PLINT, PLINT, PLPointer);
%}

%typemap(in, numinputs=0) defined_func df {
  $1 = NULL;
}

%typemap(in, numinputs=0) fill_func ff {
  $1 = plfill;
}

/* Process options list using current options info. */
%typemap(in) (int *p_argc, const char **argv) {
  int i, n;

  if(!lua_istable(L, $input)) {
    lua_pushstring(L,"expected a table");
    SWIG_fail;
  }
  
  lua_pushstring(L, "n");
  lua_gettable(L, $input);
  if(!lua_isnumber(L, -1)) {
    lua_pushfstring(L, "Table doesn't contain key 'n'.");
    SWIG_fail;
  }
  n = (int)lua_tonumber(L, -1);
  lua_pop(L, 1);  /* remove number */
  n=n+1; /* since lua only counts the options */
  printf("n=%d\n", n);
  $1 = &n;
  
  $2 = SWIG_ALLOC_ARRAY(char*, n+1);

  for(i = 0; i < n; i++) {
    lua_rawgeti(L, $input, i);
    if(lua_isstring(L, -1)) {
      $2[i] = (char*)lua_tostring(L, -1);
      printf("argv[%d]=%s\n", i, $2[i]);
    } else {
      lua_pop(L,1);
      lua_pushfstring(L, "List items must be strings");
      SWIG_fail;
      /* $2 array is freed after 'fail:' */
    }
    lua_pop(L,1);
  }
  puts("here");
  $2[n] = NULL;
}
%typemap(freearg) (int *p_argc, const char **argv) {
  SWIG_FREE_ARRAY($2);
}
%typemap(in,checkfn="lua_isnumber") PLINT mode {
  $1 = ((int)lua_tonumber(L,$input)) | PL_PARSE_NODELETE;
}


%ignore plshades;
%ignore plshade;
%ignore plshade1;
%ignore plvect;
%ignore plfcont;
%ignore plfshade;
%ignore plimagefr;
%ignore plcont;

/******************************************************************************
				 Renames
******************************************************************************/
%rename(parseopts) plparseopts;

/* swig compatible PLplot API definitions from here on. */
%include plplotcapi.i

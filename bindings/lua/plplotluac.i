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
  int temp;
  $2 = (PLFLT*)SWIG_get_double_num_array_var(L, $input, &temp);
  if(!$2) SWIG_fail;
  $1 = Alen = temp;
}
%typemap(freearg) (PLINT n, PLFLT *Array) { SWIG_FREE_ARRAY($2); }


/* Trailing count and check consistency with previous */
%typemap(in) (PLFLT *ArrayCk, PLINT n) {
  int temp;
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
%typemap(in) PLFLT *Array {
  int temp;
  $1 = (PLFLT*)SWIG_get_double_num_array_var(L, $input, &temp);
  if(!$1) SWIG_fail;
  Alen = temp;
}
%typemap(freearg) (PLFLT *Array) { SWIG_FREE_ARRAY($1); }


/* with trailing count */
%typemap(in) (PLFLT *Array, PLINT n) {
  int temp;
  $1 = (PLFLT*)SWIG_get_double_num_array_var(L, $input, &temp);
  if(!$1) SWIG_fail;
  $2 = Alen = temp;
}
%typemap(freearg) (PLFLT *Array, PLINT n) { SWIG_FREE_ARRAY($1); }


/* check consistency with X dimension of previous */
%typemap(in) PLFLT *ArrayCkX {
  int temp;
  $1 = (PLFLT*)SWIG_get_double_num_array_var(L, $input, &temp);
  if(!$1) SWIG_fail;
  if(temp != Xlen) {
    lua_pushfstring(L, "Tables must be of same length.");
    SWIG_fail;
  }
}
%typemap(freearg) PLFLT *ArrayCkX { SWIG_FREE_ARRAY($1); }


/* check consistency with Y dimension of previous */
%typemap(in) PLFLT *ArrayCkY {
  int temp;
  $1 = (PLFLT*)SWIG_get_double_num_array_var(L, $input, &temp);
  if(!$1) SWIG_fail;
  if(temp != Ylen) {
    lua_pushfstring(L, "Tables must be of same length.");
    SWIG_fail;
  }
}
%typemap(freearg) PLFLT *ArrayCkY { SWIG_FREE_ARRAY($1); }


/* set X length for later consistency checking, with trailing count */
%typemap(in) (PLFLT *ArrayX, PLINT nx) {
  int temp;
  $1 = (PLFLT*)SWIG_get_double_num_array_var(L, $input, &temp);
  if(!$1) SWIG_fail;
  $2 = Xlen = temp;
}
%typemap(freearg) (PLFLT *ArrayX, PLINT nx) { SWIG_FREE_ARRAY($1); }


/* set X length for later consistency checking */
%typemap(in) PLFLT *ArrayX {
  int temp;
  $1 = (PLFLT*)SWIG_get_double_num_array_var(L, $input, &temp);
  if(!$1) SWIG_fail;
  Xlen = temp;
}
%typemap(freearg) PLFLT *ArrayX { SWIG_FREE_ARRAY($1); }


/* Set Y length for later consistency checking, with trailing count */
%typemap(in) (PLFLT *ArrayY, PLINT ny) {
  int temp;
  $1 = (PLFLT*)SWIG_get_double_num_array_var(L, $input, &temp);
  if(!$1) SWIG_fail;
  $2 = Ylen = temp;
}
%typemap(freearg) (PLFLT *ArrayY, PLINT ny) { SWIG_FREE_ARRAY($1); }


/* set Y length for later consistency checking */
%typemap(in) PLFLT *ArrayY {
  int temp;
  $1 = (PLFLT*)SWIG_get_double_num_array_var(L, $input, &temp);
  if(!$1) SWIG_fail;
  Ylen = temp;
}
%typemap(freearg) (PLFLT *ArrayY) { SWIG_FREE_ARRAY($1); }


/* 2D array with trailing dimensions, check consistency with previous */
%typemap(in) (PLFLT **MatrixCk, PLINT nx, PLINT ny) {
}
%typemap(freearg) (PLFLT **MatrixCk, PLINT nx, PLINT ny) {
}


/* 2D array with trailing dimensions, set the X, Y size for later checking */
%typemap(in) (PLFLT **Matrix, PLINT nx, PLINT ny) {
}
%typemap(freearg) (PLFLT **Matrix, PLINT nx, PLINT ny) {
}


/* 2D array with no dimensions, set the X, Y size for later checking */
%typemap(in) PLFLT **Matrix {
}
%typemap(freearg) PLFLT **Matrix {
}


/* for plshade1, note the difference in the type for the first arg */
%typemap(in) (PLFLT *Matrix, PLINT nx, PLINT ny) {
}
%typemap(freearg) (PLFLT *Matrix, PLINT nx, PLINT ny) {
}

/* 2D array, check for consistency */
%typemap(in) PLFLT **MatrixCk {
}
%typemap(freearg) PLFLT **MatrixCk {
}

/* 2D array, check for consistency input / output version */
%typemap(in) PLFLT **OutMatrixCk {
}
%typemap(freearg) PLFLT **OutMatrixCk {
}


/******************************************************************************
				 String returning functions
******************************************************************************/

/* This currently just used for plgdev, plgfnam, and plgver which apparently
 * have a limit of 80 bytes.  But to (hopefully) be safe for any future use
 * have a 1000 byte limit here. */
%typemap(in, numinputs=0) char *OUTPUT ( char buff[1000] ) {
  $1 = buff;
}
/*%typemap(argout,fragment="t_output_helper") char *OUTPUT {
   PyObject *o = PyString_FromString($1);
   $result = t_output_helper($result,o);
}*/


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
%typemap(in, checkfn="lua_istable") (int *p_argc, const char **argv) {
  int i, n;

  lua_pushstring(L, "n");
  lua_gettable(L, $input);
  if(!lua_isnumber(L, -1)) {
    lua_pushfstring(L, "Table doesn't contain key 'n'.");
    SWIG_fail;
  }
  n = (int)lua_tonumber(L, -1);
  lua_pop(L, 1);  /* remove number */
  n=n+1; /* since lua only counts the options */
  $1 = &n;
  
  $2 = SWIG_ALLOC_ARRAY(char*, (n+1));

  for(i = 0; i < n; i++) {
    lua_rawgeti(L, $input, i);
    if(lua_isstring(L, -1)) {
      $2[i] = (char*)lua_tostring(L, -1);
    } else {
      lua_pop(L,1);
      lua_pushfstring(L, "List items must be strings");
      SWIG_fail;
      /* $2 array is freed after 'fail:' */
    }
    lua_pop(L,1);
  }
  $2[n] = NULL;
}
%typemap(freearg) (int *p_argc, const char **argv) {
  SWIG_FREE_ARRAY($2);
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
%rename(adv) pladv;
%rename(axes) plaxes;
%rename(bin) plbin;
%rename(bop) plbop;
%rename(box) plbox;
%rename(box3) plbox3;
%rename(calworld) plcalworld;
%rename(clear) plclear;
%rename(col0) plcol0;
%rename(col1) plcol1;
%rename(cont) plcont;
%rename(cpstrm) plcpstrm;
%rename(plend) plend;
%rename(end1) plend1;
%rename(env) plenv;
%rename(env0) plenv0;
%rename(eop) pleop;
%rename(errx) plerrx;
%rename(erry) plerry;
%rename(famadv) plfamadv;
%rename(fill) plfill;
%rename(fill3) plfill3;
%rename(flush) plflush;
%rename(font) plfont;
%rename(fontld) plfontld;
%rename(gchr) plgchr;
%rename(gcol0) plgcol0;
%rename(gcolbg) plgcolbg;
%rename(gcol0a) plgcol0a;
%rename(gcolbga) plgcolbga;
%rename(gcompression) plgcompression;
%rename(gdev) plgdev;
%rename(gdidev) plgdidev;
%rename(gdiori) plgdiori;
%rename(gdiplt) plgdiplt;
%rename(gfam) plgfam;
%rename(gfci) plgfci;
%rename(gfnam) plgfnam;
%rename(gfont) plgfont;
%rename(glevel) plglevel;
%rename(gpage) plgpage;
%rename(gra) plgra;
%rename(griddata) plgriddata;
%rename(gspa) plgspa;
%rename(gstrm) plgstrm;
%rename(gver) plgver;
%rename(gvpd) plgvpd;
%rename(gvpw) plgvpw;
%rename(gxax) plgxax;
%rename(gyax) plgyax;
%rename(gzax) plgzax;
%rename(hist) plhist;
%rename(hls) plhls;
%rename(hlsrgb) plhlsrgb;
%rename(image) plimage;
%rename(imagefr) plimagefr;
%rename(init) plinit;
%rename(join) pljoin;
%rename(lab) pllab;
%rename(lightsource) pllightsource;
%rename(line) plline;
%rename(line3) plline3;
%rename(lsty) pllsty;
%rename(map) plmap;
%rename(meridians) plmeridians;
%rename(mesh) plmesh;
%rename(meshc) plmeshc;
%rename(mkstrm) plmkstrm;
%rename(mtex) plmtex;
%rename(mtex3) plmtex3;
%rename(ot3d) plot3d;
%rename(ot3dc) plot3dc;
%rename(ot3dcl) plot3dcl;
%rename(parseopts) plparseopts;
%rename(pat) plpat;
%rename(poin) plpoin;
%rename(poin3) plpoin3;
%rename(poly3) plpoly3;
%rename(prec) plprec;
%rename(psty) plpsty;
%rename(ptex) plptex;
%rename(ptex3) plptex3;
%rename(randd) plrandd;
%rename(replot) plreplot;
%rename(rgb) plrgb;
%rename(rgb1) plrgb1;
%rename(rgbhls) plrgbhls;
%rename(schr) plschr;
%rename(scmap0) plscmap0;
%rename(scmap0a) plscmap0a;
%rename(scmap0n) plscmap0n;
%rename(scmap1) plscmap1;
%rename(scmap1a) plscmap1a;
%rename(scmap1l) plscmap1l;
%rename(scmap1la) plscmap1la;
%rename(scmap1n) plscmap1n;
%rename(scol0) plscol0;
%rename(scol0a) plscol0a;
%rename(scolbg) plscolbg;
%rename(scolbga) plscolbga;
%rename(scolor) plscolor;
%rename(scompression) plscompression;
%rename(sdev) plsdev;
%rename(sdidev) plsdidev;
%rename(sdimap) plsdimap;
%rename(sdiori) plsdiori;
%rename(sdiplt) plsdiplt;
%rename(sdiplz) plsdiplz;
%rename(seed) plseed;
%rename(sesc) plsesc;
%rename(setopt) plsetopt;
%rename(sfam) plsfam;
%rename(sfci) plsfci;
%rename(sfnam) plsfnam;
%rename(sfont) plsfont;
%rename(shade) plshade;
%rename(shade1) plshade1;
%rename(shades) plshades;
%rename(smaj) plsmaj;
%rename(smem) plsmem;
%rename(smin) plsmin;
%rename(sori) plsori;
%rename(spage) plspage;
%rename(spause) plspause;
%rename(sstrm) plsstrm;
%rename(ssub) plssub;
%rename(ssym) plssym;
%rename(star) plstar;
%rename(start) plstart;
%rename(stripa) plstripa;
%rename(stripc) plstripc;
%rename(stripd) plstripd;
%rename(styl) plstyl;
%rename(surf3d) plsurf3d;
%rename(surf3dl) plsurf3dl;
%rename(svect) plsvect;
%rename(svpa) plsvpa;
%rename(sxax) plsxax;
%rename(syax) plsyax;
%rename(sym) plsym;
%rename(szax) plszax;
%rename(text) pltext;
%rename(timefmt) pltimefmt;
%rename(vasp) plvasp;
%rename(vect) plvect;
%rename(vpas) plvpas;
%rename(vpor) plvpor;
%rename(vsta) plvsta;
%rename(w3d) plw3d;
%rename(wid) plwid;
%rename(wind) plwind;
%rename(xormod) plxormod;

/* swig compatible PLplot API definitions from here on. */
%include plplotcapi.i

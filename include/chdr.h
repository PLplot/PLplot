/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:45:28  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:33:37  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*--------------------------------------------------------------------------*\

  Header file for C programs.
  Maurice LeBrun, IFS, University of Texas, Sep 16, 1991.

  This file sets up macros and typedefs to make C program development more
  portable and predictable.  It is public domain; do what you want with it.

  Notes:

  - specify -DDOUBLE_PREC to get doubles instead of floats.

  - specify -DNO_STDC to disable Standard C style declarations.

  - some C compilers (such as VAXC) barf on whitespace before preprocessor
  statements, so #define's inside ifdef's are not indented.

\*--------------------------------------------------------------------------*/

#ifndef INCLUDED_CHDR
#define INCLUDED_CHDR

/* Float and int types */

#ifdef DOUBLE_PREC
   typedef double FLOAT;
#else
   typedef float FLOAT;
#endif

typedef long INT;

/* Signed char type, in case we ever need it. */

#ifdef VAXC
#define NO_SIGNED_CHAR
#endif

#ifdef HPUX
#define NO_SIGNED_CHAR
#endif

#ifdef sun
#define NO_SIGNED_CHAR
#endif

#ifdef NO_SIGNED_CHAR
   typedef char        SCHAR;
#else
   typedef signed char SCHAR;
#endif

/* Some unsigned types */

#ifndef U_CHAR
#define U_CHAR unsigned char
#endif

#ifndef U_SHORT
#define U_SHORT unsigned short
#endif

#ifndef U_INT
#define U_INT unsigned int
#endif

#ifndef U_LONG
#define U_LONG unsigned long
#endif

/* Some systems need the "b" flag when opening binary files.
   Other systems will choke on it, hence the need for this silliness.
*/

#ifdef MSDOS
#define BINARY_FLAG
#endif

#ifdef GNU386
#define BINARY_FLAG
#endif

#ifdef BINARY_FLAG
#define BINARY_WRITE "wb+"
#define BINARY_READ "rb"
#else
#define BINARY_WRITE "w+"
#define BINARY_READ "r"
#endif

/* Utility macros */

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

#ifndef MAX
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
#endif
#ifndef ABS
#define ABS(a)      ((a)<0 ? -(a) : (a))
#endif
#ifndef ROUND
#define ROUND(a)    (INT)((a)<0. ? ((a)-.5) : ((a)+.5))
#endif
#ifndef BETW
#define BETW(ix,ia,ib)  (((ix)<=(ia)&&(ix)>=(ib)) || ((ix)>=(ia)&&(ix)<=(ib)))
#endif
#ifndef SSQR
#define SSQR(a,b)       sqrt((a)*(a)+(b)*(b))
#endif
#ifndef SIGN
#define SIGN(a)         ((a)<0 ? -1 : 1)
#endif

#define UNDEFINED -9999999

/*----------------------------------------------------------------------*\
   Standard C -vs- K&R tomfoolery

   The USE_STDC flag is used to switch between Standard C (ANSI C) type
   declarations or K&R style.  This will be defined if __STDC__ is defined,
   but not if NO_STDC is defined (a kludge for when the compiler doesn't 
   live up to its claims).

   If USE_STDC is defined,     full ANSI compliance assumed.
   If USE_STDC is not defined, full K&R  compliance assumed.

   THERE IS NO MIDDLE GROUND!  

   If you choose Standard C, then a float argument must be cast as a float, 
   so that automatic promotion to double does not occur (if using K&R, the
   promotion to double will occur regardless of the cast).  Therefore, it is
   highly encouraged to include plplot.h in your main program to get the
   prototypes right, and cast float arguments to FLOAT (but not pointers 
   to float, of course).

   Eventually there will be no more strictly K&R compilers and we can
   forget about all this mess.
\*----------------------------------------------------------------------*/

#ifndef USE_STDC

#ifdef MSDOS
#define USE_STDC             1
#endif

#ifdef __STDC__
#define USE_STDC             1
#endif

#ifdef NO_STDC
#ifdef USE_STDC
#undef USE_STDC
#endif
#endif

#endif

/* ANSI vs non-ANSI */

#ifdef USE_STDC
#ifndef PROTO
#define PROTO(a)  a		/* Use function prototypes. */
#endif
typedef void        VOID;

#else
#ifndef PROTO
#define PROTO(a)  ()		/* Turn off function prototypes. */
#endif
typedef char        VOID;
#endif

/* Get REAL, buddy !!! */

/* We're gonna track down all this garbage and throw it away! */

#define PROTO a;lskdjfka;lskdfjka;lskdfja;lsdkfja;dslkfjasdf;lkj$$##%^$

#endif	/* INCLUDED_CHDR */

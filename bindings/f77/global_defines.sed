# sed script specifically designed to parse the #define statements in
# bindings/swig-support/plplotcapi.i into a file that can be used for
# global variable definitions for f77.

# Drop every line that is not a #define
/^#define/! d

# Drop specific #defines from the file that are not available
# because of specific #if statements.
/^#define ZEROW/ d
/^#define ONEW/ d
/OBJECT_DATA/ d
/^#define pltr_img/ d

# Translate trailing comment identifier into fortran 95 trailing comment.
/^#define/ s?//?!?
# Transform octal constants to Fortran (BOZ) form.
/^#define/ s? 0\([0-9][0-9]*\)? o'\1'?
# Transform hexadecimal constants to Fortran (BOZ) form.
/^#define/ s? 0[xX]\([0-9a-fA-F][0-9a-fA-F]*\)? z'\1'?
# Handle special case of z'80000000' which generates integer overflow
# for gfortran (and probably other Fortran 95 compilers).
/^#define/ s? z'80000000'? ishft(1,31)?

# Parse what is left while preserving trailing comments.  Watch out
# for parentheses around value as well.
# Treat PL_NOTSET as a floating value and append special parameter lines for
# PI and TWOPI in this case as well.

# N.B. These double-precision results are transformed later to single precision
# for the (uncommon) case where PLplot is a single-precision library.
/^#define/ s?^#define *\(PL_NOTSET\)[ (]*\([^ ]*\)[ )]*\(.*\)$?      real*8 \1 \3\n      parameter (\1 = \2.0d0)\n      real*8 PI\n      parameter (PI = 3.1415926535897932384d0)\n      real*8 TWOPI\n      parameter (TWOPI = 2.0d0*PI)?

/^#define/ s?^#define *\([^ ]*\)[ (]*\([^ ]*\)[ )]*\(.*\)$?      integer*4 \1 \3\n      parameter (\1 = \2)?

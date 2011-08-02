# sed script specifically designed to parse the #define statements in
# bindings/swig-support/plplotcapi.i into a file that can be used for
# global variable definitions for f95.

# Insert text at the start:
1 i\
!     Do not edit this generated file.  Instead, check its consistency\
!     with the #defines in bindings/swig-support/plplotcapi.i using the\
!     (Unix) target "check_f95_parameters".  If that target reports an\
!     inconsistency (via a cmp message) between the generated\
!     plplot_parameters.h_compare file in the build tree and\
!     plplot_parameters.h in the source tree, then copy\
!     plplot_parameters.h_compare on top of plplot_parameters.h and\
!     check in that result.\

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
# PL_PI and PL_TWOPI in this case as well.

/^#define/ s?^#define *\(PL_NOTSET\)[ (]*\([^ ]*\)[ )]*\(.*\)$?      real(kind=plflt), parameter :: \1 = \2.0_plflt\3\n      real(kind=plflt), parameter :: PL_PI = 3.1415926535897932384_plflt\n      real(kind=plflt), parameter :: PL_TWOPI = 2.0_plflt*PL_PI?

/^#define/ s?^#define *\([^ ]*\)[ (]*\([^ ]*\)[ )]*\(.*\)$?      integer, parameter :: \1 = \2 \3?

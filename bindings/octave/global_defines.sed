# sed script specifically designed to parse the #define statements in
# bindings/swig-support/plplotcapi.i into a file that can be used for
# global variable definitions for octave.

# Drop every line that is not a #define
/^#define/! d

# Drop specific #defines from the file that are not available to swig-generated
# octave because of specific #if statements.
/^#define ZEROW/ d
/^#define ONEW/ d
/OBJECT_DATA/ d
/^#define pltr_img/ d

# First translate comment identifier into octave.
/^#define/ s?//?#?
# Parse what is left while preserving trailing comments.  Watch out
# for parentheses around value as well.
/^#define/ s?^#define *\([^ ]*\)[ (]*\([^ ]*\)[ )]*\(.*\)$?global \1\n\1 = \2; \3?

# This sed command script converts a fortran file from double to single
# precision.  A number of simplifying assumptions are made.
# * The file is all in lower case
# * No implicit statements other than implicit none
# * No "double precision" statements
# * (This is the biggy) all floating-point numbers in standard form consisting
#   of one or more digits, a decimal point, zero or more digits, "d", optional
#   sign, 1 or more digits.
# N.B. the example *.fm4 files in this directory have been carefully
# maintained to satisfy these conditions, and if this maintainence fails,
# the single-precision (and possibly the double-precision form as well) will
# no longer work.

# Convert real*8
s/real\*8/real*4/
# Convert dble
s/dble/real/g
# Convert numbers in above standard form.
s/\([0-9]\+\.[0-9]*\)d\([+-][0-9]\+\)/\1e\2/g
s/\([0-9]\+\.[0-9]*\)d\([0-9]\+\)/\1e\2/g
		  

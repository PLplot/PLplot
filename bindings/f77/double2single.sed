# This sed command script converts sfstubs.fm4 from double to single
# precision.  Currently the only double-precision construct that file
# has in it is "real*8" so that is all this script converts.  See
# ../../examples/f77/double2single.sed for an example of a more complicated
# case.

# Convert real*8
s/real\*8/real*4/
		  

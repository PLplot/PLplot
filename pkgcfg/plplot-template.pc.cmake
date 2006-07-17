libdir=@LIB_DIR@
includedir=@INCLUDE_DIR@
drvdir=@LIB_DIR@/plplot@VERSION@/drivers@LIB_TAG@

Name: PLplot @PC_SHORT_NAME@
Description: Scientific plotting library (@PC_LONG_NAME@@PC_PRECISION@ precision)
Requires: @PC_REQUIRES@
Version: @VERSION@
Libs: -L${libdir} @PC_LINK_FLAGS@
Cflags: -I${includedir} @PC_COMPILE_FLAGS@ 

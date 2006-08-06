# - Find AQT
# Find the aquaterm includes and library
# This module defines
#  AQT_INCLUDE_DIR, where to find AQTAdapter.h, etc.
#  AQT_LIBRARY, the libraries needed to use aquaterm.
#  AQT_FOUND, If false, do not try to use aquaterm.

# Look for aqt libraries

FIND_PATH(AQT_INCLUDE_DIR AQTAdapter.h
	/usr/local/include/aquaterm
	/usr/include/aquaterm
	/sw/include/aquaterm
)

FIND_LIBRARY(AQT_LIBRARY
  	NAMES aquaterm
  	PATHS /usr/lib /usr/local/lib /sw/lib
)

IF (AQT_LIBRARY AND AQT_INCLUDE_DIR)
    SET(AQT_FOUND "YES")
ELSE (AQT_LIBRARY AND AQT_INCLUDE_DIR)
	SET(AQT_FOUND "NO")
ENDIF (AQT_LIBRARY AND AQT_INCLUDE_DIR)

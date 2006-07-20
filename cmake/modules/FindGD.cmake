# - Find GD
# Find the native GD includes and library
# This module defines
#  GD_INCLUDE_DIR, where to find jpeglib.h, etc.
#  GD_LIBRARIES, the libraries needed to use GD.
#  GD_FOUND, If false, do not try to use GD.
# also defined, but not for general use are
#  GD_LIBRARY, where to find the GD library.

FIND_PATH(GD_INCLUDE_DIR gd.h
/usr/local/include
/usr/include
)

SET(GD_NAMES ${GD_NAMES} gd)
FIND_LIBRARY(GD_LIBRARY
  NAMES ${GD_NAMES}
  PATHS /usr/lib /usr/local/lib
  )

IF (GD_LIBRARY AND GD_INCLUDE_DIR)
    SET(GD_LIBRARIES ${GD_LIBRARY})
    SET(GD_FOUND "YES")
ELSE (GD_LIBRARY AND GD_INCLUDE_DIR)
  SET(GD_FOUND "NO")
ENDIF (GD_LIBRARY AND GD_INCLUDE_DIR)


IF (GD_FOUND)
   IF (NOT GD_FIND_QUIETLY)
      MESSAGE(STATUS "Found GD: ${GD_LIBRARIES}")
   ENDIF (NOT GD_FIND_QUIETLY)
ELSE (GD_FOUND)
   IF (GD_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find GD library")
   ENDIF (GD_FIND_REQUIRED)
ENDIF (GD_FOUND)

MARK_AS_ADVANCED(
  GD_LIBRARY
  GD_INCLUDE_DIR
  )

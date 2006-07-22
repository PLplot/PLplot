# - Find Freetype
# Find the native freetype includes and library
# This module defines
#  FREETYPE_INCLUDE_DIR, where to find freetype header files.
#  FREETYPE_LIBRARIES, the libraries needed to use freetype.
#  FREETYPE_FOUND, If false, do not try to use freetype.
# also defined, but not for general use are
#  FREETYPE_LIBRARY, where to find the freetype library.

FIND_PATH(FREETYPE_INCLUDE_DIR freetype/config/ftheader.h
/usr/local/include/freetype2
/usr/include/freetype2
)

SET(FREETYPE_NAMES ${FREETYPE_NAMES} freetype)
FIND_LIBRARY(FREETYPE_LIBRARY
  NAMES ${FREETYPE_NAMES}
  PATHS /usr/lib /usr/local/lib
  )

IF (FREETYPE_LIBRARY AND FREETYPE_INCLUDE_DIR)
    SET(FREETYPE_LIBRARIES ${FREETYPE_LIBRARY})
    SET(FREETYPE_FOUND "YES")
ELSE (FREETYPE_LIBRARY AND FREETYPE_INCLUDE_DIR)
  SET(FREETYPE_FOUND "NO")
ENDIF (FREETYPE_LIBRARY AND FREETYPE_INCLUDE_DIR)

# - Try to find the AGG graphics library
# Once done this will define
#
#  AGG_FOUND - system has AGG
#  AGG_INCLUDE_DIR - the AGG include directory
#  AGG_LIBRARIES - Link these to use AGG
#  AGG_DEFINITIONS - Compiler switches required for using AGG
#

if (AGG_INCLUDE_DIR AND AGG_LIBRARIES)

  # in cache already
  SET(AGG_FOUND TRUE)

else (AGG_INCLUDE_DIR AND AGG_LIBRARIES)

  if(PKG_CONFIG_EXECUTABLE)
    pkg_check_pkgconfig(libagg _AGGIncDir _AGGLinkDir _AGGLinkFlags _AGGCflags _AGG)
    #message(STATUS "_AGGIncDir = ${_AGGIncDir}")
    #message(STATUS "_AGGLinkDir = ${_AGGLinkDir}")
    #message(STATUS "_AGGLinkFlags = ${_AGGLinkFlags}")
    #message(STATUS "_AGGCflags = ${_AGGCflags}")
    
    # Blank-delimited is required.
    string(REGEX REPLACE ";" " " AGG_DEFINITIONS "${_AGGCflags}")
  endif(PKG_CONFIG_EXECUTABLE)
  
  FIND_PATH(AGG_INCLUDE_DIR agg2/agg_pixfmt_gray.h
    ${_AGGIncDir}
    /usr/include
    /usr/local/include
  )
  
  FIND_LIBRARY(AGG_LIBRARIES NAMES agg
    PATHS
    ${_AGGLinkDir}
    /usr/lib
    /usr/local/lib
  )
  
  if (AGG_INCLUDE_DIR AND AGG_LIBRARIES)
     set(AGG_FOUND TRUE)
  endif (AGG_INCLUDE_DIR AND AGG_LIBRARIES)
  
  if (AGG_FOUND)
    if (NOT AGG_FIND_QUIETLY)
      message(STATUS "Found AGG: ${AGG_LIBRARIES}")
    endif (NOT AGG_FIND_QUIETLY)
  else (AGG_FOUND)
    if (AGG_FIND_REQUIRED)
      message(FATAL_ERROR "Could NOT find AGG")
    endif (AGG_FIND_REQUIRED)
  endif (AGG_FOUND)
  
  MARK_AS_ADVANCED(AGG_INCLUDE_DIR AGG_LIBRARIES)
  
endif (AGG_INCLUDE_DIR AND AGG_LIBRARIES)

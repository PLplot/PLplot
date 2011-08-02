# Settings for the GNU Fortran compiler (gfortran) under MinGW
#
SET(CMAKE_SHARED_LIBRARY_Fortran_FLAGS "")                     # -pic
SET(CMAKE_SHARED_LIBRARY_CREATE_Fortran_FLAGS "-shared")       # -shared
SET(CMAKE_SHARED_LIBRARY_LINK_Fortran_FLAGS "")                # +s, flag for exe link to use shared lib
SET(CMAKE_SHARED_LIBRARY_RUNTIME_Fortran_FLAG "")              # -rpath
SET(CMAKE_SHARED_LIBRARY_RUNTIME_Fortran_FLAG_SEP "")          # : or empty

SET(CMAKE_Fortran_CREATE_SHARED_MODULE
  "<CMAKE_Fortran_COMPILER> <CMAKE_SHARED_MODULE_Fortran_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_MODULE_CREATE_Fortran_FLAGS> -o <TARGET> ${CMAKE_GNULD_IMAGE_VERSION} <OBJECTS> <LINK_LIBRARIES>")

SET(CMAKE_Fortran_CREATE_SHARED_LIBRARY
  "<CMAKE_Fortran_COMPILER> <CMAKE_SHARED_LIBRARY_Fortran_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_Fortran_FLAGS> -o <TARGET> -Wl,--out-implib,<TARGET_IMPLIB> ${CMAKE_GNULD_IMAGE_VERSION} <OBJECTS> <LINK_LIBRARIES>")

SET(CMAKE_Fortran_LINK_EXECUTABLE
  "<CMAKE_Fortran_COMPILER> <FLAGS> <CMAKE_Fortran_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> -Wl,--out-implib,<TARGET_IMPLIB> -Wl,--allow-multiple-definition ${CMAKE_GNULD_IMAGE_VERSION} <LINK_LIBRARIES>")

# Initialize Fortran link type selection flags.  These flags are used when
# building a shared library, shared module, or executable that links
# to other libraries to select whether to use the static or shared
# versions of the libraries.
IF(MSYS OR MINGW)
  FOREACH(type SHARED_LIBRARY SHARED_MODULE EXE)
    SET(CMAKE_${type}_LINK_STATIC_Fortran_FLAGS "-Wl,-Bstatic")
    SET(CMAKE_${type}_LINK_DYNAMIC_Fortran_FLAGS "-Wl,-Bdynamic")
  ENDFOREACH(type)
ENDIF(MSYS OR MINGW)

# Create archiving rules to support large object file lists for static
# libraries.
IF(MSYS OR MINGW)
  SET(CMAKE_Fortran_ARCHIVE_CREATE "<CMAKE_AR> cr <TARGET> <LINK_FLAGS> <OBJECTS>")
  SET(CMAKE_Fortran_ARCHIVE_APPEND "<CMAKE_AR> r  <TARGET> <LINK_FLAGS> <OBJECTS>")
  SET(CMAKE_Fortran_ARCHIVE_FINISH "<CMAKE_RANLIB> <TARGET>")
ENDIF(MSYS OR MINGW)

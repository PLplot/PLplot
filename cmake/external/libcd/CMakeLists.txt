# CMakeLists.txt for the cd library and executables
# Put this file in the main directory, create and cd into a build directory,
# and run this cmake command:
#    cmake -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=../local ..
# or similar.

# no shared library support for win32 compilers except cygwin, mingw
IF( WIN32 AND NOT CYGWIN AND NOT MINGW )
  IF( BUILD_SHARED_LIBS )
    SET( BUILD_SHARED_LIBS OFF )
    MESSAGE( " >> There is no shared library support for your Windows compiler!\n >> Turning option off." )
  ENDIF( BUILD_SHARED_LIBS )
ENDIF( WIN32 AND NOT CYGWIN AND NOT MINGW  )

# add cd library
set( cd_LIB_SRCS cd.c )
set( cd_LIB_HDRS cd.h defines.h )
ADD_LIBRARY( cd ${cd_LIB_SRCS} )

# add samples
set( cd_EXE color16 cdsimple cdtext cdmulti cdexpert )
foreach(EXE ${cd_EXE})
  ADD_EXECUTABLE( ${EXE} ${EXE}.c )
  TARGET_LINK_LIBRARIES( ${EXE} cd )
endforeach(EXE ${cd_EXE})

# install library and binaries
INSTALL( TARGETS
  cd ${cd_EXE}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# install header files
INSTALL(
  FILES ${cd_LIB_HDRS}
  DESTINATION include
)
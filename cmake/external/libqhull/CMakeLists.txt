# Configurable items here.
option(USE_RPATH "Use -rpath when linking libraries, executables" ON)

# libraries are all shared by default
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Change before each release whenever there has been an API change of any
# kind.
set(qhull_SOVERSION 5)
set(qhull_VERSION ${qhull_SOVERSION}.0.0)

# no shared library support for win32 compilers except cygwin, mingw
if(WIN32 AND NOT CYGWIN AND NOT MINGW)
  if(BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS OFF)
    message( " >> There is no shared library support for your Windows compiler!\n >> Turning option off." )
  endif(BUILD_SHARED_LIBS)
endif(WIN32 AND NOT CYGWIN AND NOT MINGW)

# find math library
if(NOT WIN32)
  find_library(MATH_LIB NAMES m PATHS /usr/local/lib /usr/lib)
  if(NOT MATH_LIB)
    message(FATAL_ERROR "Cannot find required math library")
  endif(NOT MATH_LIB)
endif(NOT WIN32)

# some extra flags for Borland C++ Compiler 5.5
if(BORLAND)
  add_definitions(-A -w-)
endif(BORLAND)

# Order object files by frequency of execution.  Small files at end.
set(
    qhull_src
        rboxlib.c
        user.c
        global.c
        stat.c
        io.c
        geom2.c
        poly2.c
        merge.c
        libqhull.c
        geom.c
        poly.c
        qset.c
        mem.c
        usermem.c
        userprintf.c
        random.c
)

# header files
set(
	qhull_hdr
		geom.h
		io.h
		libqhull.h
		mem.h
		merge.h
		poly.h
		qhull.h
		qhull_a.h
		qset.h
		random.h
		stat.h
		user.h
)

add_library(
	qhull 
  	${qhull_src}
)

target_link_libraries(
	qhull 
		${MATH_LIB}
)

set_target_properties(
  qhull
	  PROPERTIES 
	  SOVERSION ${qhull_SOVERSION}
	  VERSION ${qhull_VERSION}
 )

if(UNIX)
	if(APPLE)
		set_target_properties(qhull PROPERTIES 
			INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
	else(APPLE)
		set_target_properties(qhull PROPERTIES 
			INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
			INSTALL_RPATH_USE_LINK_PATH TRUE
			BUILD_WITH_INSTALL_RPATH FALSE)
	endif(APPLE)
endif(UNIX)

install(
	TARGETS qhull
		ARCHIVE DESTINATION lib
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
)


# samples
set(qhullcmd_SOURCES  unix.c)
set(rbox_SOURCES      rbox.c)
set(qconvex_SOURCES   qconvex.c)
set(qdelaunay_SOURCES qdelaun.c)
set(qvoronoi_SOURCES  qvoronoi.c)
set(qhalf_SOURCES     qhalf.c)

set(
	TARGET_LIST
		qhullcmd rbox qconvex qdelaunay qvoronoi qhalf
)


foreach(TARGET ${TARGET_LIST})
  add_executable(${TARGET} ${${TARGET}_SOURCES})
  target_link_libraries(${TARGET} qhull)
  if(USE_RPATH)
    set_target_properties(
      ${TARGET}
	      PROPERTIES 
	      INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib
	      INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/bin
      )
  else(USE_RPATH)
    set_target_properties(
      ${TARGET}
	      PROPERTIES 
	      INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/bin
      )
  endif(USE_RPATH)
  install(TARGETS ${TARGET} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endforeach(TARGET ${TARGET_LIST})

set_property(TARGET qhullcmd PROPERTY OUTPUT_NAME "qhull")


# install header
install(
	FILES ${qhull_hdr}
		DESTINATION include/qhull
)

# CMakeLists.txt for the qhull library and executables
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

set(
  qhull_LIB_SRCS
    src/user.c
    src/global.c
    src/stat.c
    src/io.c
    src/geom2.c
    src/poly2.c
    src/merge.c
    src/qhull.c
    src/geom.c
    src/poly.c
    src/qset.c
    src/mem.c
)

set(
  qhull_LIB_HDRS
    src/geom.h
    src/io.h
    src/mem.h
    src/merge.h
    src/poly.h
    src/qhull.h
    src/qhull_a.h
    src/qset.h
    src/stat.h
    src/user.h
)

set(
  qhull_HTML_FILES
    qhull.man rbox.man qh-in.htm qh-optg.htm qh-optt.htm qh-optp.htm
    index.htm qh-quick.htm qh-impre.htm qh-eg.htm
    qh-optc.htm qh-opto.htm qh-optf.htm qh-optq.htm
	  qh-c.htm qh-faq.htm qhull.htm qconvex.htm qdelaun.htm
		qh-geom.htm qh-globa.htm qh-io.htm qh-mem.htm qh-merge.htm
		qh-poly.htm qh-qhull.htm qh-set.htm qh-stat.htm qh-user.htm
		qdelau_f.htm qhalf.htm qvoronoi.htm qvoron_f.htm rbox.htm 
)


IF(BORLAND)
  ADD_DEFINITIONS(-A -w-)
ENDIF(BORLAND)

ADD_LIBRARY(
  qhull
  ${qhull_LIB_SRCS}
)

# add samples
set( qhull_EXE qconvex qhalf qvoronoi user_eg user_eg2)
foreach(EXE ${qhull_EXE})
  ADD_EXECUTABLE( ${EXE} src/${EXE}.c )
  TARGET_LINK_LIBRARIES( ${EXE} qhull )
endforeach(EXE ${qhull_EXE})

ADD_EXECUTABLE( qhullx src/unix.c )
TARGET_LINK_LIBRARIES( qhullx qhull )

ADD_EXECUTABLE( qdelaunay src/qdelaun.c )
TARGET_LINK_LIBRARIES( qdelaunay qhull )

ADD_EXECUTABLE( rbox src/rbox.c )


INSTALL(
  TARGETS
    qhull ${qhull_EXE} qhullx qdelaunay rbox
  ARCHIVE DESTINATION 
    lib
  LIBRARY DESTINATION 
    lib
  RUNTIME DESTINATION
    bin
)

INSTALL(
  FILES
    ${qhull_LIB_HDRS}
  DESTINATION
    include/qhull
)
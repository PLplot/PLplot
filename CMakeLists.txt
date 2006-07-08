#Version 2.4.2 or above of cmake is required!
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.2 FATAL_ERROR)
project(plplot)

#libraries are all shared for now.
set(BUILD_SHARED_LIBS ON)

#relative path of installed libraries.
set(LIB_INSTALL_DIR lib)

#relative path of installed documentation.
set(DOC_INSTALL_DIR share/doc/plplot)

add_subdirectory(lib)

#
# Packing stuff
#
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Scientific Plotting Library PLplot")
set(CPACK_PACKAGE_VENDOR "PLplot development team")
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README)
set(CPACK_PACKAGE_VERSION_MAJOR 5)
set(CPACK_PACKAGE_VERSION_MINOR 6)
set(CPACK_PACKAGE_VERSION_PATCH 1)
set(CPACK_GENERATOR TGZ)
set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_IGNORE_FILES
"Makefile\\\\.in$"
"~$"
"^${PROJECT_SOURCE_DIR}.*/CVS/"
"^${PROJECT_SOURCE_DIR}/bindings/"
"^${PROJECT_SOURCE_DIR}/cf/"
"^${PROJECT_SOURCE_DIR}/data/"
"^${PROJECT_SOURCE_DIR}/debian/"
"^${PROJECT_SOURCE_DIR}/doc/"
"^${PROJECT_SOURCE_DIR}/drivers/"
"^${PROJECT_SOURCE_DIR}/examples/"
"^${PROJECT_SOURCE_DIR}/fonts/"
"^${PROJECT_SOURCE_DIR}/include/"
"^${PROJECT_SOURCE_DIR}/libltdl/"
"^${PROJECT_SOURCE_DIR}/old/"
"^${PROJECT_SOURCE_DIR}/pkgcfg/"
"^${PROJECT_SOURCE_DIR}/rpm/"
"^${PROJECT_SOURCE_DIR}/scripts/"
"^${PROJECT_SOURCE_DIR}/src/"
"^${PROJECT_SOURCE_DIR}/sys/"
"^${PROJECT_SOURCE_DIR}/test/"
"^${PROJECT_SOURCE_DIR}/utils/"
)
message("CPACK_SOURCE_IGNORE_FILES = ${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)

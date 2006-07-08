# Version 2.4.2 or above of cmake is required!
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.2 FATAL_ERROR)
project(plplot)

# parameters to control overall cmake behaviour.

# Print out complete Makefile commands.
# This is commented out because if ever need this, just
# use "make VERBOSE=1"
#SET(CMAKE_VERBOSE_MAKEFILE ON)

# libraries are all shared for now.
set(BUILD_SHARED_LIBS ON)

# Install locations.

# Relative path of installed libraries.
set(LIB_INSTALL_DIR lib)

# Relative path of installed headers.
set(HEADER_INSTALL_DIR include/plplot)

# Relative path of installed documentation.
set(DOC_INSTALL_DIR share/doc/plplot)

# Configure PLplot components variables....

# To be done RSN...

# Use configured variables to process configurable top-level files.

configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake 
${CMAKE_CURRENT_BINARY_DIR}/config.h
)

# Install top-level files

set(top_level_DOCFILES
AUTHORS
COPYING.LIB
ChangeLog
Copyright
FAQ
NEWS
PROBLEMS
README
README.release
)
install(FILES ${top_level_DOCFILES} DESTINATION ${DOC_INSTALL_DIR})

# Process other directories including using configured variables to 
# process configurable files in those directories.

add_subdirectory(lib)
add_subdirectory(include)

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
# The following components are regex's to match anywhere (unless anchored)
# in absolute patch + filename to find files or directories to be excluded
# from source tarball.
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

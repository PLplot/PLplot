
# determine the compiler to use for Ada programs
# NOTE, a generator may set CMAKE_Ada_COMPILER before
# loading this file to force a compiler.
# use environment variable ADA first if defined by user, next use
# the cmake variable CMAKE_GENERATOR_ADA which can be defined by a generator
# as a default compiler
#
# Sets the following variables:
#   CMAKE_Ada_COMPILER
#   CMAKE_AR
#   CMAKE_RANLIB
#   CMAKE_COMPILER_IS_GNUGNAT

IF(NOT CMAKE_Ada_COMPILER)
  # prefer the environment variable ADA
  IF($ENV{ADA} MATCHES ".+")
    GET_FILENAME_COMPONENT(CMAKE_Ada_COMPILER_INIT $ENV{ADA} PROGRAM PROGRAM_ARGS CMAKE_Ada_FLAGS_ENV_INIT)
    IF(CMAKE_Ada_FLAGS_ENV_INIT)
      SET(CMAKE_Ada_COMPILER_ARG1 "${CMAKE_Ada_FLAGS_ENV_INIT}" CACHE STRING "First argument to Ada compiler")
    ENDIF(CMAKE_Ada_FLAGS_ENV_INIT)
    IF(NOT EXISTS ${CMAKE_Ada_COMPILER_INIT})
      MESSAGE(FATAL_ERROR "Could not find compiler set in environment variable ADA:\n$ENV{ADA}.")
    ENDIF(NOT EXISTS ${CMAKE_Ada_COMPILER_INIT})
  ENDIF($ENV{ADA} MATCHES ".+")

  # next prefer the generator-specified compiler
  IF(CMAKE_GENERATOR_ADA)
    IF(NOT CMAKE_Ada_COMPILER_INIT)
      SET(CMAKE_Ada_COMPILER_INIT ${CMAKE_GENERATOR_ADA})
    ENDIF(NOT CMAKE_Ada_COMPILER_INIT)
  ENDIF(CMAKE_GENERATOR_ADA)

  # finally list compilers to try
  IF(CMAKE_Ada_COMPILER_INIT)
    SET(CMAKE_Ada_COMPILER_LIST ${CMAKE_Ada_COMPILER_INIT})
  ELSE(CMAKE_Ada_COMPILER_INIT)
    SET(CMAKE_Ada_COMPILER_LIST gnatgcc gcc)
  ENDIF(CMAKE_Ada_COMPILER_INIT)

  # Find the compiler.
  FIND_PROGRAM(CMAKE_Ada_COMPILER NAMES ${CMAKE_Ada_COMPILER_LIST} DOC "Ada compiler")
  IF(CMAKE_Ada_COMPILER_INIT AND NOT CMAKE_Ada_COMPILER)
    SET(CMAKE_Ada_COMPILER "${CMAKE_Ada_COMPILER_INIT}" CACHE FILEPATH "Ada compiler" FORCE)
  ENDIF(CMAKE_Ada_COMPILER_INIT AND NOT CMAKE_Ada_COMPILER)
ENDIF(NOT CMAKE_Ada_COMPILER)
MARK_AS_ADVANCED(CMAKE_Ada_COMPILER)

GET_FILENAME_COMPONENT(COMPILER_LOCATION "${CMAKE_Ada_COMPILER}"
  PATH)

FIND_PROGRAM(GNAT_EXECUTABLE_BUILDER NAMES gnatmake PATHS ${COMPILER_LOCATION} )

# Assume always use GNU version of GNAT.
SET(CMAKE_COMPILER_IS_GNUGNAT 1)

INCLUDE(CMakeFindBinUtils)

# configure variables set in this file for fast reload later on
# FIXME later after this location hack is no longer needed.
#CONFIGURE_FILE(${CMAKE_ROOT}/Modules/CMakeAdaCompiler.cmake.in 
CONFIGURE_FILE(${CMAKE_MODULE_PATH}/CMakeAdaCompiler.cmake.in 
  ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeAdaCompiler.cmake
  @ONLY IMMEDIATE # IMMEDIATE must be here for compatibility mode <= 2.0
  )
SET(CMAKE_Ada_COMPILER_ENV_VAR "ADA")

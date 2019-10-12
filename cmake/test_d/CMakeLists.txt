# Top-level CMakeLists.txt for a simple project consisting of a
# "hello, world" C library, a D binding for that library, and a D
# executable that links to that D binding to
# test CMake language support for D. See the README file in this
# directory for more details.

# MAINTENANCE 2019-08-29
# Use minimum version that insures necessary CMake language support fix is available.
# for all platforms.
cmake_minimum_required(VERSION 3.15.20190829-g3ec986c FATAL_ERROR)

# Before the project command must specify the location where
# the test_d project looks for CMake D language support files.

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

project(test_d C D)

# Deliver the same strong -pthread test case for test_d as occurs for
# the combination of Threads (xwin), pkg-config (for psttf and cairo),
# and wx-config (for wxwidgets) for the plplot project case.
option(THREADS_PREFER_PTHREAD_FLAG "Prefer -pthread" ON)
if(THREADS_PREFER_PTHREAD_FLAG)
  find_package(Threads)
  if(Threads_FOUND)
    set(tll_argument Threads::Threads)
    # N.B. From comprehensive test results for the plplot project case
    # that have been reported to me (AWI) by various testers it is
    # known that one or both of pkg-config and wx-config uses the
    # -pthread flag for linking for the Linux(2019) and MSYS2(2019)
    # platforms, and this is NOT the case for the Cygwin(2016 and
    # 2018) platform where, for example, -pthread does not occur in
    # the CMakeCache.txt file for those two Cygwin comprehensive
    # tests.  Furthermore, we assume for the Mac OS X platform (unless
    # and until plplot comprehensive tests on that platform show
    # otherwise) that pkg-config and/or wx-config uses the -pthread
    # flag for linking.  Therefore, for test_d we currently enforce -pthread
    # testing for all platforms other than Cygwin.
    if(NOT CYGWIN)
      set_target_properties(Threads::Threads PROPERTIES INTERFACE_LINK_LIBRARIES -pthread)
    endif(NOT CYGWIN)
  endif(Threads_FOUND)
endif(THREADS_PREFER_PTHREAD_FLAG)

# Identify this project as the core build (as opposed to installed example
# build).

set(CORE_BUILD ON)

# Build and test build-tree example?
option(BUILD_TEST "Compile example in the build tree and test it" OFF)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Install locations

set(
  CMAKE_INSTALL_EXEC_PREFIX
  ${CMAKE_INSTALL_PREFIX}
  CACHE PATH "install location for architecture-dependent files"
  )

set(
  CMAKE_INSTALL_BINDIR
  ${CMAKE_INSTALL_EXEC_PREFIX}/bin
  CACHE PATH "install location for user executables"
  )

set(
  CMAKE_INSTALL_DATADIR
  ${CMAKE_INSTALL_PREFIX}/share
  CACHE PATH "install location for read-only architecture-independent data"
  )

set(
  CMAKE_INSTALL_LIBDIR
  ${CMAKE_INSTALL_EXEC_PREFIX}/lib
  CACHE PATH "install location for object code libraries"
  )

set(
  CMAKE_INSTALL_INCLUDEDIR
  ${CMAKE_INSTALL_PREFIX}/include
  CACHE PATH "General install location for included files"
  )

# Configured test_d install locations determined from user-updatable
# cached values above.

# Data.
set(DATA_DIR ${CMAKE_INSTALL_DATADIR}/test_d)

# Libraries.
set(LIB_DIR ${CMAKE_INSTALL_LIBDIR})

# D source code.
set(SOURCE_CODE_DIR ${CMAKE_INSTALL_LIBDIR}/d/test_d)

# Binaries.
set(BIN_DIR ${CMAKE_INSTALL_BINDIR})

set(LIB_INSTALL_RPATH ${LIB_DIR})

option(USE_RPATH "Use -rpath when linking libraries, executables" ON)

if(BUILD_SHARED_LIBS AND (WIN32 OR CYGWIN))
  # For platforms (currently WIN32 or Cygwin, although the Cygwin version
  # of CMake may support this in future since -rpath apparently does work
  # on that platform) where CMake does not use -rpath, use a workaround
  # where all dll's are collected in the dll subdirectory of the build tree.
  set(USE_DLL_SUBDIRECTORY ON)
else(BUILD_SHARED_LIBS AND (WIN32 OR CYGWIN))
  set(USE_DLL_SUBDIRECTORY OFF)
endif(BUILD_SHARED_LIBS AND (WIN32 OR CYGWIN))

# For all windows platforms all created dlls are gathered in the dll directory
# if you add this directory to your PATH all shared libraries are available
if(USE_DLL_SUBDIRECTORY)
  set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/dll)
endif(USE_DLL_SUBDIRECTORY)

set(LANG_SUPPORT_FILES
  ./:CMakeDCompiler.cmake.in
  ./:CMakeDCompilerABI.d
  ./:CMakeDCompilerId.d.in
  ./:CMakeDInformation.cmake
  ./:CMakeDetermineDCompiler.cmake
  ./:CMakePlatformId.di.in
  ./:CMakeTestDCompiler.cmake
  Platform/:Darwin-dmd.cmake
  Platform/:Darwin-ldc2.cmake
  Platform/:Linux-dmd.cmake
  Platform/:Linux-gdc.cmake
  Platform/:Linux-ldc2.cmake
  Platform/:Windows-dmd.cmake
  Platform/:Windows-gdc.cmake
  )

foreach(LANG_SUPPORT_info ${LANG_SUPPORT_FILES})
  string(REGEX REPLACE "^(.*):.*$" "\\1" LANG_SUPPORT_DIR ${LANG_SUPPORT_info})
  string(REGEX REPLACE "^.*:(.*)$" "\\1" LANG_SUPPORT_FILE ${LANG_SUPPORT_info})
  install(FILES
    ${CMAKE_MODULE_PATH}/${LANG_SUPPORT_DIR}/${LANG_SUPPORT_FILE}
    DESTINATION ${DATA_DIR}/examples/cmake/Modules/${LANG_SUPPORT_DIR}
    )
endforeach(LANG_SUPPORT_info ${LANG_SUPPORT_FILES})

# cmp executable is used to help implement test_noninteractive targed for
# the D executable.
find_program(CMP_EXECUTABLE cmp)

# Configure and install a file to configure the build of the
# installed example.
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/test_d_configure.cmake_installed_examples.in
  ${CMAKE_CURRENT_BINARY_DIR}/test_d_configure.cmake_installed_examples
  @ONLY
  )
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/test_d_configure.cmake_installed_examples
  DESTINATION ${DATA_DIR}/examples/cmake/Modules
  RENAME test_d_configure.cmake
  )

# Build C library
add_subdirectory(src_c_lib)

# Build D binding of that C library
add_subdirectory(src_d_lib)

# Build "hello, world" D example that links to the D (and C?) libraries.
add_subdirectory(src_d_executable)

install(FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/installed_CMakeLists.txt
  DESTINATION ${DATA_DIR}/examples
  RENAME CMakeLists.txt
  )

# Setup and run comprehensive_test.sh for MinGW/MSYS from
# bash.  This script is designed with the limitations of the current
# wine version of Windows platform in mind but could easily be
# generalized for MinGW/MSYS on the Microsoft version of Windows.

# This file is designed to be sourced (with the bash "." command)
# rather than run as a bash subshell so save some attributes that
# need to be restored at the end.

ORIGINAL_DIR=$(pwd)
ORIGINAL_PATH=$PATH

# Establish prefixes for all relevant locations for the PLplot build
# and test.

. set_plplot_build_prefixes.wine_sh

# Establish PATH to MinGW/MSYS.  Order apparently matters here.  MinGW
# should be higher on the PATH then MSYS.
PATH=$MINGW_PREFIX/bin:$MSYS_PREFIX/bin:$PATH

# Use downloaded binary version of cmake to build PLplot.
PATH=$CMAKE_283_PREFIX/bin:$PATH

# Make GTK+ stack of libraries and pkg-config accessible to PLplot
# build.
PATH=$GTKPLUS_PREFIX/bin:$PATH
export PKG_CONFIG_PATH=$GTKPLUS_PREFIX/lib/pkgconfig

# Make qmake application and therefore Qt4 libraries accessible to
# PLplot build.  Lower in the PATH in this case because this Qt4 bin
# directory contains a library name (libgcc_s_dw2-1.dll) that is
# identical with a MinGW library name and probably taken from an older
# version of MinGW.
# However, comment this out because qt (even the latest version below)
# is unreliable.  qt.driver_info was produced perfectly by
# test-drv-info.exe on wine but there was still an exit(3) that
# occurred in __mingw_CRTStartup according to gdb.  That exit code of
# 3 generates a make error and everything grinds to a halt.  However,
# if you ignore it and continue, then many examples work with
# qt-related devices, but some do not.  It is quite possible the
# problem with 2010.05 (and prior versions) is they were compiled
# according to http://doc.qt.nokia.com/4.7/compiler-notes.html to
# be compatible with MinGW-4.4, but I cannot get that compiler
# to work with cmake, so forget Qt.
# PATH=$PATH:$QT4_PREFIX/bin

# Make built qhull library accessible to PLplot build.
PATH=$QHULL_PREFIX/bin:$PATH
export CMAKE_INCLUDE_PATH=$QHULL_PREFIX/include
export CMAKE_LIBRARY_PATH=$QHULL_PREFIX/lib

# Give access to swig.  (Swig installation has no separate bin directory).
PATH=$SWIG_PREFIX:$PATH

# Give access to Python. (Python installation has no separate bin directory).
PATH=$PYTHON_PREFIX:$PATH
CMAKE_INCLUDE_PATH=$PYTHON_PREFIX/Lib/site-packages/numpy/core/include/numpy:$CMAKE_INCLUDE_PATH
CMAKE_INCLUDE_PATH=$PYTHON_PREFIX/include:$CMAKE_INCLUDE_PATH
CMAKE_LIBRARY_PATH=$PYTHON_PREFIX/libs:$CMAKE_LIBRARY_PATH

# Give access to Windows version of Tcl.  N.B. the windows installer for
# Tcl appears to put $TCL_PREFIX/bin permanently on the PATH so no
# need to place it on the PATH again.
#PATH=$TCL_PREFIX/bin:$PATH
CMAKE_INCLUDE_PATH=$TCL_PREFIX/include:$CMAKE_INCLUDE_PATH
CMAKE_LIBRARY_PATH=$TCL_PREFIX/lib:$CMAKE_LIBRARY_PATH

# Set MinGW compiler flags for the PLplot build.
export CFLAGS='-O3'
export CXXFLAGS=$CFLAGS
export FFLAGS=$CFLAGS

# Notes on invocation of comprehensive_test.sh below.

# Must disable Tcl because cmake under wine somehow finds Linux
# versions of Tcl.  WTF? (1.3.8 last wine version checked).
# Now no longer disable Tcl because you force it to find the
# Windows version (see above).

# Must disable f95 because of some gfortran module parsing problem
# under wine.  (1.3.8 last wine version checked).

# Set DEBUG_COMPREHENSIVE_TEST_RESULT to some value before sourcing
# this script if you want to set up all environment variables as
# they are for a normal run of comprehensive_test.sh without
# actually running that script.
#	--cmake_added_options "-DENABLE_f95=OFF -DDEFAULT_NO_BINDINGS=ON -DENABLE_tcl=ON -DDEFAULT_NO_DEVICES=ON -DPLD_ps=ON" --do_shared yes --do_nondynamic no --do_static no \

if [ -z "$DEBUG_COMPREHENSIVE_TEST_RESULT" ] ; then
    bash $PLPLOT_SOURCE_PREFIX/scripts/comprehensive_test.sh \
	--prefix $TEST_RESULTS_PREFIX \
	--generator_string "MSYS Makefiles" --build_command "make -j4" \
	--cmake_added_options "-DENABLE_f95=OFF" --do_shared yes --do_nondynamic no --do_static no \
	--do_ctest no --do_test_noninteractive yes --do_test_interactive no --do_test_build_tree yes --do_test_install_tree yes --do_test_traditional_install_tree yes

    # This file is sourced so restore original directory location and PATH.
    cd $ORIGINAL_DIR
    PATH=$ORIGINAL_PATH
fi

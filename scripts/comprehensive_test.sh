#!/bin/bash

# Complete tests of PLplot for the three generic build types which
# consist of shared+dynamic, shared+nondynanic, and
# static+nondynamic.  These complete tests that are run for each build
# type are (I) ctest, test_noninteractive, and test_interactive in
# build tree; (II) traditional [Makefile+pkg-config]
# test_noninteractive and test_interactive of installed examples; and
# (III) CMake-based test_noninteractive and test_interactive of
# installed examples.

# FIXME: the following should be set as a result of user options.
# These control what kind of build is tested.
CMAKE_ADDED_OPTIONS='-DPLD_pdf=ON'
do_shared=yes
do_nondynamic=yes
do_static=yes

# These control which of 7 tests that are run.
do_ctest=yes
do_build_tree_test=yes
do_install_tree_test=yes
do_traditional_tree_test=yes
do_noninteractive_test=yes
do_interactive_test=yes

# Find absolute PATH of script without using readlink (since readlink is
# not available on all platforms).  Followed advice at
# http://fritzthomas.com/open-source/linux/551-how-to-get-absolute-path-within-shell-script-part2/
ORIGINAL_PATH="$(pwd)"
cd "$(dirname $0)"
# Absolute Path of the script
SCRIPT_PATH="$(pwd)"
cd "${ORIGINAL_PATH}"

# Assumption: top-level source tree is parent directory of where script
# is located.
SOURCE_TREE="$(dirname ${SCRIPT_PATH})"
# This is the parent tree for the BUILD_TREE, INSTALL_TREE, 
# INSTALL_BUILD_TREE, and OUTPUT_TREE.  It is disposable.
PARENT_TREE="${SOURCE_TREE}/comprehensive_test_disposeable"

function comprehensive_test {
    mkdir -p "$OUTPUT_TREE"
    rm -rf "$BUILD_TREE"
    mkdir -p "$BUILD_TREE"
    cd "$BUILD_TREE"
    if [ "$do_ctest" = "yes" -o "$do_build_tree_test" = "yes" ] ; then
	BUILD_TEST_OPTION="-DBUILD_TEST=ON"
    else
	BUILD_TEST_OPTION=""
    fi
    output="$OUTPUT_TREE"/cmake.out
    rm -f "$output"
    cmake "-DCMAKE_INSTALL_PREFIX=$INSTALL_TREE" $BUILD_TEST_OPTION \
	$CMAKE_ADDED_OPTIONS $1 "$SOURCE_TREE" >& "$output"
    cmake_rc=$?
    if [ "$cmake_rc" -eq 0 ] ; then
	if [ "$do_build_tree_test" = "yes" -a  "$do_noninteractive_test" = "yes" ] ; then
	    output="$OUTPUT_TREE"/make_noninteractive.out
	    rm -f "$output"
	    make -j4 VERBOSE=1 test_noninteractive >& "$output"
	fi
	rm -rf "$INSTALL_TREE"
	output="$OUTPUT_TREE"/make_install.out
	rm -f "$output"
	make -j4 VERBOSE=1 install >& "$output"
	make_install_rc=$?
	if [ "$make_install_rc" -eq 0 ] ; then
	    PATH="$INSTALL_TREE/bin":$PATH
	    if [ "$do_ctest" = "yes" ] ; then
		output="$OUTPUT_TREE"/ctest.out
		rm -f "$output"
		ctest --extra-verbose >& "$output" &
	    fi
	    if [ "$do_install_tree_test" = "yes" ] ; then
		rm -rf "$INSTALL_BUILD_TREE"
		mkdir -p "$INSTALL_BUILD_TREE"
		cd "$INSTALL_BUILD_TREE"
		output="$OUTPUT_TREE"/installed_cmake.out
		rm -f "$output"
		cmake "$INSTALL_TREE"/share/plplot?.?.?/examples >& "$output"
		if [ "$do_noninteractive_test" = "yes" ] ; then
		    output="$OUTPUT_TREE"/installed_make_noninteractive.out
		    rm -f "$output"
		    make -j4 VERBOSE=1 test_noninteractive >& "$output"
		fi
	    fi
	    if [ "$do_traditional_tree_test" = "yes" -a "$do_noninteractive_test" = "yes" ] ; then
		cd "$INSTALL_TREE"/share/plplot?.?.?/examples
		output="$OUTPUT_TREE"/traditional_make_noninteractive.out
		rm -f "$output"
		make -j4 test_noninteractive >& "$output"
	    fi
	else
	    echo "ERROR: make install failed"
	    exit 1
	fi

	if [ "$do_interactive_test" = "yes" ] ; then
	    if [ "$do_build_tree_test" = "yes" ] ; then
		cd "$BUILD_TREE"
		output="$OUTPUT_TREE"/make_interactive.out
		rm -f "$output"
		make -j4 VERBOSE=1 test_interactive >& "$output"
	    fi
	    if [ "$do_install_tree_test" = "yes" ] ; then
		cd "$INSTALL_BUILD_TREE"
		output="$OUTPUT_TREE"/installed_make_interactive.out
		rm -f "$output"
		make -j4 VERBOSE=1 test_interactive >& "$output"
	    fi
	    if [ "$do_traditional_tree_test" = "yes" ] ; then
		cd "$INSTALL_TREE"/share/plplot?.?.?/examples
		output="$OUTPUT_TREE"/traditional_make_interactive.out
		rm -f "$output"
		make -j4 test_interactive >& "$output"
	    fi
	fi
    else
	echo "ERROR: cmake in build tree failed"
	exit 1
    fi
}

# Shared + dynamic
if [ "$do_shared" = "yes" ] ; then
    OUTPUT_TREE="$PARENT_TREE/shared/output_tree"
    BUILD_TREE="$PARENT_TREE/shared/build_tree"
    INSTALL_TREE="$PARENT_TREE/shared/install_tree"
    INSTALL_BUILD_TREE="$PARENT_TREE/shared/install_build_tree"
    comprehensive_test "-DBUILD_SHARED_LIBS=ON"
fi

# Shared + nondynamic
if [ "$do_nondynamic" = "yes" ] ; then
    OUTPUT_TREE="$PARENT_TREE/nondynamic/output_tree"
    BUILD_TREE="$PARENT_TREE/nondynamic/build_tree"
    INSTALL_TREE="$PARENT_TREE/nondynamic/install_tree"
    INSTALL_BUILD_TREE="$PARENT_TREE/nondynamic/install_build_tree"
    comprehensive_test "-DBUILD_SHARED_LIBS=ON -DENABLE_DYNDRIVERS=OFF"
fi

if [ "$do_static" = "yes" ] ; then
# Static + nondynamic
    OUTPUT_TREE="$PARENT_TREE/static/output_tree"
    BUILD_TREE="$PARENT_TREE/static/build_tree"
    INSTALL_TREE="$PARENT_TREE/static/install_tree"
    INSTALL_BUILD_TREE="$PARENT_TREE/static/install_build_tree"
    comprehensive_test "-DBUILD_SHARED_LIBS=OFF"
fi

#!/bin/bash

# Complete tests of PLplot for the three generic build types which
# consist of shared+dynamic, shared+nondynanic, and
# static+nondynamic.  These complete tests that are run for each build
# type are (I) ctest, test_noninteractive, and test_interactive in
# build tree; (II) traditional [Makefile+pkg-config]
# test_noninteractive and test_interactive of installed examples; and
# (III) CMake-based test_noninteractive and test_interactive of
# installed examples.

usage () {
  local prog=`basename $0`
  echo "Usage: $prog [OPTIONS]
OPTIONS:
  This next option is mandatory.
  [--build_command (e.g.,  'make -j4' or 'nmake')]
  The next four control what kind of build is tested.
  [--cmake_added_options (defaults to no additional cmake options, but this
                         shell option can be used to supply additional cmake
                         options to, e.g, specify the cmake generator or
                         refine what parts of the PLplot software are
                         built and tested)
  [--do_shared (yes/no, defaults to yes)]
  [--do_nondynamic (yes/no, defaults to yes)]
  [--do_static (yes/no, defaults to yes)]

  The next six control which of seven kinds of tests are done for 
  each kind of build.
  [--do_ctest (yes/no, defaults to yes)]
  [--do_noninteractive_test (yes/no, defaults to yes)]
  [--do_interactive_test (yes/no, defaults to yes)]
  [--do_build_tree_test (yes/no, defaults to yes)]
  [--do_install_tree_test (yes/no, defaults to yes)]
  [--do_traditional_tree_test (yes/no, defaults to yes)]

  [--help] Show this message.
"
  exit $1
}

comprehensive_test () {
    echo "
Running comprehensive_test function with the following variables set:

The variables below are key CMake options which determine the entire
kind of build that will be tested.
\$1 = $1

The location below is where all the important *.out files will be found.
OUTPUT_TREE = $OUTPUT_TREE

The location below is the top-level build-tree directory.
BUILD_TREE = $BUILD_TREE

The location below is the top-level install-tree directory.
INSTALL_TREE = $INSTALL_TREE

The location below is the top-level directory of the build tree used
for the CMake-based build and test of the installed examples.
INSTALL_BUILD_TREE = $INSTALL_BUILD_TREE

Each of the steps in this comprehensive test may take a while...."

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
    echo "cmake in the build tree"
    cmake "-DCMAKE_INSTALL_PREFIX=$INSTALL_TREE" $BUILD_TEST_OPTION \
	"$cmake_added_options" $1 "$SOURCE_TREE" >& "$output"
    cmake_rc=$?
    if [ "$cmake_rc" -eq 0 ] ; then
	if [ "$do_build_tree_test" = "yes" -a  "$do_noninteractive_test" = "yes" ] ; then
	    output="$OUTPUT_TREE"/make_noninteractive.out
	    rm -f "$output"
	    echo "$build_command test_noninteractive in the build tree"
	    $build_command VERBOSE=1 test_noninteractive >& "$output"
	fi
	rm -rf "$INSTALL_TREE"
	output="$OUTPUT_TREE"/make_install.out
	rm -f "$output"
	echo "$build_command install in the build tree"
	$build_command VERBOSE=1 install >& "$output"
	make_install_rc=$?
	if [ "$make_install_rc" -eq 0 ] ; then
	    PATH="$INSTALL_TREE/bin":$PATH
	    if [ "$do_ctest" = "yes" ] ; then
		output="$OUTPUT_TREE"/ctest.out
		rm -f "$output"
		echo "launch ctest job in the build tree"
		ctest --extra-verbose >& "$output" &
	    fi
	    if [ "$do_install_tree_test" = "yes" ] ; then
		rm -rf "$INSTALL_BUILD_TREE"
		mkdir -p "$INSTALL_BUILD_TREE"
		cd "$INSTALL_BUILD_TREE"
		output="$OUTPUT_TREE"/installed_cmake.out
		rm -f "$output"
		echo "cmake in the installed examples build tree"
		cmake "$INSTALL_TREE"/share/plplot?.?.?/examples >& "$output"
		if [ "$do_noninteractive_test" = "yes" ] ; then
		    output="$OUTPUT_TREE"/installed_make_noninteractive.out
		    rm -f "$output"
		    echo "$build_command test_noninteractive in the installed examples build tree"
		    $build_command VERBOSE=1 test_noninteractive >& "$output"
		fi
	    fi
	    if [ "$do_traditional_tree_test" = "yes" -a "$do_noninteractive_test" = "yes" ] ; then
		cd "$INSTALL_TREE"/share/plplot?.?.?/examples
		output="$OUTPUT_TREE"/traditional_make_noninteractive.out
		rm -f "$output"
		echo "Traditional $build_command test_noninteractive in the installed examples tree"
		$build_command test_noninteractive >& "$output"
	    fi
	else
	    echo "ERROR: $build_command install failed"
	    exit 1
	fi

	if [ "$do_interactive_test" = "yes" ] ; then
	    if [ "$do_build_tree_test" = "yes" ] ; then
		cd "$BUILD_TREE"
		output="$OUTPUT_TREE"/make_interactive.out
		rm -f "$output"
		echo "$build_command test_interactive in the build tree"
		$build_command VERBOSE=1 test_interactive >& "$output"
	    fi
	    if [ "$do_install_tree_test" = "yes" ] ; then
		cd "$INSTALL_BUILD_TREE"
		output="$OUTPUT_TREE"/installed_make_interactive.out
		rm -f "$output"
		echo "$build_command test_interactive in the installed examples build tree"
		$build_command VERBOSE=1 test_interactive >& "$output"
	    fi
	    if [ "$do_traditional_tree_test" = "yes" ] ; then
		cd "$INSTALL_TREE"/share/plplot?.?.?/examples
		output="$OUTPUT_TREE"/traditional_make_interactive.out
		rm -f "$output"
		echo "Traditional $build_command test_interactive in the installed examples tree"
		$build_command test_interactive >& "$output"
	    fi
	fi
    else
	echo "ERROR: cmake in build tree failed"
	exit 1
    fi
}

build_command=
cmake_added_options=
do_shared=yes
do_nondynamic=yes
do_static=yes

# These control which of 7 tests that are run.
do_ctest=yes
do_noninteractive_test=yes
do_interactive_test=yes
do_build_tree_test=yes
do_install_tree_test=yes
do_traditional_tree_test=yes

while test $# -gt 0; do

    case $1 in
        --build_command)
	    build_command=$2
	    shift
	    ;;
        --cmake_added_options)
	    cmake_added_options=$2
            shift
            ;;
	--do_shared)
	    case $2 in
		yes|no)
		    do_shared=$2
		    shift
		    ;;
		
		*)
		    usage 1 1>&2
		    ;;
	    esac
	    ;;
	--do_nondynamic)
	    case $2 in
		yes|no)
		    do_nondynamic=$2
		    shift
		    ;;
		*)
		    usage 1 1>&2
		    ;;
	    esac
	    ;;
	--do_static)
	    case $2 in
		yes|no)
		    do_static=$2
		    shift
		    ;;
		*)
		    usage 1 1>&2
		    ;;
	    esac
	    ;;
	--do_ctest)
	    case $2 in
		yes|no)
		    do_ctest=$2
		    shift
		    ;;
		*)
		    usage 1 1>&2
		    ;;
	    esac
	    ;;
	--do_noninteractive_test)
	    case $2 in
		yes|no)
		    do_noninteractive_test=$2
		    shift
		    ;;
		*)
		    usage 1 1>&2
		    ;;
	    esac
	    ;;
	--do_interactive_test)
	    case $2 in
		yes|no)
		    do_interactive_test=$2
		    shift
		    ;;
		*)
		    usage 1 1>&2
		    ;;
	    esac
	    ;;
	--do_build_tree_test)
	    case $2 in
		yes|no)
		    do_build_tree_test=$2
		    shift
		    ;;
		*)
		    usage 1 1>&2
		    ;;
	    esac
	    ;;
	--do_install_tree_test)
	    case $2 in
		yes|no)
		    do_install_tree_test=$2
		    shift
		    ;;
		*)
		    usage 1 1>&2
		    ;;
	    esac
	    ;;
	--do_traditional_tree_test)
	    case $2 in
		yes|no)
		    do_traditional_tree_test=$2
		    shift
		    ;;
		*)
		    usage 1 1>&2
		    ;;
	    esac
	    ;;
        --help)
            usage 0 1>&2
            ;;
        *)
            usage 1 1>&2
            ;;
    esac
    shift
done

if [ -z "$build_command" ] ; then
    echo "--build_command option is mandatory!"
    usage 1 1>&2
fi

echo "Summary of options used for these tests

This variable set as a result of the one mandatory --build_command option.
build_command = $build_command

This/these cmake option(s) added for all builds
cmake_added_options = $cmake_added_options

The next three control whether builds with shared libraries/dynamic
drivers; shared_libraries/nondynamic drivers; and/or static
libraries/nondynamic drivers are tested
do_shared = $do_shared
do_nondynamic = $do_nondynamic
do_static = $do_static

Run the ctest test?
do_ctest = $do_ctest

The next two control whether noninteractive and/or interactive tests are
performed for the next three kinds of tests
do_noninteractive_test = $do_noninteractive_test
do_interactive_test = $do_interactive_test

The next three control whether build tree examples, installed examples
with CMake-based build system, and/or installed examples with
traditional (Makefile + pkg-config) build system tests are run.
do_build_tree_test = $do_build_tree_test
do_install_tree_test = $do_install_tree_test
do_traditional_tree_test = $do_traditional_tree_test
"
ANSWER=
while [ "$ANSWER" != "yes" -a "$ANSWER" != "no" ] ; do
    echo -n "Continue (yes/no)? "
    read ANSWER
done
if [ "$ANSWER" = "no" ] ; then
    echo "Immediate exit specified!"
    exit
fi

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

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
  The next five control what kind of build is tested.
  [--generator_string (defaults to 'Unix Makefiles')]
  [--cmake_added_options (defaults to none, but can be used to specify any
                          combination of white-space-separated cmake options
                          to, e.g., refine what parts of the PLplot software are
                          built and tested)]
  [--do_shared (yes/no, defaults to yes)]
  [--do_nondynamic (yes/no, defaults to yes)]
  [--do_static (yes/no, defaults to yes)]

  The next six control which of seven kinds of tests are done for 
  each kind of build.
  [--do_ctest (yes/no, defaults to yes)]
  [--do_test_noninteractive (yes/no, defaults to yes)]
  [--do_test_interactive (yes/no, defaults to yes)]
  [--do_test_build_tree (yes/no, defaults to yes)]
  [--do_test_install_tree (yes/no, defaults to yes)]
  [--do_test_traditional_install_tree (yes/no, defaults to yes)]

  The next option controls what command is used for the build(s).
  [--build_command (defaults to 'make -j4')]

  [--help] Show this message.
"
  exit $1
}

comprehensive_test () {
    CMAKE_BUILD_TYPE_OPTION=$1
    echo "
Running comprehensive_test function with the following variables set:

The variables below are key CMake options which determine the entire
kind of build that will be tested.
CMAKE_BUILD_TYPE_OPTION = $CMAKE_BUILD_TYPE_OPTION

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

    PATH_SAVE=$PATH
    if [ "$CMAKE_BUILD_TYPE_OPTION" = "-DBUILD_SHARED_LIBS=ON" -a ( "$generator_string" = "MinGW Makefiles" -o "$generator_string" = "MSYS Makefiles" ) ] ; then
	PATH=$PATH_SAVE:$BUILD_TREE/dll
    fi
    mkdir -p "$OUTPUT_TREE"
    rm -rf "$BUILD_TREE"
    mkdir -p "$BUILD_TREE"
    cd "$BUILD_TREE"
    if [ "$do_ctest" = "yes" -o "$do_test_build_tree" = "yes" ] ; then
	BUILD_TEST_OPTION="-DBUILD_TEST=ON"
    else
	BUILD_TEST_OPTION=""
    fi
    output="$OUTPUT_TREE"/cmake.out
    rm -f "$output"
    #Process $cmake_added_options into $*
    set -- $cmake_added_options
    echo "cmake in the build tree"
    cmake "-DCMAKE_INSTALL_PREFIX=$INSTALL_TREE" $BUILD_TEST_OPTION \
	$* $CMAKE_BUILD_TYPE_OPTION -G "$generator_string" \
        "$SOURCE_TREE" >& "$output"
    cmake_rc=$?
    if [ "$cmake_rc" -eq 0 ] ; then
	if [ "$do_test_build_tree" = "yes" -a  "$do_test_noninteractive" = "yes" ] ; then
	    output="$OUTPUT_TREE"/make_noninteractive.out
	    rm -f "$output"
	    echo "$build_command test_noninteractive in the build tree"
	    $build_command VERBOSE=1 test_noninteractive >& "$output"
	fi
	if [ "$do_ctest" = "yes" -o \
	    "$do_test_install_tree" = "yes" -o \
	    "$do_test_traditional_install_tree" = "yes" ] ; then
	    rm -rf "$INSTALL_TREE"
	    output="$OUTPUT_TREE"/make_install.out
	    rm -f "$output"
	    echo "$build_command install in the build tree"
	    $build_command VERBOSE=1 install >& "$output"
	    make_install_rc=$?
	    if [ "$make_install_rc" -eq 0 ] ; then
		if [ "$do_ctest" = "yes" ] ; then
		    output="$OUTPUT_TREE"/ctest.out
		    rm -f "$output"
		    echo "launch ctest job in the build tree"
		    ctest --extra-verbose >& "$output" &
		fi
		PATH="$INSTALL_TREE/bin":$PATH_SAVE
		if [ "$do_test_install_tree" = "yes" ] ; then
		    rm -rf "$INSTALL_BUILD_TREE"
		    mkdir -p "$INSTALL_BUILD_TREE"
		    cd "$INSTALL_BUILD_TREE"
		    output="$OUTPUT_TREE"/installed_cmake.out
		    rm -f "$output"
		    echo "cmake in the installed examples build tree"
		    cmake "$INSTALL_TREE"/share/plplot?.?.?/examples >& "$output"
		    if [ "$do_test_noninteractive" = "yes" ] ; then
			output="$OUTPUT_TREE"/installed_make_noninteractive.out
			rm -f "$output"
			echo "$build_command test_noninteractive in the installed examples build tree"
			$build_command VERBOSE=1 test_noninteractive >& "$output"
		    fi
		fi
		if [ "$do_test_traditional_install_tree" = "yes" -a "$do_test_noninteractive" = "yes" ] ; then
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
	fi

	PATH=$PATH_SAVE
	if [ "$do_test_interactive" = "yes" ] ; then
	    if [ "$do_test_build_tree" = "yes" ] ; then
		cd "$BUILD_TREE"
		output="$OUTPUT_TREE"/make_interactive.out
		rm -f "$output"
		echo "$build_command test_interactive in the build tree"
		$build_command VERBOSE=1 test_interactive >& "$output"
	    fi
	    PATH="$INSTALL_TREE/bin":$PATH_SAVE
	    if [ "$do_test_install_tree" = "yes" ] ; then
		cd "$INSTALL_BUILD_TREE"
		output="$OUTPUT_TREE"/installed_make_interactive.out
		rm -f "$output"
		echo "$build_command test_interactive in the installed examples build tree"
		$build_command VERBOSE=1 test_interactive >& "$output"
	    fi
	    if [ "$do_test_traditional_install_tree" = "yes" ] ; then
		cd "$INSTALL_TREE"/share/plplot?.?.?/examples
		output="$OUTPUT_TREE"/traditional_make_interactive.out
		rm -f "$output"
		echo "Traditional $build_command test_interactive in the installed examples tree"
		$build_command test_interactive >& "$output"
	    fi
	fi
    else
	echo "ERROR: cmake in the build tree failed"
	exit 1
    fi
}

# Default values for options
generator_string="Unix Makefiles"
cmake_added_options=
do_shared=yes
do_nondynamic=yes
do_static=yes

do_ctest=yes
do_test_noninteractive=yes
do_test_interactive=yes
do_test_build_tree=yes
do_test_install_tree=yes
do_test_traditional_install_tree=yes

build_command="make -j4"

while test $# -gt 0; do

    case $1 in
        --generator_string)
	    generator_string=$2
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
	--do_test_noninteractive)
	    case $2 in
		yes|no)
		    do_test_noninteractive=$2
		    shift
		    ;;
		*)
		    usage 1 1>&2
		    ;;
	    esac
	    ;;
	--do_test_interactive)
	    case $2 in
		yes|no)
		    do_test_interactive=$2
		    shift
		    ;;
		*)
		    usage 1 1>&2
		    ;;
	    esac
	    ;;
	--do_test_build_tree)
	    case $2 in
		yes|no)
		    do_test_build_tree=$2
		    shift
		    ;;
		*)
		    usage 1 1>&2
		    ;;
	    esac
	    ;;
	--do_test_install_tree)
	    case $2 in
		yes|no)
		    do_test_install_tree=$2
		    shift
		    ;;
		*)
		    usage 1 1>&2
		    ;;
	    esac
	    ;;
	--do_test_traditional_install_tree)
	    case $2 in
		yes|no)
		    do_test_traditional_install_tree=$2
		    shift
		    ;;
		*)
		    usage 1 1>&2
		    ;;
	    esac
	    ;;
        --build_command)
	    build_command=$2
	    shift
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

echo "Summary of options used for these tests

generator_string = $generator_string
cmake_added_options = $cmake_added_options

do_shared = $do_shared
do_nondynamic = $do_nondynamic
do_static = $do_static

do_ctest = $do_ctest

do_test_noninteractive = $do_test_noninteractive
do_test_interactive = $do_test_interactive

do_test_build_tree = $do_test_build_tree
do_test_install_tree = $do_test_install_tree
do_test_traditional_install_tree = $do_test_traditional_install_tree

build_command = $build_command

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

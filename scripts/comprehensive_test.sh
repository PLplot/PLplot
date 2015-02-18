#!/bin/bash

# Complete tests of PLplot for the three generic build types which
# consist of shared+dynamic, shared+nondynamic, and
# static+nondynamic.  These complete tests that are run for each build
# type are (I) ctest, test_noninteractive, and test_interactive in
# build tree; (II) traditional [Makefile+pkg-config]
# test_noninteractive and test_interactive of installed examples; and
# (III) CMake-based test_noninteractive and test_interactive of
# installed examples.

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
    if [ "$generator_string" = "MinGW Makefiles" -o "$generator_string" = "MSYS Makefiles" ] ; then
	MINGW_OR_MSYS="true"
    else
	MINGW_OR_MSYS="false"
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

    if [ "$CMAKE_BUILD_TYPE_OPTION" != "-DBUILD_SHARED_LIBS=OFF" -a "$MINGW_OR_MSYS" = "true" ] ; then
	echo "Prepend $BUILD_TREE/dll to the original PATH"
	PATH=$BUILD_TREE/dll:$PATH_SAVE
    fi

    # Process $cmake_added_options into $* to be used on the cmake command
    # line below.
    set -- $cmake_added_options
    echo "cmake in the build tree"
    cmake "-DCMAKE_INSTALL_PREFIX=$INSTALL_TREE" $BUILD_TEST_OPTION \
	$* $CMAKE_BUILD_TYPE_OPTION -G "$generator_string" \
        "$SOURCE_TREE" >& "$output"
    cmake_rc=$?
    if [ "$cmake_rc" -ne 0 ] ; then
	echo "ERROR: cmake in the build tree failed"
	exit 1
    fi

    if [ "$do_ctest" = "yes" ] ; then
	output="$OUTPUT_TREE"/make.out
	rm -f "$output"
	echo "$build_command VERBOSE=1 in the build tree"
	$build_command VERBOSE=1 >& "$output"
	make_rc=$?
	if [ "$make_rc" -eq 0 ] ; then
	    output="$OUTPUT_TREE"/ctest.out
	    rm -f "$output"
	    echo "$ctest_command in the build tree"
	    $ctest_command --extra-verbose >& "$output"
	    ctest_rc=$?
	    if [ "$ctest_rc" -eq 0 ] ; then
		if [ "$do_clean_as_you_go" = "yes" ] ; then
		    output="$OUTPUT_TREE"/clean_ctest_plot_files.out
		    rm -f "$output"
		    echo "$build_command clean_ctest_plot_files in the build tree (since we are done with ctest)"
		    $build_command clean_ctest_plot_files >& "$output"
		    make_rc=$?
		    if [ "$make_rc" -ne 0 ] ; then
			echo "ERROR: $build_command clean_ctest_plot_files failed in the build tree"
			exit 1
		    fi
		fi
	    else
		echo "ERROR: $ctest_command failed in the build tree"
		exit 1
	    fi
	else
	    echo "ERROR: $build_command failed in the build tree"
	    exit 1
	fi
    fi

    if [ "$do_test_build_tree" = "yes" -a  "$do_test_noninteractive" = "yes" ] ; then
	output="$OUTPUT_TREE"/make_noninteractive.out
	rm -f "$output"
	echo "$build_command VERBOSE=1 test_noninteractive in the build tree"
	$build_command VERBOSE=1 test_noninteractive >& "$output"
	make_test_noninteractive_rc=$?
	if [ "$make_test_noninteractive_rc" -ne 0 ] ; then
	    echo "ERROR: $build_command test_noninteractive failed in the build tree"
	    exit 1
	fi
    fi

    if [ "$do_test_install_tree" = "yes" -o \
	"$do_test_traditional_install_tree" = "yes" ] ; then
	rm -rf "$INSTALL_TREE"
	output="$OUTPUT_TREE"/make_install.out
	rm -f "$output"
	echo "$build_command VERBOSE=1 install in the build tree"
	$build_command VERBOSE=1 install >& "$output"
	make_install_rc=$?
	if [ "$make_install_rc" -ne 0 ] ; then
	    echo "ERROR: $build_command install failed in the build tree"
	    exit 1
	fi
    fi

    if [ "$do_clean_as_you_go" = "yes" ] ; then
	output="$OUTPUT_TREE"/clean.out
	rm -f "$output"
	echo "$build_command clean in the build tree (since we are done with it at least for the non-interactive test case)"
	$build_command clean >& "$output"
	make_rc=$?
	if [ "$make_rc" -ne 0 ] ; then
	    echo "ERROR: $build_command clean failed in the build tree"
	    exit 1
	fi
    fi

    if [ "$do_test_install_tree" = "yes" -o \
	"$do_test_traditional_install_tree" = "yes" ] ; then
	echo "Prepend $INSTALL_TREE/bin to the original PATH"
	PATH="$INSTALL_TREE/bin":$PATH_SAVE

	if [ "$CMAKE_BUILD_TYPE_OPTION" = "-DBUILD_SHARED_LIBS=ON" -a "$MINGW_OR_MSYS" = "true" ] ; then
	    # Use this logic to be as version-independent as possible.
	    current_dir=$(pwd)
	    # Wild cards must not be inside quotes.
	    cd "$INSTALL_TREE"/lib/plplot[0-9].[0-9]*.[0-9]*/drivers*
	    echo "Prepend $(pwd) to the current PATH"
	    PATH="$(pwd):$PATH"
	    cd $current_dir
	fi

	if [ "$do_test_install_tree" = "yes" ] ; then
	    rm -rf "$INSTALL_BUILD_TREE"
	    mkdir -p "$INSTALL_BUILD_TREE"
	    cd "$INSTALL_BUILD_TREE"
	    output="$OUTPUT_TREE"/installed_cmake.out
	    rm -f "$output"
	    echo "cmake in the installed examples build tree"
	    cmake -G "$generator_string" "$INSTALL_TREE"/share/plplot[0-9].[0-9]*.[0-9]*/examples >& "$output"
	    if [ "$do_test_noninteractive" = "yes" ] ; then
		output="$OUTPUT_TREE"/installed_make_noninteractive.out
		rm -f "$output"
		echo "$build_command VERBOSE=1 test_noninteractive in the installed examples build tree"
		$build_command VERBOSE=1 test_noninteractive >& "$output"
		make_rc=$?
		if [ "$make_rc" -ne 0 ] ; then
		    echo "ERROR: $build_command test_noninteractive failed in the installed examples build tree"
		    exit 1
		fi
		if [ "$do_clean_as_you_go" = "yes" ] ; then
		    output="$OUTPUT_TREE"/installed_clean.out
		    rm -f "$output"
		    echo "$build_command clean in the installed examples build tree (since we are done with it at least for the non-interactive test case)"
		    $build_command clean >& "$output"
		    make_rc=$?
		    if [ "$make_rc" -ne 0 ] ; then
			echo "ERROR: $build_command clean failed in the installed examples build tree"
			exit 1
		    fi
		fi
	    fi
	fi

	if [ "$do_test_traditional_install_tree" = "yes" -a "$do_test_noninteractive" = "yes" ] ; then
	    cd "$INSTALL_TREE"/share/plplot[0-9].[0-9]*.[0-9]*/examples
	    output="$OUTPUT_TREE"/traditional_make_noninteractive.out
	    rm -f "$output"
	    echo "Traditional $traditional_build_command test_noninteractive in the installed examples tree"
	    $traditional_build_command test_noninteractive >& "$output"
	    make_rc=$?
	    if [ "$make_rc" -ne 0 ] ; then
		echo "ERROR: Traditional $traditional_build_command test_noninteractive failed in the installed examples tree"
		exit 1
	    fi
	    if [ "$do_clean_as_you_go" = "yes" ] ; then
		output="$OUTPUT_TREE"/traditional_clean.out
		rm -f "$output"
		echo "Traditional $traditional_build_command clean in the installed examples tree (since we are done with it at least for the non-interactive test case)"
		$traditional_build_command clean >& "$output"
		make_rc=$?
		if [ "$make_rc" -ne 0 ] ; then
		    echo "ERROR: Traditional $traditional_build_command clean failed in the installed examples tree"
		    exit 1
		fi
	    fi
	fi
    fi

    if [ "$do_test_interactive" = "yes" ] ; then
	if [ "$do_test_build_tree" = "yes" ] ; then
	    if [ "$CMAKE_BUILD_TYPE_OPTION" != "-DBUILD_SHARED_LIBS=OFF" -a "$MINGW_OR_MSYS" = "true" ] ; then
		echo "Prepend $BUILD_TREE/dll to the original PATH"
		PATH=$BUILD_TREE/dll:$PATH_SAVE
	    fi

	    cd "$BUILD_TREE"
	    output="$OUTPUT_TREE"/make_interactive.out
	    rm -f "$output"
	    echo "$build_command VERBOSE=1 test_interactive in the build tree"
	    $build_command VERBOSE=1 test_interactive >& "$output"
	    make_rc=$?
	    if [ "$make_rc" -ne 0 ] ; then
		echo "ERROR: $build_command test_interactive failed in the build tree"
		exit 1
	    fi
	fi
	if [ "$do_clean_as_you_go" = "yes" ] ; then
	    output="$OUTPUT_TREE"/clean.out
	    rm -f "$output"
	    echo "$build_command clean in the build tree (since we are done with it)"
	    $build_command clean >& "$output"
	    make_rc=$?
	    if [ "$make_rc" -ne 0 ] ; then
		echo "ERROR: $build_command clean failed in the build tree"
		exit 1
	    fi
	fi
	echo "Prepend $INSTALL_TREE/bin to the original PATH"
	PATH="$INSTALL_TREE/bin":$PATH_SAVE

	if [ "$CMAKE_BUILD_TYPE_OPTION" = "-DBUILD_SHARED_LIBS=ON" -a "$MINGW_OR_MSYS" = "true" ] ; then
	    # Use this logic to be as version-independent as possible.
	    current_dir=$(pwd)
	    # Wild cards must not be inside quotes.
	    cd "$INSTALL_TREE"/lib/plplot[0-9].[0-9]*.[0-9]*/drivers*
	    echo "Prepend $(pwd) to the current PATH"
	    PATH="$(pwd):$PATH"
	    cd $current_dir
	fi

	if [ "$do_test_install_tree" = "yes" ] ; then
	    cd "$INSTALL_BUILD_TREE"
	    output="$OUTPUT_TREE"/installed_make_interactive.out
	    rm -f "$output"
	    echo "$build_command VERBOSE=1 test_interactive in the installed examples build tree"
	    $build_command VERBOSE=1 test_interactive >& "$output"
	    make_rc=$?
	    if [ "$make_rc" -ne 0 ] ; then
		echo "ERROR: $build_command test_interactive failed in the installed examples build tree"
		exit 1
	    fi
	    if [ "$do_clean_as_you_go" = "yes" ] ; then
		output="$OUTPUT_TREE"/installed_clean.out
		rm -f "$output"
		echo "$build_command clean in the installed examples build tree (since we are done with it)"
		$build_command clean >& "$output"
		make_rc=$?
		if [ "$make_rc" -ne 0 ] ; then
		    echo "ERROR: $build_command clean failed in the installed examples build tree"
		    exit 1
		fi
	    fi
	fi
	if [ "$do_test_traditional_install_tree" = "yes" ] ; then
	    cd "$INSTALL_TREE"/share/plplot[0-9].[0-9]*.[0-9]*/examples
	    output="$OUTPUT_TREE"/traditional_make_interactive.out
	    rm -f "$output"
	    echo "Traditional $traditional_build_command test_interactive in the installed examples tree"
	    $traditional_build_command test_interactive >& "$output"
	    make_rc=$?
	    if [ "$make_rc" -ne 0 ] ; then
		echo "ERROR: Traditional $traditional_build_command test_interactive failed in the installed examples tree"
		exit 1
	    fi
	    if [ "$do_clean_as_you_go" = "yes" ] ; then
		output="$OUTPUT_TREE"/traditional_clean.out
		rm -f "$output"
		echo "Traditional $traditional_build_command clean in the installed examples tree (since we are done with it)"
		$traditional_build_command clean >& "$output"
		make_rc=$?
		if [ "$make_rc" -ne 0 ] ; then
		    echo "ERROR: Traditional $traditional_build_command clean failed in the installed examples tree"
		    exit 1
		fi
	    fi
	fi
    fi
    echo "Restore PATH to the original PATH"
    PATH=$PATH_SAVE
}

usage () {
  local prog=`basename $0`
  echo "Usage: $prog [OPTIONS]
OPTIONS:
  The next option specifies the directory prefix which controls where
  all files produced by this script are located.
  [--prefix (defaults to the 'comprehensive_test_disposeable'
                  subdirectory of the top-level source-tree directory)]

  The next option controls whether the script runs clean to get rid of
  file results and save disk space after the tests are completed.
  This option is highly recommended to greatly reduce the
  the disk usage (which can be as large as 40GB [!] without this
  option).
  [--do_clean_as_you_go (yes/no, defaults to yes)]

  The next four control how the builds and tests are done.
  [--generator_string (defaults to 'Unix Makefiles')]
  [--ctest_command (defaults to 'ctest -j4')]
  [--build_command (defaults to 'make -j4')]

  The next four control what kind of builds and tests are done.
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

  [--help] Show this message.
"
  if [ $1 -ne 0 ]; then
      exit $1
  fi
}

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

# Default values for options
prefix="${SOURCE_TREE}/../comprehensive_test_disposeable"

do_clean_as_you_go=yes

generator_string="Unix Makefiles"
ctest_command="ctest -j4"
build_command="make -j4"

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

usage_reported=0

while test $# -gt 0; do

    case $1 in
        --prefix)
	    prefix=$2
	    shift
	    ;;
	--do_clean_as_you_go)
	    case $2 in
		yes|no)
		    do_clean_as_you_go=$2
		    shift
		    ;;
		
		*)
		    usage 1 1>&2
		    ;;
	    esac
	    ;;
        --generator_string)
	    generator_string=$2
	    shift
	    ;;
        --ctest_command)
	    ctest_command=$2
	    shift
	    ;;
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
        --help)
            usage 0 1>&2
	    exit 0;
            ;;
        *)
            if [ $usage_reported -eq 0 ]; then
                usage_reported=1
                usage 0 1>&2
                echo " "
            fi
            echo "Unknown option: $1"
            ;;
    esac
    shift
done

if [ $usage_reported -eq 1 ]; then
    exit 1
fi

hash pkg-config
hash_rc=$?
if [ "$hash_rc" -ne 0 ] ; then
    echo "WARNING: pkg-config not on PATH so setting do_test_traditional_install_tree=no"
    do_test_traditional_install_tree=no
fi

# The question of what to use for the traditional build command is a
# tricky one that depends on platform.  Therefore, we hard code the
# value of this variable rather than allowing the user to change it.
if [ "$generator_string" = "MinGW Makefiles" -o "$generator_string" = "MSYS Makefiles" ] ; then
    # For both these cases the MSYS make command should be used rather than
    # the MinGW mingw32-make command.  But a
    # parallel version of the MSYS make command is problematic.
    # Therefore, specify no -j option.
    traditional_build_command="make"
else
    # For all other cases, the traditional build command should be the
    # same as the build command.
    traditional_build_command="$build_command"
fi

echo "Summary of options used for these tests

prefix=$prefix

do_clean_as_you_go=$do_clean_as_you_go

generator_string=$generator_string"
echo "
ctest_command=$ctest_command
build_command=$build_command
traditional_build_command=$traditional_build_command

cmake_added_options=$cmake_added_options
do_shared=$do_shared
do_nondynamic=$do_nondynamic
do_static=$do_static

do_ctest=$do_ctest
do_test_noninteractive=$do_test_noninteractive
do_test_interactive=$do_test_interactive
do_test_build_tree=$do_test_build_tree
do_test_install_tree=$do_test_install_tree
do_test_traditional_install_tree=$do_test_traditional_install_tree

N.B. do_clean_as_you_go above should be yes unless you don't mind an
accumulation of ~40GB of plot files!  Even with this option set to yes
the high-water mark of disk usage can still be as high as 4GB so be
sure you have enough free disk space to run this test!
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

# Shared + dynamic
if [ "$do_shared" = "yes" ] ; then
    OUTPUT_TREE="$prefix/shared/output_tree"
    BUILD_TREE="$prefix/shared/build_tree"
    INSTALL_TREE="$prefix/shared/install_tree"
    INSTALL_BUILD_TREE="$prefix/shared/install_build_tree"
    comprehensive_test "-DBUILD_SHARED_LIBS=ON"
fi

# Shared + nondynamic
if [ "$do_nondynamic" = "yes" ] ; then
    OUTPUT_TREE="$prefix/nondynamic/output_tree"
    BUILD_TREE="$prefix/nondynamic/build_tree"
    INSTALL_TREE="$prefix/nondynamic/install_tree"
    INSTALL_BUILD_TREE="$prefix/nondynamic/install_build_tree"
    comprehensive_test "-DBUILD_SHARED_LIBS=ON -DENABLE_DYNDRIVERS=OFF"
fi

if [ "$do_static" = "yes" ] ; then
# Static + nondynamic
    OUTPUT_TREE="$prefix/static/output_tree"
    BUILD_TREE="$prefix/static/build_tree"
    INSTALL_TREE="$prefix/static/install_tree"
    INSTALL_BUILD_TREE="$prefix/static/install_build_tree"
    comprehensive_test "-DBUILD_SHARED_LIBS=OFF"
fi

dnl import the whole of SWIG-1.3.21/Tools/config/swig.m4
dnl subsequently AWI (with help from RL) fixed the case where find finds
dnl multiple files, and also removed some bashism's and extended sed
dnl constructs so now there is some hope these macros will work cross-platform.
dnl also, disabled SWIG when version check does not succeed
# Contributed by Sebastian Huber with improvements from Alan W. Irwin and
# Rafael Laboissiere to remove non-portable shell constructs and deal with
# a logic issue when more than one header file or Python library is found.
# They also changed the logic so that swig is always disabled (with
# appropriate message) unless the version check is passed.

# SWIG_PROG([required-version = N[.N[.N]]])
#
# Checks for the SWIG program.  If found you can (and should) call
# SWIG via $(SWIG).  You can use the optional first argument to check
# if the version of the available SWIG is greater than or equal to the
# value of the argument.  It should have the format: N[.N[.N]] (N is a
# number between 0 and 999.  Only the first N is mandatory.)
AC_DEFUN([SWIG_PROG],[
	AC_PATH_PROG([SWIG],[swig])
	if test -z "$SWIG" ; then
		AC_MSG_WARN([cannot find 'swig' program. You should look at http://www.swig.org])
		SWIG='echo "Error: SWIG is not installed. You should look at http://www.swig.org" ; false'
	elif test -n "$1" ; then
		AC_MSG_CHECKING([for SWIG version])
		[swig_version=`$SWIG -version 2>&1 | grep 'SWIG Version' | sed 's/.*\([0-9][0-9]*\.[0-9][0-9]*\.[0-9][0-9]*\).*/\1/g'`]
		AC_MSG_RESULT([$swig_version])
		if test -n "$swig_version" ; then
			# Calculate the required version number
			[major=0`echo $1 | sed 's/\..*//'`]
			[minor=0`echo $1 | sed 's/[0-9]*\.//;s/\..*//'`]
			[patch=0`echo $1 | sed 's/[0-9]*\.[0-9]*\.//' | sed 's/[^0-9].*//'`]
			[swig_required_version=$(( 1000000 * $major  + 1000 * $minor + $patch ))]

			# Calculate the available version number
			[major=0`echo $swig_version | sed 's/\..*//'`]
			[minor=0`echo $swig_version | sed 's/[0-9]*\.//;s/\..*//'`]
			[patch=0`echo $swig_version | sed 's/[0-9]*\.[0-9]*\.//' | sed 's/[^0-9].*//'`]
			[swig_available_version=$(( 1000000 * $major  + 1000 * $minor + $patch ))]

			if test $swig_required_version -gt $swig_available_version ; then
				AC_MSG_WARN([SWIG version >= $1 is required.  You have $swig_version.  You should look at http://www.swig.org])
				SWIG='echo "Error: SWIG version >= $1 is required.  You have '"$swig_version"'.  You should look at http://www.swig.org" ; false'
			else
				AC_MSG_NOTICE([SWIG executable is '$SWIG'])
				SWIG_RUNTIME_LIBS_DIR=`echo $SWIG | sed "s,/bin.*$,/lib,"`
				AC_MSG_NOTICE([SWIG runtime library directory is '$SWIG_RUNTIME_LIBS_DIR'])
			fi
		else
			AC_MSG_WARN([cannot determine SWIG version])
			SWIG='echo "Error: Cannot determine SWIG version.  You should look at http://www.swig.org" ; false'
		fi
	fi
	AC_SUBST([SWIG_RUNTIME_LIBS_DIR])
])

# SWIG_ENABLE_CXX()
#
# Enable SWIG C++ support.  This affects all invocations of $(SWIG).
AC_DEFUN([SWIG_ENABLE_CXX],[
	AC_REQUIRE([SWIG_PROG])
	AC_REQUIRE([AC_PROG_CXX])
	SWIG="$SWIG -c++"
])

# SWIG_MULTI_MODULE_SUPPORT()
#
# Enable support for multiple modules.  This effects all invocations
# of $(SWIG).  You have to link all generated modules against the
# appropriate SWIG runtime library.  If you want to build Python
# modules for example, use the SWIG_PYTHON() macro and link the
# modules against $(SWIG_PYTHON_LIBS).
AC_DEFUN([SWIG_MULTI_MODULE_SUPPORT],[
	AC_REQUIRE([SWIG_PROG])
	SWIG="$SWIG -c"
])

# SWIG_PYTHON([use-shadow-classes = {no, yes}])
#
# Checks for Python and provides the $(SWIG_PYTHON_CPPFLAGS),
# $(SWIG_PYTHON_LIBS) and $(SWIG_PYTHON_OPT) output variables.
# $(SWIG_PYTHON_OPT) contains all necessary SWIG options to generate
# code for Python.  Shadow classes are enabled unless the value of the
# optional first argument is exactly 'no'.  If you need multi module
# support (provided by the SWIG_MULTI_MODULE_SUPPORT() macro) use
# $(SWIG_PYTHON_LIBS) to link against the appropriate library.  It
# contains the SWIG Python runtime library that is needed by the type
# check system for example.
AC_DEFUN([SWIG_PYTHON],[
	AC_REQUIRE([SWIG_PROG])
	AC_REQUIRE([PYTHON_DEVEL])
	test "x$1" != "xno" || swig_shadow=" -noproxy"
	AC_SUBST([SWIG_PYTHON_OPT],[-python$swig_shadow])
	AC_SUBST([SWIG_PYTHON_CPPFLAGS],[$PYTHON_CPPFLAGS])
	AC_SUBST([SWIG_PYTHON_LIBS],["$SWIG_RUNTIME_LIBS_DIR -lswigpy"])
])

# PYTHON_DEVEL()
#
# Checks for Python and tries to get the include path to 'Python.h'.
# It provides the $(PYTHON_CPPFLAGS) and $(PYTHON_LDFLAGS) output variable.
AC_DEFUN([PYTHON_DEVEL],[
	AC_REQUIRE([AM_PATH_PYTHON])

	# Check for Python include path
	AC_MSG_CHECKING([for Python include path])
	python_path=`echo $PYTHON | sed "s,/bin.*$,,"`
	for i in "$python_path/include/python$PYTHON_VERSION/" "$python_path/include/python/" "$python_path/" ; do
		python_path=`find $i -type f -name Python.h -print | sed "1q"`
		if test -n "$python_path" ; then
			break
		fi
	done
	python_path=`echo $python_path | sed "s,/Python.h$,,"`
	AC_MSG_RESULT([$python_path])
	if test -z "$python_path" ; then
		AC_MSG_ERROR([cannot find Python include path])
	fi
	AC_SUBST([PYTHON_CPPFLAGS],[-I$python_path])

	# Check for Python library path
	AC_MSG_CHECKING([for Python library path])
	python_path=`echo $PYTHON | sed "s,/bin.*$,,"`
	for i in "$python_path/lib/python$PYTHON_VERSION/config/" "$python_path/lib/python$PYTHON_VERSION/" "$python_path/lib/python/config/" "$python_path/lib/python/" "$python_path/" ; do
		python_path=`find $i -type f -name libpython$PYTHON_VERSION.* -print | sed "1q"`
		if test -n "$python_path" ; then
			break
		fi
	done
	python_path=`echo $python_path | sed "s,/libpython.*$,,"`
	AC_MSG_RESULT([$python_path])
	if test -z "$python_path" ; then
		AC_MSG_ERROR([cannot find Python library path])
	fi
	AC_SUBST([PYTHON_LDFLAGS],["-L$python_path -lpython$PYTHON_VERSION"])
])

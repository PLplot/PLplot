dnl @synopsis AC_PYTHON_DEVEL()
dnl
dnl Checks for Python and tries to get the include path to 'Python.h'.
dnl It provides the $(PYTHON_CPPFLAGS) and $(PYTHON_LDFLAGS) output variable.
dnl
dnl @authors Sebastian Huber <sebastian-huber@web.de>, Alan W. Irwin
dnl <irwin@beluga.phys.uvic.ca>, Rafael Laboissiere <laboissiere@psy.mpg.de> and
dnl Andrew Collier <colliera@nu.ac.za>.
dnl
dnl
AC_DEFUN([AC_PYTHON_DEVEL],[
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
	PYTHON_EXTRA_LIBS=`$PYTHON -c "import distutils.sysconfig; \
                conf = distutils.sysconfig.get_config_var; \
                print conf('LOCALMODLIBS')+' '+conf('LIBS')"`
	AC_SUBST(PYTHON_EXTRA_LIBS)
])

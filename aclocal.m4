dnl> $Id$
dnl> 
dnl> Local autoconf extensions.  These are based on the autoconf builtin
dnl> macros, and you can do what you want with them.  Side note: if you
dnl> introduce all comments as done here, the m4 file is more readable and
dnl> you can do emacs adaptive fills.
dnl> 
dnl> Maurice LeBrun
dnl> IFS, University of Texas at Austin
dnl> 14-Jul-1994
dnl> ------------------------------------------------------------------------
dnl> It's kind of nice to have an include macro.
dnl> 
define([AC_INCLUDE],[builtin([include],$*)])dnl
dnl> ------------------------------------------------------------------------
dnl> This quicky is good during development, e.g. AC_IGNORE([ ... ]) to
dnl> ignore everything inside the brackets.
dnl>
define([AC_IGNORE],)dnl
dnl> ------------------------------------------------------------------------
dnl> Guess the value for the `prefix' variable by looking for the argument
dnl> program along PATH and taking its parent.  Example: if the argument
dnl> is `gcc' and we find /usr/local/gnu/bin/gcc, set `prefix' to
dnl> /usr/local/gnu.  Unlike the builtin AC_PREFIX, this one tries to
dnl> follow any symbolic links back to their terminus, and also determines
dnl> if a non-standard installation is being used.  It works as follows:
dnl>
dnl>  	prefix=`ls -l $ac_dir/$1 | awk '{print $NF}'`
dnl>
dnl> This stores the last space-delimited field of a long directory
dnl> list of the file in quesion into "prefix".  If the file is really
dnl> a softlink, this will be the actual file location, unless that
dnl> points to yet another link (and if so, tough luck).
dnl>
dnl>	prefix=`echo $prefix | sed 's%/[^/][^/]*$%%'`
dnl>
dnl> This strips off the last part of the file name.  "prefix" should
dnl> now hold the absolute pathname of the containing directory.
dnl>
dnl>	dirname=`echo $prefix | sed 's%/.*/%%'`
dnl>
dnl> This strips off the last "/"-delimited field and stores into
dnl> "dirname".  
dnl>
dnl>	if test $dirname = "bin"; then
dnl>	    prefix=`echo $prefix | sed 's%/[^/][^/]*$%%'`
dnl>	fi
dnl>
dnl> This tests to see if the immediate parent of the file is named "bin"
dnl> as is customary.  If so, the "bin" is stripped off to get the actual
dnl> prefix.  If not, the prefix is left alone, and a warning is issued.
dnl>
define([AC_PREFIX],
[if test -z "$prefix"
then
  AC_CHECKING([for $1 to derive installation directory prefix])
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="$IFS:"
  for ac_dir in $PATH; do
    test -z "$ac_dir" && ac_dir=.
    if test $ac_dir != . && test -f $ac_dir/$1; then
changequote(,)dnl
      # Not all systems have dirname.
      prefix=`ls -l $ac_dir/$1 | awk '{print $NF}'`
      prefix=`echo $prefix | sed 's%/[^/][^/]*$%%'`
      dirname=`echo $prefix | sed 's%/.*/%%'`
      if test $dirname = "bin"; then
        prefix=`echo $prefix | sed 's%/[^/][^/]*$%%'`
      else
        echo ""
	echo "dirname is: $dirname"
        echo "Warning: Non-standard installed distribution ($1 not stored under bin)."
        echo "Please completely delete old files before installation."
      fi
changequote([,])dnl
      break
    fi
  done
  IFS="$ac_save_ifs"
  AC_VERBOSE(chose installation directory prefix ${prefix})
fi
])dnl
dnl> ------------------------------------------------------------------------
dnl> Fixed version of AC_ISC_POSIX: /etc/conf/kconfig.d doesn't need to
dnl> exist for it to be POSIX.  Furthermore, unistd.h is in /usr/include
dnl> on some systems, not /usr/include/sys.  The redefinition of CC is not
dnl> important, since I #define _POSIX_SOURCE in the code.
dnl>
define([AC_ISC_POSIX],
[AC_PROVIDE([$0])AC_BEFORE([$0], [AC_COMPILE_CHECK])AC_BEFORE([$0], [AC_TEST_PROGRAM])AC_BEFORE([$0], [AC_HEADER_EGREP])AC_BEFORE([$0], [AC_TEST_CPP])AC_CHECKING(for POSIXized ISC)
if \
     grep _POSIX_VERSION [/usr/include/unistd.h] >/dev/null 2>&1 ||
     grep _POSIX_VERSION [/usr/include/sys/unistd.h] >/dev/null 2>&1
then
  echo "yup, it's POSIX"
  ISC=1 # If later tests want to check for ISC.
  AC_DEFINE(_POSIX_SOURCE)
  if test -n "$GCC"; then
    CC="$CC -posix"
  else
    CC="$CC -Xp"
  fi
else
 echo "nope, not POSIX"
fi
])dnl
dnl> ------------------------------------------------------------------------
dnl> Some X11 headers require "caddr_t" even though on a POSIX.1 system
dnl> this is illegal.  This macro defines it if not already typedef'ed.
dnl>
define(AC_CADDR_T,
[AC_PROVIDE([$0])AC_CHECKING(for caddr_t in sys/types.h)
AC_HEADER_EGREP(caddr_t, sys/types.h, , AC_DEFINE(caddr_t, char *))])dnl
dnl

dnl $Id$ --*-Autoconf-*--
dnl
dnl Local autoconf extensions.  These are based on the autoconf builtin
dnl macros, and you can do what you want with them.
dnl
dnl Maurice LeBrun
dnl IFS, University of Texas at Austin
dnl 14-Jul-1994
dnl
dnl Copyright (C) 2003, 2004  Rafael Laboissiere
dnl Copyright (C) 2004  Alan W. Irwin
dnl
dnl This file is part of PLplot.
dnl
dnl PLplot is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Library Public License as published
dnl by the Free Software Foundation; either version 2 of the License, or
dnl (at your option) any later version.
dnl
dnl PLplot is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU Library General Public License for more details.
dnl
dnl You should have received a copy of the GNU Library General Public License
dnl along with PLplot; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
dnl
dnl ------------------------------------------------------------------------
dnl Include macro which knows about the cf/ directory
dnl
define([PL_INCLUDE],[builtin([include],[cf/$1])])dnl
dnl
dnl ------------------------------------------------------------------------
dnl This quicky is good during development, e.g. PL_IGNORE([ ... ]) to
dnl ignore everything inside the brackets.
dnl
define([PL_IGNORE],)dnl
dnl
dnl ------------------------------------------------------------------------
dnl The following macros search a list of directories for the given
dnl include file and takes appropriate actions if found or not.
dnl Arguments:
dnl 	$1 - the include file name, the part before the .h
dnl	$2 - a variable that holds the matched directory name
dnl	$3 - a variable indicating if the search succeeded ("yes"/"no")
dnl	     (if missing, we exit)
dnl Use just PL_FIND_INC, or the PL_FIND_INC_<...> set for more control.
dnl
define(PL_FIND_INC_BEGIN, [
    AC_MSG_CHECKING(for $1.h)
    $2=""
])
define(PL_FIND_INC_SET, [
    for dir in $incdirs; do
	if test -r "$dir/$1.h"; then
	    $2="$dir"
	    AC_MSG_RESULT($dir/$1.h)
	    break
	fi
    done
])
define(PL_FIND_INC_END, [
    if test -z "$$2"; then
	ifelse($3,,[
	    AC_MSG_RESULT(not found -- exiting)
	    exit 1
	],[
	    AC_MSG_RESULT(no)
	    AC_MSG_RESULT([warning: can't find $1.h, setting $3 to no])
	    $3="no"
	])
    fi
    if test "$$2" = "/usr/include"; then
	$2="default"
    fi
])
define(PL_FIND_INC, [
    PL_FIND_INC_BEGIN($*)
    PL_FIND_INC_SET($*)
    PL_FIND_INC_END($*)
])
dnl ------------------------------------------------------------------------
dnl The following macro searches a list of directories for the given
dnl library file and takes appropriate actions if found or not.
dnl Use just PL_FIND_LIB, or the PL_FIND_LIB_<...> set for more control.
dnl
dnl Arguments:
dnl 	$1 - the library name, the part after the -l and before the "."
dnl	$2 - a variable that holds the matched directory name
dnl
dnl PL_FIND_LIB_SET takes:
dnl	$3 - a variable that holds the matched library name in a form
dnl	     suitable for input to the linker (without the suffix, so that
dnl	     any shared library form is given preference).
dnl
dnl PL_FIND_LIB_END takes:
dnl	$3 - a variable indicating if the search succeeded ("yes"/"no")
dnl	     (if missing, we exit)
dnl
dnl PL_FIND_LIB takes these as $3 and $4, respectively.
dnl
define(PL_FIND_LIB_BEGIN, [
    AC_MSG_CHECKING(for lib$1)
    $2=""
])
define(PL_FIND_LIB_SET, [
    for dir in $libdirs; do
	if test -z "$LIBEXTNS"; then
	    LIBEXTNS="so a"
	fi
	for suffix in $LIBEXTNS; do
	    if test -f "$dir/lib$1.$suffix"; then
		$2="$dir"
		$3="-l$1"
		AC_MSG_RESULT($dir/lib$1.$suffix)
		break 2
	    fi
	done
    done
])
define(PL_FIND_LIB_END, [
    if test -z "$$2"; then
	ifelse($3,,[
	    AC_MSG_ERROR(not found -- exiting)
	],[
	    AC_MSG_RESULT(no)
	    AC_MSG_WARN([can't find lib$1, setting $3 to no])
	    $3="no"
	])
    fi
    if test "$$2" = "/usr/lib"; then
	$2="default"
    fi
])
define(PL_FIND_LIB, [
    PL_FIND_LIB_BEGIN($1, $2)
    PL_FIND_LIB_SET($1, $2, $3)
    PL_FIND_LIB_END($1, $2, $4)
])
dnl ------------------------------------------------------------------------
dnl The following macro makes it easier to add includes without adding
dnl redundant -I specifications (to keep the build line concise).
dnl Arguments:
dnl 	$1 - the searched directory name
dnl	$2 - a variable that holds the include specification
dnl	$3 - a variable that holds all the directories searched so far
dnl
define([PL_ADD_TO_INCS],[
    INCSW=""
    if test "$1" != "default"; then
	INCSW="-I$1"
    fi
    for dir in $$3; do
	if test "$1" = "$dir"; then
	    INCSW=""
	    break
	fi
    done
    if test -n "$INCSW"; then
	$2="$$2 $INCSW"
    fi
    $3="$$3 $1"
])
dnl ------------------------------------------------------------------------
dnl The following macro makes it easier to add libs without adding
dnl redundant -L and -l specifications (to keep the build line concise).
dnl Arguments:
dnl 	$1 - the searched directory name
dnl	$2 - the command line option to give to the linker (e.g. -lfoo)
dnl	$3 - a variable that holds the library specification
dnl	$4 - a variable that holds all the directories searched so far
dnl
define([PL_ADD_TO_LIBS],[
    LIBSW=""
    if test "$1" != "default"; then
	LIBSW="-L$1"
    fi
    for dir in $$4; do
	if test "$1" = "$dir"; then
	    LIBSW=""
	    break
	fi
    done
    LIBL="$2"
    for lib in $$3; do
	if test "$2" = "$lib"; then
	    LIBL=""
	    break
	fi
    done
    if test -n "$LIBSW"; then
	$3="$$3 $LIBSW $LIBL"
    else
	$3="$$3 $LIBL"
    fi
    $4="$$4 $1"
])
dnl ------------------------------------------------------------------------
dnl Front-end macros to the above, which when:
dnl  - a with-<opt> is registered, the corresponding enable-<opt> is
dnl	registered as bogus
dnl  - an enable-<opt> is registered, the corresponding with-<opt> is
dnl	registered as bogus
dnl
AC_DEFUN([MY_ARG_WITH],
[AC_ARG_WITH($@)dnl
AC_ARG_ENABLE([$1],[],[AC_MSG_ERROR([unrecognized variable: enable_$1])])])
dnl
AC_DEFUN([MY_ARG_ENABLE],
[AC_ARG_ENABLE($@)dnl
AC_ARG_WITH([$1],[],[AC_MSG_ERROR([unrecognized variable: with_$1])])])
dnl ------------------------------------------------------------------------
dnl Get rid of caching since it doesn't always work.  I.e. changing the
dnl compiler from the vendor's to gcc can change all sorts of settings,
dnl but the autoconf logic isn't set up to handle that.  I'll opt for
dnl stability over speed any day.
dnl
define([AC_CACHE_LOAD],)
define([AC_CACHE_SAVE],)
dnl ------------------------------------------------------------------------
dnl Gnome configuration function.
dnl Adapted from that distributed by the Gnome Project
dnl Added by Rafael Laboissiere on Fri Feb 23 21:26:56 CET 2001
dnl
dnl GNOME_INIT_HOOK (script-if-gnome-enabled, [failflag], [additional-inits])
dnl
dnl if failflag is "fail" then GNOME_INIT_HOOK will abort if gnomeConf.sh
dnl is not found.
dnl

AC_DEFUN([GNOME_INIT_HOOK],[
	AC_SUBST(GNOME_LIBS)
	AC_SUBST(GNOMEUI_LIBS)
dnl	AC_SUBST(GNOMEGNORBA_LIBS)
dnl	AC_SUBST(GTKXMHTML_LIBS)
dnl	AC_SUBST(ZVT_LIBS)
	AC_SUBST(GNOME_LIBDIR)
	AC_SUBST(GNOME_INCLUDEDIR)

	AC_ARG_WITH(gnome-includes,
	[  --with-gnome-includes   Specify location of GNOME headers],[
	CFLAGS="$CFLAGS -I$withval"
	])

	AC_ARG_WITH(gnome-libs,
	[  --with-gnome-libs       Specify location of GNOME libs],[
	LDFLAGS="$LDFLAGS -L$withval"
	gnome_prefix=$withval
	])

	AC_ARG_WITH(gnome,
	[  --with-gnome            Specify prefix for GNOME files],
		if test x$withval = xyes; then
	    		want_gnome=yes
	    		dnl Note that an empty true branch is not
			dnl valid sh syntax.
	    		ifelse([$1], [], :, [$1])
        	else
	    		if test "x$withval" = xno; then
	        		want_gnome=no
	    		else
	        		want_gnome=yes
	    			LDFLAGS="$LDFLAGS -L$withval/lib"
	    			CFLAGS="$CFLAGS -I$withval/include"
	    			gnome_prefix=$withval/lib
	    		fi
  		fi,
		want_gnome=yes)

	if test "x$want_gnome" = xyes; then

	    AC_PATH_PROG(GNOME_CONFIG,gnome-config,no)
	    if test "$GNOME_CONFIG" = "no"; then
	      no_gnome_config="yes"
	    else
	      AC_MSG_CHECKING(if $GNOME_CONFIG works)
	      if $GNOME_CONFIG --libs-only-l gnome >/dev/null 2>&1; then
	        AC_MSG_RESULT(yes)
dnl	        GNOME_GNORBA_HOOK([],$2)
	        GNOME_LIBS="`$GNOME_CONFIG --libs-only-l gnome`"
	        GNOMEUI_LIBS="`$GNOME_CONFIG --libs-only-l gnomeui`"
dnl	        GNOMEGNORBA_LIBS="`$GNOME_CONFIG --libs-only-l gnorba gnomeui`"
dnl	        GTKXMHTML_LIBS="`$GNOME_CONFIG --libs-only-l gtkxmhtml`"
dnl		ZVT_LIBS="`$GNOME_CONFIG --libs-only-l zvt`"
	        GNOME_LIBDIR="`$GNOME_CONFIG --libs-only-L gnorba gnomeui`"
	        GNOME_INCLUDEDIR="`$GNOME_CONFIG --cflags gnorba gnomeui`"
                $1
	      else
	        AC_MSG_RESULT(no)
	        no_gnome_config="yes"
              fi
            fi

	    if test x$exec_prefix = xNONE; then
	        if test x$prefix = xNONE; then
		    gnome_prefix=$ac_default_prefix/lib
	        else
 		    gnome_prefix=$prefix/lib
	        fi
	    else
	        gnome_prefix=`eval echo \`echo $libdir\``
	    fi

	    if test "$no_gnome_config" = "yes"; then
              AC_MSG_CHECKING(for gnomeConf.sh file in $gnome_prefix)
	      if test -f $gnome_prefix/gnomeConf.sh; then
	        AC_MSG_RESULT(found)
	        echo "loading gnome configuration from" \
		     "$gnome_prefix/gnomeConf.sh"
	        . $gnome_prefix/gnomeConf.sh
	        $1
	      else
	        AC_MSG_RESULT(not found)
 	        if test x$2 = xfail; then
	          AC_MSG_ERROR(Could not find the gnomeConf.sh file that is generated by gnome-libs install)
 	        fi
	      fi
            fi
	fi

	if test -n "$3"; then
	  n="$3"
	  for i in $n; do
	    AC_MSG_CHECKING(extra library \"$i\")
	    case $i in
	      applets)
		AC_SUBST(GNOME_APPLETS_LIBS)
		GNOME_APPLETS_LIBS=`$GNOME_CONFIG --libs-only-l applets`
		AC_MSG_RESULT($GNOME_APPLETS_LIBS);;
	      capplet)
		AC_SUBST(GNOME_CAPPLET_LIBS)
		GNOME_CAPPLET_LIBS=`$GNOME_CONFIG --libs-only-l capplet`
		AC_MSG_RESULT($GNOME_CAPPLET_LIBS);;
	      *)
		AC_MSG_RESULT(unknown library)
	    esac
	  done
	fi
])

dnl
dnl GNOME_INIT ([additional-inits])
dnl

AC_DEFUN([GNOME_INIT],[
	GNOME_INIT_HOOK([],fail,$1)
])
dnl ------------------------------------------------------------------------
dnl AC substitution of stripped paths
dnl     AC_SUBST_STRIP(PATH,PREFIX)
dnl Essentially, do an AC_SUBST of PATH variable, but previously suppress
dnl the leading PREFIX.
dnl This is needed for relocatability, i.e. installing the package in a
dnl different prefix from that used at build time.
dnl Added by Rafael Laboissiere on Wed Mar 21 22:57:57 CET 2001
AC_DEFUN([AC_SUBST_STRIP],[
  $1=`echo $]$1[ | sed s%$]$2[/%%`
  AC_SUBST($1)
])
dnl Available from the GNU Autoconf Macro Archive at:
dnl http://www.gnu.org/software/ac-archive/htmldoc/check_gnu_make.html
dnl
AC_DEFUN(
        [CHECK_GNU_MAKE], [ AC_CACHE_CHECK( for GNU make,_cv_gnu_make_command,
                _cv_gnu_make_command='' ;
dnl Search all the common names for GNU make
                for a in "$MAKE" make gmake gnumake ; do
                        if test -z "$a" ; then continue ; fi ;
                        if  ( sh -c "$a --version" 2> /dev/null | grep GNU  2>&1 > /dev/null ) ;  then
                                _cv_gnu_make_command=$a ;
                                break;
                        fi
                done ;
        ) ;
dnl If there was a GNU version, then set @ifGNUmake@ to the empty string, '#' otherwise
        if test  "x$_cv_gnu_make_command" != "x"  ; then
                ifGNUmake='' ;
        else
                ifGNUmake='#' ;
                AC_MSG_RESULT("Not found");
        fi
        AC_SUBST(ifGNUmake)
] )
dnl ------------------------------------------------------------------------
dnl Determine the dlname of a library to be installed by libtool
dnl     PL_GET_DLNAME(STEM,VERSION_INFO,VARIABLE)
dnl For a given library STEM and a given VERSION_INFO (a la
dnl -version-info option of libtool), determine the dlname of the
dnl library in the form lib$STEM.<so_ext>.<so_number>.  Set the
dnl variable VARIABLE with the resulting value.  This macro should be used
dnl only after the call to AM_PROG_LIBTOOL.
AC_DEFUN([PL_GET_DLNAME],[
  if test -z "$LIBTOOL" -a -z "$CC" ; then
    AC_MSG_ERROR([Dlname guessings can be done only after libtool is initialized])
  else
    TMP_DIR=./tmp-cfg
    rm -rf $TMP_DIR
    mkdir -p $TMP_DIR
    cd $TMP_DIR
    ../libtool --mode=link $CC -rpath /usr/lib -version-info $2 \
        -o lib$1.la > /dev/null
    $3=`grep ^dlname= lib$1.la | sed "s/dlname='\(.*\)'/\1/"`
    cd ..
    rm -rf $TMP_DIR
  fi
])
dnl ------------------------------------------------------------------------
dnl Determine the dlname of a DLL to be installed by libtool
dnl This is a usefule variation of GET_DLNAME above for dynamically loaded
dnd libraries (DLL's).
dnl     PL_GET_DLLNAME(STEM,VARIABLE)
dnl For a given DLL STEM determine the dlname of the
dnl library in the form $STEM.<so_ext>.  Set the
dnl variable VARIABLE with the resulting value.  This macro should be used
dnl only after the call to AM_PROG_LIBTOOL.
AC_DEFUN([PL_GET_DLLNAME],[
  if test -z "$LIBTOOL" -a -z "$CC" ; then
    AC_MSG_ERROR([Dlname guessings can be done only after libtool is initialized])
  else
    TMP_DIR=./tmp-cfg
    rm -rf $TMP_DIR
    mkdir -p $TMP_DIR
    cd $TMP_DIR
    ../libtool --mode=link $CC -rpath /usr/lib -avoid-version -module \
        -o $1.la > /dev/null
    $2=`grep ^dlname= $1.la | sed "s/dlname='\(.*\)'/\1/"`
    cd ..
    rm -rf $TMP_DIR
  fi
])
dnl ------------------------------------------------------------------------
dnl PL_EXPAND_EXPRESSION(VARIABLE,EXPRESSION)
dnl
dnl Expand recursively a given EXPRESSION, until no variable expansion is
dnl possible.  Put the result in VARIABLE.
AC_DEFUN([PL_EXPAND_EXPRESSION],[
  str1="$2"
  while true ; do
   str2=`eval echo $str1`
     test "$str1" = "$str2" && break
     str1="$str2"
   done
   $1="$str2"
])

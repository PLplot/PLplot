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
define([AC_PREFIX_PROGRAM],
[if test "$prefix" = NONE; then
  AC_MSG_CHECKING(for prefix by location of $1)
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
	AC_MSG_RESULT($prefix)
      else
	AC_MSG_RESULT($prefix)
	AC_MSG_RESULT(warning: non-standard installed distribution ($1 not stored under bin))
	AC_MSG_RESULT(warning: please completely delete old files before installation)
      fi
changequote([,])dnl
      break
    fi
  done
  if test "$prefix" = NONE; then
    AC_MSG_RESULT(not found -- using default)
  fi
  IFS="$ac_save_ifs"
fi
])dnl
dnl> ------------------------------------------------------------------------
dnl> Fixed version of AC_ISC_POSIX: /etc/conf/kconfig.d doesn't need to
dnl> exist for it to be POSIX.  Furthermore, unistd.h is in /usr/include
dnl> on some systems, not /usr/include/sys.  Also, the distributed version
dnl> of AC_ISC_POSIX sets the value of CC (to be $CC -posix or $CC -Xp,
dnl> which is a bad idea: these choices are not particularly portable and
dnl> it overrides whatever the user may have set for CC.
dnl>
define([AC_ISC_POSIX],
[AC_PROVIDE([$0])AC_BEFORE([$0], dnl
[AC_COMPILE_CHECK])AC_BEFORE([$0], [AC_TEST_PROGRAM])AC_BEFORE([$0], dnl
[AC_HEADER_EGREP])AC_BEFORE([$0], [AC_TEST_CPP])dnl
AC_MSG_CHECKING(for POSIXized ISC)
if \
     grep _POSIX_VERSION [/usr/include/unistd.h] >/dev/null 2>&1 ||
     grep _POSIX_VERSION [/usr/include/sys/unistd.h] >/dev/null 2>&1
then
  AC_MSG_RESULT([yes])
  ISC=1 # If later tests want to check for ISC.
  AC_DEFINE(_POSIX_SOURCE)
else
  AC_MSG_RESULT([no])
fi
])dnl
dnl> ------------------------------------------------------------------------
dnl> Modified versions of two subroutines of AC_OUTPUT from autoconf
dnl> version 2.3. 
dnl>
dnl> The only difference is that this version looks for the input file
dnl> in the in the target directory first, then in the source directory.

dnl This is a subroutine of AC_OUTPUT.  It is called inside an unquoted
dnl here document whose contents are going into config.status.
dnl AC_OUTPUT_FILES(FILE...)
define([AC_OUTPUT_FILES],
[# Protect against being on the right side of a sed subst in config.status. 
changequote(, )dnl
sed 's/%@/@@/; s/@%/@@/; s/%g$/@g/; /@g$/s/[\\\\&%]/\\\\&/g; 
 s/@@/%@/; s/@@/@%/; s/@g$/%g/' > conftest.subs <<\CEOF
changequote([, ])dnl
dnl These here document variables are unquoted when configure runs
dnl but quoted when config.status runs, so variables are expanded once.
$ac_vpsub
dnl Shell code in configure.in might set extrasub.
$extrasub
dnl Insert the sed substitutions of variables.
undivert(AC_DIVERSION_SED)
CEOF
EOF
cat >> $CONFIG_STATUS <<EOF

CONFIG_FILES=\${CONFIG_FILES-"$1"}
EOF
cat >> $CONFIG_STATUS <<\EOF
for ac_file in .. $CONFIG_FILES; do if test "x$ac_file" != x..; then
dnl Specifying an input file breaks the trap to clean up on interrupt,
dnl but that's not a huge problem.
  # Support "outfile[:infile]", defaulting infile="outfile.in".
  case "$ac_file" in
  *:*) ac_file_in=`echo "$ac_file"|sed 's%.*:%%'`
       ac_file=`echo "$ac_file"|sed 's%:.*%%'` ;;
  *) ac_file_in="${ac_file}.in" ;;
  esac

  # Adjust relative srcdir, etc. for subdirectories.

  # Remove last slash and all that follows it.  Not all systems have dirname.
changequote(, )dnl
  ac_dir=`echo $ac_file|sed 's%/[^/][^/]*$%%'`
changequote([, ])dnl
  if test "$ac_dir" != "$ac_file" && test "$ac_dir" != .; then
    # The file is in a subdirectory.
    test ! -d "$ac_dir" && mkdir "$ac_dir"
    ac_dir_suffix="/`echo $ac_dir|sed 's%^\./%%'`"
    # A "../" for each directory in $ac_dir_suffix.
changequote(, )dnl
    ac_dots=`echo $ac_dir_suffix|sed 's%/[^/]*%../%g'`
changequote([, ])dnl
  else
    ac_dir_suffix= ac_dots=
  fi

  case "$ac_given_srcdir" in
  .)  srcdir=.
      if test -z "$ac_dots"; then top_srcdir=.
      else top_srcdir=`echo $ac_dots|sed 's%/$%%'`; fi ;;
  /*) srcdir="$ac_given_srcdir$ac_dir_suffix"; top_srcdir="$ac_given_srcdir" ;;
  *) # Relative path.
    srcdir="$ac_dots$ac_given_srcdir$ac_dir_suffix"
    top_srcdir="$ac_dots$ac_given_srcdir" ;;
  esac

ifdef([AC_PROVIDE_AC_PROG_INSTALL],
[  case "$ac_given_INSTALL" in
changequote(, )dnl
  [/$]*) INSTALL="$ac_given_INSTALL" ;;
changequote([, ])dnl
  *) INSTALL="$ac_dots$ac_given_INSTALL" ;;
  esac
])dnl
  echo creating "$ac_file"
  rm -f "$ac_file"
  configure_input="Generated automatically from `echo $ac_file_in|sed 's%.*/%%'` by configure."
  case "$ac_file" in
  *Makefile*) ac_comsub="1i\\
# $configure_input" ;;
  *) ac_comsub= ;;
  esac
  sed -e "$ac_comsub
s%@configure_input@%$configure_input%g
s%@srcdir@%$srcdir%g
s%@top_srcdir@%$top_srcdir%g
ifdef([AC_PROVIDE_AC_PROG_INSTALL], [s%@INSTALL@%$INSTALL%g
])dnl
" -f conftest.subs `if test -f $ac_file_in; then echo $ac_file_in; else echo $ac_given_srcdir/$ac_file_in; fi` > $ac_file
fi; done
rm -f conftest.subs
])

dnl This is a subroutine of AC_OUTPUT.  It is called inside a quoted
dnl here document whose contents are going into config.status.
dnl AC_OUTPUT_HEADER(HEADER-FILE...)
define([AC_OUTPUT_HEADER],
[changequote(<<, >>)dnl
# These sed commands are passed to sed as "A NAME B NAME C VALUE D", where
# NAME is the cpp macro being defined and VALUE is the value it is being given.
#
# ac_d sets the value in "#define NAME VALUE" lines.
ac_dA='s%^\([ 	]*\)#\([ 	]*define[ 	][ 	]*\)'
ac_dB='\([ 	][ 	]*\)[^ 	]*%\1#\2'
ac_dC='\3'
ac_dD='%g'
# ac_u turns "#undef NAME" with trailing blanks into "#define NAME VALUE".
ac_uA='s%^\([ 	]*\)#\([ 	]*\)undef\([ 	][ 	]*\)'
ac_uB='\([ 	]\)%\1#\2define\3'
ac_uC=' '
ac_uD='\4%g'
# ac_e turns "#undef NAME" without trailing blanks into "#define NAME VALUE".
ac_eA='s%^\([ 	]*\)#\([ 	]*\)undef\([ 	][ 	]*\)'
ac_eB='<<$>>%\1#\2define\3'
ac_eC=' '
ac_eD='%g'
changequote([, ])dnl

CONFIG_HEADERS=${CONFIG_HEADERS-"$1"}
for ac_file in .. $CONFIG_HEADERS; do if test "x$ac_file" != x..; then
  # Support "outfile[:infile]", defaulting infile="outfile.in".
  case "$ac_file" in
  *:*) ac_file_in=`echo "$ac_file"|sed 's%.*:%%'`
       ac_file=`echo "$ac_file"|sed 's%:.*%%'` ;;
  *) ac_file_in="${ac_file}.in" ;;
  esac

  echo creating $ac_file

  rm -f conftest.frag conftest.in conftest.out
  cp `if test -f $ac_file_in; then echo $ac_file_in; else echo $ac_given_srcdir/$ac_file_in; fi` conftest.in

EOF

# Transform confdefs.h into a sed script conftest.vals that substitutes
# the proper values into config.h.in to produce config.h.  And first:
# Protect against being on the right side of a sed subst in config.status. 
# Protect against being in an unquoted here document in config.status.
rm -f conftest.vals
dnl Using a here document instead of a string reduces the quoting nightmare.
dnl Putting comments in sed scripts is not portable.
cat > conftest.hdr <<\EOF
changequote(<<, >>)dnl
s/[\\&%]/\\&/g
s%[\\$`]%\\&%g
s%<<#define>> \([A-Za-z_][A-Za-z0-9_]*\) \(.*\)%${ac_dA}\1${ac_dB}\1${ac_dC}\2${ac_dD}%gp
s%ac_d%ac_u%gp
s%ac_u%ac_e%gp
changequote([, ])dnl
EOF
sed -n -f conftest.hdr confdefs.h > conftest.vals
rm -f conftest.hdr

# This sed command replaces #undef with comments.  This is necessary, for
# example, in the case of _POSIX_SOURCE, which is predefined and required
# on some systems where configure will not decide to define it.
cat >> conftest.vals <<\EOF
changequote(, )dnl
s%^[ 	]*#[ 	]*undef[ 	][ 	]*[a-zA-Z_][a-zA-Z_0-9]*%/* & */%
changequote([, ])dnl
EOF

# Break up conftest.vals because some shells have a limit on
# the size of here documents, and old seds have small limits too.
# Maximum number of lines to put in a single here document.
ac_max_here_lines=12

rm -f conftest.tail
while :
do
  ac_lines=`grep -c . conftest.vals`
  # grep -c gives empty output for an empty file on some AIX systems.
  if test -z "$ac_lines" || test "$ac_lines" -eq 0; then break; fi
  # Write a limited-size here document to conftest.frag.
  echo '  cat > conftest.frag <<CEOF' >> $CONFIG_STATUS
  sed ${ac_max_here_lines}q conftest.vals >> $CONFIG_STATUS
  echo 'CEOF
  sed -f conftest.frag conftest.in > conftest.out
  rm -f conftest.in
  mv conftest.out conftest.in
' >> $CONFIG_STATUS
  sed 1,${ac_max_here_lines}d conftest.vals > conftest.tail
  rm -f conftest.vals
  mv conftest.tail conftest.vals
done
rm -f conftest.vals

dnl Now back to your regularly scheduled config.status.
cat >> $CONFIG_STATUS <<\EOF
  rm -f conftest.frag conftest.h
  echo "/* $ac_file.  Generated automatically by configure.  */" > conftest.h
  cat conftest.in >> conftest.h
  rm -f conftest.in
  if cmp -s $ac_file conftest.h 2>/dev/null; then
    echo "$ac_file is unchanged"
    rm -f conftest.h
  else
    rm -f $ac_file
    mv conftest.h $ac_file
  fi
fi; done

])
dnl> ------------------------------------------------------------------------
dnl> The following macros search a list of directories for the given
dnl> include file and takes appropriate actions if found or not.
dnl> Arguments: 
dnl> 	$1 - the include file name, the part before the .h
dnl>	$2 - a variable that holds the matched directory name
dnl>	$3 - a variable indicating if the search succeeded ("yes"/"no") 
dnl>	     (if missing, we exit)
dnl> Use just FIND_INC, or the FIND_INC_<...> set for more control.
dnl
define(FIND_INC_BEGIN, [
    AC_MSG_CHECKING(for $1.h)
    $2=""
])
define(FIND_INC_SET, [
    for dir in $incdirs; do
	if test -r "$dir/$1.h"; then
	    $2="$dir"
	    AC_MSG_RESULT($dir/$1.h)
	    break
	fi
    done
])
define(FIND_INC_END, [
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
define(FIND_INC, [
    FIND_INC_BEGIN($*)
    FIND_INC_SET($*)
    FIND_INC_END($*)
])
dnl> ------------------------------------------------------------------------
dnl> The following macro searches a list of directories for the given
dnl> library file and takes appropriate actions if found or not.
dnl> Use just FIND_LIB, or the FIND_LIB_<...> set for more control.
dnl>
dnl> Arguments: 
dnl> 	$1 - the library name, the part after the -l and before the "."
dnl>	$2 - a variable that holds the matched directory name
dnl>
dnl> FIND_LIB_SET takes:
dnl>	$3 - a variable that holds the matched library name in a form
dnl>	     suitable for input to the linker (without the suffix, so that
dnl>	     any shared library form is given preference).
dnl>
dnl> FIND_LIB_END takes:
dnl>	$3 - a variable indicating if the search succeeded ("yes"/"no") 
dnl>	     (if missing, we exit)
dnl>
dnl> FIND_LIB takes these as $3 and $4, respectively.
dnl
define(FIND_LIB_BEGIN, [
    AC_MSG_CHECKING(for lib$1)
    $2=""
])
define(FIND_LIB_SET, [
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
define(FIND_LIB_END, [
    if test -z "$$2"; then
	ifelse($3,,[
	    AC_MSG_RESULT(not found -- exiting)
	    exit 1
	],[
	    AC_MSG_RESULT(no)
	    AC_MSG_RESULT([warning: can't find lib$1, setting $3 to no])
	    $3="no"
	])
    fi
    if test "$$2" = "/usr/lib"; then
	$2="default"
    fi
])
define(FIND_LIB, [
    FIND_LIB_BEGIN($1, $2)
    FIND_LIB_SET($1, $2, $3)
    FIND_LIB_END($1, $2, $4)
])
dnl> ------------------------------------------------------------------------
dnl> The following macro makes it easier to add includes without adding
dnl> redundant -I specifications (to keep the build line concise).  
dnl> Arguments: 
dnl> 	$1 - the searched directory name
dnl>	$2 - a variable that holds the include specification
dnl>	$3 - a variable that holds all the directories searched so far
dnl
define([ADD_TO_INCS],[
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
dnl> ------------------------------------------------------------------------
dnl> The following macro makes it easier to add libs without adding
dnl> redundant -L specifications (to keep the build line concise).
dnl> Arguments: 
dnl> 	$1 - the searched directory name
dnl>	$2 - the command line option to give to the linker (e.g. -lfoo)
dnl>	$3 - a variable that holds the library specification
dnl>	$4 - a variable that holds all the directories searched so far
dnl
define([ADD_TO_LIBS],[
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
    if test -n "$LIBSW"; then
	$3="$$3 $LIBSW $2"
    else
	$3="$$3 $2"
    fi
    $4="$$4 $1"
])
dnl> ------------------------------------------------------------------------
dnl ### Selecting optional features
dnl AC_ARG_ENABLE(FEATURE, HELP-STRING, ACTION-IF-TRUE [, ACTION-IF-FALSE])
undefine([AC_ARG_ENABLE])
AC_DEFUN(AC_ARG_ENABLE,
[ifelse([$2],,,[AC_DIVERT_PUSH(AC_DIVERSION_NOTICE)dnl
ac_help="$ac_help
[$2]"
AC_DIVERT_POP()])dnl
[#] Check whether --enable-[$1] or --disable-[$1] was given.
enableval="[$enable_]patsubst([$1], -, _)"
if test -n "$enableval"; then
  ifelse([$3], , :, [$3])
ifelse([$4], , , [else
  $4
])dnl
fi
])
dnl
dnl ### Working with optional software
dnl AC_ARG_WITH(PACKAGE, HELP-STRING, ACTION-IF-TRUE [, ACTION-IF-FALSE])
undefine([AC_ARG_WITH])
AC_DEFUN(AC_ARG_WITH,
[ifelse([$2],,,[AC_DIVERT_PUSH(AC_DIVERSION_NOTICE)dnl
ac_help="$ac_help
[$2]"
AC_DIVERT_POP()])dnl
[#] Check whether --with-[$1] or --without-[$1] was given.
withval="[$with_]patsubst([$1], -, _)"
if test -n "$withval"; then
  ifelse([$3], , :, [$3])
ifelse([$4], , , [else
  $4
])dnl
fi
])
dnl> ------------------------------------------------------------------------
dnl> Front-end macros to the above, which when:
dnl>  - a with-<opt> is registered, the corresponding enable-<opt> is
dnl>	registered as bogus
dnl>  - an enable-<opt> is registered, the corresponding with-<opt> is
dnl>	registered as bogus
dnl
AC_DEFUN([MY_ARG_WITH],
[AC_ARG_WITH($@)dnl
AC_ARG_ENABLE([$1],[],[AC_MSG_ERROR([unrecognized variable: enable_$1])])])
dnl
AC_DEFUN([MY_ARG_ENABLE],
[AC_ARG_ENABLE($@)dnl
AC_ARG_WITH([$1],[],[AC_MSG_ERROR([unrecognized variable: with_$1])])])
dnl> ------------------------------------------------------------------------
dnl> Get rid of caching since it doesn't always work.  I.e. changing the
dnl> compiler from the vendor's to gcc can change all sorts of settings,
dnl> but the autoconf logic isn't set up to handle that.  I'll opt for
dnl> stability over speed any day.
dnl
define([AC_CACHE_LOAD],)
define([AC_CACHE_SAVE],)
dnl> ------------------------------------------------------------------------
dnl> Define the package version
AC_DEFUN([PACKAGE_VERSION],
[builtin(include, version.in)
PLPLOT_VERSION=$MAJOR_VERSION.$MINOR_VERSION.$RELEASE_VERSION
AC_SUBST(MAJOR_VERSION)
AC_SUBST(MINOR_VERSION)
AC_SUBST(RELEASE_VERSION)
AC_SUBST(PLPLOT_VERSION)
AC_DEFINE_UNQUOTED(PLPLOT_VERSION, "$PLPLOT_VERSION")])

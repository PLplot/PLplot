divert(-1)dnl
#============================================================================!
# fmacs.m4
# Some utility macros for Fortran m4 hacking.
# Maurice J. LeBrun
# IFS, University of Texas at Austin
# Mar 24, 1994
#
# Some notes:
#
# I always use upper case for macro constants.  Utility macros are
# typically lower case, so you must be careful of namespace collisions
# with these (few) and the builtin m4 macros.  In particular, the m4
# "include" command will take precedence over the fortran one (common
# fortran extension), so its best to use the m4 include instead.  Or, you
# can protect it using quoting characters.
#
# The default m4 quoting characters: `', are changed to {}, since (a)
# these don't conflict with anything in Fortran (unlike the forward
# quote, which is needed for strings), and (b) they are easier to see.
# Some people also like to use [].
#
# The text (comments) in this file are discarded by the divert().
#
# At the end of the file the m4 comments are turned off.
#
# $Id$
# $Log$
# Revision 1.5  2000/12/24 01:44:52  airwin
# At end of file turn off m4 commenting.  Before it was set to * to end of
# line which meant that any fortran line with "*" in it had everything after
# that "*" ignored for m4 processing.  (Not good! for type conversions that
# AWI needs in arithmetic statements.)
#
# Revision 1.4  2000/12/23 23:09:18  airwin
# For fortran conversions using m4 define DBLE to be either dble or real
# depending on whether DOUBLE is set.  Also, for linux attempt to
# define implicit_none as implicit none, but so far this latter
# definition does not work.
#
# Revision 1.3  1994/07/12 19:21:32  mjl
# Fixed type definitions to help generated text stay within 72 columns.
#
# Revision 1.2  1994/03/30  08:28:56  mjl
# Fixed broken macro definition.
#
# Revision 1.1  1994/03/30  07:29:39  mjl
# Moved fortran interface files up one level -- no longer separate single
# and double precision files.  Implemented m4 expansion of REAL into either
# real*4 or real*8 depending on configure settings, for portable builds
# of either the single or double precision library without copying files.
# Files with .fm4 suffix must be run through m4 to become normal fortran
# files.
#
#============================================================================#

changequote({,})

ifdef({INCLUDED_FMACS},,{
    define(INCLUDED_FMACS, 1)

#============================================================================#
# Some new macro primitives.

    define(TRUE, 1)
    define(YES, 1)
    define(ON, 1)

    define(FALSE, 0)
    define(NO, 0)
    define(OFF, 0)

    define(if_true, {ifelse($1,TRUE, {$2},{$3})})
    define(if_false,{ifelse($1,FALSE,{$2},{$3})})

# AND, OR, and CMP macros for logical operations with macros.
# Each expands to only TRUE or FALSE, and accept only two arguments (at 
# present).  Example usage:
#
#	if_true(OR(FOO, BAR), { ... })
#
# will expand the given code if FOO or BAR are defined to TRUE.

    define(AND, {if_true($1, if_true({$2}, TRUE, FALSE), FALSE)})
    define(OR,  {if_true($1, TRUE, if_true({$2}, TRUE, FALSE))})
    define(CMP, {ifelse($1, {$2}, TRUE, FALSE)})

# Null macro (for easily discarding sections of code!)

    define(ignore,)

# undef()	alias for undefine
# ifndef()	if-not-defined -- reverses action of ifdef.
# cdefine()	Conditional define -- only define if not already defined.
# cexpand()	Conditional expand -- expands argument if it is a macro.

    define(undef,  {undefine({$1})})
    define(ifndef, {ifdef({$1},{$3},{$2})})
    define(cdefine,{ifndef({$1},{define({$1},{$2})})})
    define(cexpand,{ifdef({$1},{$1})})

# getstring returns its arg as a string 

    define(getstring,{'{$1}'})

# concat -- concatenates arguments to make a single token

    define(concat, {ifelse($1,,,{$1{}concat(shift($*))})})

# Macro do
# Generates a do-loop like set of source code lines (}unrolls} loop).
# NOTE: you must be very careful about extra spaces when using this macro.
#
# Example code illustrating macro_do:
# Preliminary definitions:
#
#	define(terma,{ a$1 })
#	define(termb,{ b$1 })
#	define(termc,{ c$1$2 })
#
# The code:
#
#	macro_do({MD_I},1,3,
#	{
#	    macro_do({MD_J},1,2,
#	    {
#		termc(MD_I,MD_J) = terma(MD_I)*termb(MD_J);
#	    })
#	})
#
# Output (sans blank lines) :
#
#		 c11  =  a1 * b1 ;
#		 c12  =  a1 * b2 ;
#		 c21  =  a2 * b1 ;
#		 c22  =  a2 * b2 ;
#		 c31  =  a3 * b1 ;
#		 c32  =  a3 * b2 ;
#----------------------------------------------------------------------------#

    define(macro_do,
      {define({$1},$2)$4{}ifelse($1,$3,,{macro_do({$1},eval($2+1),$3,{$4})})})

#----------------------------------------------------------------------------#
# Define some system macros

    define(if_aix,	{ifelse(SYSTEM,AIX, {$1},{$2})})
    define(if_dgux,	{ifelse(SYSTEM,DGUX,{$1},{$2})})
    define(if_sysv,	{ifelse(SYSTEM,SYSV,{$1},{$2})})
    define(if_bsd,	{ifelse(SYSTEM,BSD, {$1},{$2})})
    define(if_unicos,	{ifelse(SYSTEM,UNICOS,{$1},{$2})})
    define(if_sunos,	{ifelse(SYSTEM,SUNOS,{$1},{$2})})
    define(if_ultrix,	{ifelse(SYSTEM,ULTRIX,{$1},{$2})})
    define(if_hpux,	{ifelse(SYSTEM,HPUX,{$1},{$2})})
    define(if_linux,	{ifelse(SYSTEM,Linux,{$1},{$2})})
    define(if_sx,	{ifelse(SYSTEM,SX,{$1},{$2})})

#----------------------------------------------------------------------------#
# Define the implicit_none macro.  
#
# Default
# Implicit character almost as good as implicit none.

    define(implicit_none, {implicit character(a-z)})

# "implicit undefined" is used in many "f77" compatible compilers

    if_sunos({
	define({implicit_none}, {implicit undefined(a-z)})
    })

    if_aix({
	define({implicit_none}, {implicit undefined(a-z)})
    })

# "implicit none" is F90 and Mil Std...

    if_sx({
	define({implicit_none}, {implicit none})
    })

    if_unicos({
	define({implicit_none}, {implicit none})
    })

    if_hpux({
	define({implicit_none}, {implicit none})
    })
    if_linux({
	define({implicit_none}, {implicit none})
    })

#----------------------------------------------------------------------------#
# Define REAL according to the desired floating point precision.  The
# compiler should AT LEAST support automatic promotion of constants via a
# command line flag.  Most can promote variables as well, but this way is
# a bit more portable.

    define(if_dbl,	{ifdef({DOUBLE}, {$1},{$2})})

# There should be a better way..

  if_unicos({
	define(REAL,		{real})
	define(COMPLEX,		{complex})
	define(IMPLICIT_REAL,	{implicit real(a-h,o-z)})
	define(DREAL,		{real})
	define(IMAG,		{aimag {} ifelse({$*},,,{($*)})})
  },{
	define(IMAG,		{imag {} ifelse({$*},,,{($*)})})
    if_dbl({
	define(REAL,		{real*8})
	define(COMPLEX,		{complex*16})
	define(IMPLICIT_REAL,	{implicit real*8(a-h,o-z)})
	define(DREAL,		{real*16})
	define(DBLE,		{dble})
    },{
	define(REAL,		{real*4})
	define(COMPLEX,		{complex*8})
	define(IMPLICIT_REAL,	{implicit real*4(a-h,o-z)})
	define(DREAL,		{real*8})
	define(DBLE,		{real})
    })
  })

#============================================================================#

})
changecom
divert(0)dnl

#!c:sksh

#*************************************************************************
#
# lnsoft - create a link or links
#
# Maurice LeBrun, 2/92.
#
# This function acts like the Unix "ln -s" command.
# No flags are taken at present -- the default action of MakeLink is used.
#
# The HARD option is the default on the CBM version of MakeLink since 
# it doesn't support softlinks in 2.04.  The version of MakeLink by
# Stefan Becker make softlinks by default, and this works well on a
# hard disk partition but not in ram:.
#
#*************************************************************************

if [ $# -lt 2 -o "$1" = '-?' ]
then
   echo 'Usage:' $(basename $0) 'files... target'
   echo '       (links file or files to target)'
   return 1
fi

nloop = $(expr $# - 1)
files = ""
numloop i = 1 $nloop
do
    files = "$files $1"
    shift
done

# Set directory target flag.

target = $1
if [ -d $target ] 
then
    isadir = 1
else
    isadir = 0
fi

if [ $nloop -gt 1 -a ! $isadir = 1 ]
then
    echo 'Target not a directory!'
    return 1
fi

# handle directory name vs device name for target

if [ $isadir -eq 1 ]
then
    ltarget = $(expr length $target)
    if [ $(expr substr $target $ltarget $ltarget) != ':' ]
    then
	target = "$target/"
    fi
fi

# loop over files

for file in $files
do
    if [ -f $file ]
    then
	filename = $(basename $file)
	if [ $isadir -eq 1 ]
	then
	    targetfile = $target$filename
	else
	    targetfile = $target
	fi
	makelink $targetfile $file
    fi
done
return 0

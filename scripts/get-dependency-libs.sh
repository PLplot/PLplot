#!/bin/sh

if test $# != 1 ; then
    echo 2>1 "Usage: $0 path/to/libfoo.la"
    exit 1
fi

result=

for lib in `grep ^dependency_libs= $1 \
               | sed 's/^dependency_libs=//' | sed "s/'//g"` ; do

    case $lib in

        -l* | -L* )
            result="$result $lib"
        ;;

        *.la)
            path=`echo $lib | sed 's:/*lib[^/]*.la::'`
            libname=`echo $lib | sed 's:^.*lib::' | sed 's/\.la//'`
            result="$result -L$path"
            test -d $path/.libs && result="$result -L$path/.libs"
            result="$result -l$libname"
        ;;

    esac

done

echo $result

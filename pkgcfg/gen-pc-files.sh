#!/bin/sh

FILE_INFO="\
    ::: \
    -c++:C++:C++_bindings,_:cxx \
    -f77:F77:Fortran77_bindings,_:f77 \
    -tcl:Tcl/Tk:Tcl/Tk_bindings,_:tcltk"

MASTER=plplot-master-pc

for f in $FILE_INFO ; do
    pcext=`echo $f | cut -d: -f1`
    name=`echo $f | cut -d: -f2`
    desc=`echo $f | cut -d: -f3 | sed "s/_/ /g"`
    lib=`echo $f | cut -d: -f4`
    sed "s:#NAME#:PLplot $name:" < $MASTER \
        | sed "s:#DESC#:Scientific plotting library (${desc}single precision):" \
	| sed "s:#LIB#:plplot$lib:" > plplot$pcext.pc
    sed "s:#NAME#:PLplot $name:" < $MASTER \
        | sed "s:#DESC#:Scientific plotting library (${desc}double precision):" \
	| sed "s:#LIB#:plplot${lib}d:" > plplotd$pcext.pc
done

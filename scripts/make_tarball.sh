#!/bin/sh
# Fix up a clean exported plplot tree in order to generate a release tarball.
# Must be executed from top directory of exported tree, typically a versioned
# directory such as plplot-5.2.0.
cd doc
#IMPORTANT add in documentation from our website.
wget -r -l 1 -L -nH --no-parent --cut-dirs=2 \
http://plplot.sourceforge.net/resources/docbook-manual/
tar zxf plplotdoc-html-0.4.3.tar.gz
#IMPORTANT permissions screwup fixed
cd ..
chmod a+r examples/tcl/stats.log
chmod a-x examples/python/xw??.py
#IMPORTANT prepare tree for configure; make; make install
./bootstrap.sh
#IMPORTANT perl-generate the tcl wrapper files.
cd bindings/tcl 
perl pltclgen
cd ../..

#IMPORTANT swig-generate both the single and double-precision wrappers.
cd bindings/python
swig -python -o plplotcmodule_p_double.c -c++ -DPL_DOUBLE plplotcmodule.i
swig -python -o plplotcmodule_p_single.c -c++ plplotcmodule.i
cd ../..

#IMPORTANT swig-generate the java wrapper.
cd bindings/java
swig -java -package plplot.core -DPL_DOUBLE plplotjavac.i
cd ../..

This directory contains 'TEA-complaint' configuration and makefiles
to compile Plplot as a set of extensions to Tcl.  'TEA' stands for
'Tcl Extension Architecture' and is the modern way to build extensions
for Tcl so that they can be easily recompiled on any Tcl platform.

Note that in the instructions below, the 'autoconf' step is optional, 
since we provide a correctly generated configure file.

You must already have run 'configure' on Plplot, to generate the files
plDevs.h and plConfig.h.  Also plDevs.h should include the tk
driver, and the tkwin driver.

On windows (using cygwin --- see www.scriptics.com for more
instructions), the following sort of thing should suffice:

(in the bash shell):

% autoconf
% ../configure --with-tcl=//d/tcl-source/tcl8.3.1/win/Release
--with-tclinclude=//d/tcl-source/tcl8.3.1/generic
--with-tk=//d/tcl-source/tk8.3.1/generic
--prefix=//d/progra~1/tcl

% make
% make install

'with-tcl' is the location of tclConfig.sh (this requirement may be
lifted in a future version of TEA).
'with-tclinclude' is the location of tcl.h
'with-tk' is the location of tkConfig.sh (this requirement may be
lifted in a future version of TEA).
'prefix' is the install location

You may also wish to add 'exec-prefix' for a platform specific
installation location (otherwise prefix/bin will be used).

----------------------

On Linux (Debian system with a user-defined install region right in
the plplot tree for testing purposes).
autoconf
./configure --with-tcl=/usr/lib \
--with-tclinclude=/usr/include/tcl8.2/generic \
--prefix=../testinstalldir
make
#the following is done as ordinary user since ../testinstalldir is in a normal
#user account directory tree
make install 

Testing
cd to install location
setenv PL_LIBRARY `pwd`/plplot5.1
tclsh
% #this next is required because of non-standard install location
% #use full path name before testinstalldir
% lappend auto_path [pwd]
% package require Pltk
5.1
% cd "/home/software/plplot_cvs/examples/tk"
% source tkdemos.tcl
% 1
% 2
etc....

----------------------

Not tested yet on other unices, but should probably work.

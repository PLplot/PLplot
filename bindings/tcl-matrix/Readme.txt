This directory contains 'TEA-complaint' configuration and makefiles
to compile Plplot as a set of extensions to Tcl.  'TEA' stands for
'Tcl Extension Architecture' and is the modern way to build extensions
for Tcl so that they can be easily recompiled on any Tcl platform.

==========================

Note that this compilation process produces a 'stubified' extension.  
What this means is that other extensions, such as Pltcl, Pltk should
define the macro USE_MATRIX_STUBS, and connect with this code only via
a call like this:

    if (Matrix_InitStubs(interp,"0.1",0) == NULL) {
        return TCL_ERROR;
    }

The advantage of this is no run-time dependencies between Pltcl/tk
libraries and the Matrix shared library.  This, for example, allows us
to recompile, adjust, re-implement, move, rename the Matrix shared
library without even having to recompile Pltcl/tk (let alone make any 
code modifications).

==========================

Instructions:

Note that in the instructions below, the 'autoconf' step is optional, 
since we provide a correctly generated configure file.

On windows (using cygwin --- see www.scriptics.com for more
instructions), the following sort of thing should suffice:

(in the bash shell):

% autoconf
% ./configure --with-tcl=//d/tcl-source/tcl8.3.1/win/Release
--with-tclinclude=//d/tcl-source/tcl8.3.1/generic
--prefix=//d/progra~1/tcl

% make
% make install

'with-tcl' is the location of tclConfig.sh (this requirement may be
lifted in a future version of TEA).
'with-tclinclude' is the location of tcl.h
'prefix' is the install location

You may also wish to add 'exec-prefix' for a platform specific
installation location (otherwise prefix/bin will be used).

----------------------

On Linux (Debian system with a user-defined install region right in
the plplot tree for testing purposes).
autoconf
./configure --with-tcl=/usr/lib --prefix=../testinstalldir
make
#the following is done as ordinary user since ../testinstalldir is in a normal
#user account directory tree
make install 

Testing
tclsh
% #this next is required because of non-standard install location
% #use full path name before testinstalldir
% lappend auto_path "/home/software/plplot_cvs/darley/plplot-matrix/bindings/testinstalldir/lib"
% package require Matrix
0.1
% matrix foo float 3
foo
% foo 0 = 1.55  
% foo 1 = 2.35
% foo 3 = - 4.99
Array index 0 out of bounds: 3; max: 2

% foo 2 = -4.99
% foo *
1.55 2.35 -4.99
etc....

----------------------

Not tested yet on other unices, but should probably work.

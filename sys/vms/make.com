$! make PLPLOT library
$!	This makes plplot_lib:plplot.olb and plplot_lib:plplotfor.obj.
$
$! VAXC can't compile the library.
$! GCC v2.x.x needs /nocase_hack switch to link with DECwindow routines
$ cc := GCC/nocase_hack
$
$! for DEC-C
$! cc := CC/pref=all
$
$ copt := /inc=[-.include]/def=("""getenv=VMSgetenv""")
$
$ x = f$parse("[-.lib]Z.Z;",f$environment("PROCEDURE")) - "Z.Z;"
$ define PLPLOT_LIB 'x'
$ src := [-.src]
$ drv := [-.drivers]
$ fnt := [-.fonts]
$ stbc := [-.src.stubc]
$ stbf := [-.src.stubf]
$
$ set verify
$! VMS specific
$ 'cc' vmsgetenv
$
$! core
$ 'cc''copt' plctrl
$ 'cc''copt' 'src'plfont
$ 'cc''copt' 'src'pdfutils
$ 'cc''copt' 'src'plargs
$ 'cc''copt' 'src'plbox
$ 'cc''copt' 'src'plcont
$ 'cc''copt' 'src'plcvt
$ 'cc''copt' 'src'pldtik
$ 'cc''copt' 'src'plfill
$ 'cc''copt' 'src'plfont
$ 'cc''copt' 'src'plhist
$ 'cc''copt' 'src'plline
$ 'cc''copt' 'src'plmap
$ 'cc''copt' 'src'plot3d
$ 'cc''copt' 'src'plpage
$ 'cc''copt' 'src'plsdef
$ 'cc''copt' 'src'plshade
$ 'cc''copt' 'src'plstream
$ 'cc''copt' 'src'plstring
$ 'cc''copt' 'src'plsym
$ 'cc''copt' 'src'pltick
$ 'cc''copt' 'src'plvpor
$ 'cc''copt' 'src'plwind
$
$! make drivers
$ 'cc''copt' 'src'plcore
$ 'cc''copt' 'drv'dg300
$ 'cc''copt' 'drv'hpgl
$ 'cc''copt' 'drv'ljii
$ 'cc''copt' 'drv'ljiip
$ 'cc''copt' 'drv'null
$ 'cc''copt' 'drv'ps
$ 'cc''copt' 'drv'tek
$ 'cc''copt' 'drv'plbuf
$ 'cc''copt' 'drv'xfig
$ 'cc''copt' 'drv'xwin
$ 'cc''copt' 'drv'plmeta
$
$! fonts
$ 'cc''copt' 'fnt'font01
$ 'cc''copt' 'fnt'font02
$ 'cc''copt' 'fnt'font03
$ 'cc''copt' 'fnt'font04
$ 'cc''copt' 'fnt'font05
$ 'cc''copt' 'fnt'font06
$ 'cc''copt' 'fnt'font07
$ 'cc''copt' 'fnt'font08
$ 'cc''copt' 'fnt'font09
$ 'cc''copt' 'fnt'font10
$ 'cc''copt' 'fnt'font11
$! 'cc''copt' stndfont
$! 'cc''copt' xtndfont
$
$! stubs
$ copt2 := /def=(STUB_LINKAGE=STUB_U)
$ 'cc''copt''copt2' 'stbc'SC3D.C
$ 'cc''copt''copt2' 'stbc'SCcont.c
$ 'cc''copt''copt2' 'stbc'SCstubs.c
$ if f$search("SYS$SYSTEM:*FORTRAN.EXE").eqs."" then goto makelib
$ fortran 'stbf'strutil.f
$ fortran 'stbf'sfstubs.f /obj=plplot_lib:plplotfor.obj
$makelib:
$ library/cre/log plplot_lib:plplot.olb []*.obj
$ set noverify

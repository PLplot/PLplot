$! procedure to make demos
$!
$ cc := gcc/nocase		! for GCC
$! cc := cc			! for DEC-C
$
$ copt := /include=[-.include]
$ ff := fortran
$
$ cex := [-.examples.c]
$ fex := [-.examples.f77]
$ cs = cex + "x*.c"
$ fs = fex + "x*.f"
$
$loop1:
$ src = f$search(cs)
$ if src.eqs."" then goto next
$ src = f$parse(src,,,"NAME")
$ write sys$output "Making ",src,"..."
$ set verify
$ 'cc''copt' 'cex''src'
$ link 'src',PLPLOT_LIB:COPT/opt
$ goto loop1	! 'f$verify(0)'
$
$next:
$ if f$search("SYS$SYSTEM:*FORTRAN.EXE").eqs."" then exit
$
$loop2:
$ src = f$search(fs)
$ if src.eqs."" then  exit
$ src = f$parse(src,,,"NAME")
$ write sys$output "Making ",src,"..."
$ set verify
$ 'ff' 'fex''src'.f
$ link 'src',PLPLOT_LIB:FOPT/opt
$ goto loop2	! 'f$verify(0)'

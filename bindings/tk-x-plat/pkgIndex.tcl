
if {[info exists tcl_platform(debug)]} {
    package ifneeded Plplotter 5.1 \
      [list load [file join $dir plplot51d[info sharedlibextension]] Plplotter]
} else {
    package ifneeded Plplotter 5.1 \
      [list load [file join $dir plplot51[info sharedlibextension]] Plplotter]
}

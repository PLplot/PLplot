
if {$tcl_platform(platform) == "unix"} {
    if {[info exists tcl_platform(debug)]} {
	set file [file join $dir libplplotd[info sharedlibextension].5.1.0]
    } else {
	set file [file join $dir libplplot[info sharedlibextension].5.1.0]
    }
} else {
    if {[info exists tcl_platform(debug)]} {
	set file [file join $dir plplot510d[info sharedlibextension]]
    } else {
	set file [file join $dir plplot510[info sharedlibextension]]
    }
}

# This little helper is needed to deal seamlessly with the
# possibility that either or both $dir and $file may contain
# spaces, or characters like []{}
proc loadPlplot {dir file} {
    global pllibrary
    set pllibrary $dir
    load $file Plplotter
    rename loadPlplot {}
}

package ifneeded Plplotter 5.1.0 [list loadPlplot $dir $file]
unset file

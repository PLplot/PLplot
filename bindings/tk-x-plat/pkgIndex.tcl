
if {[info exists tcl_platform(debug)]} {
    set file [file join $dir plplot51d[info sharedlibextension]]
} else {
    set file [file join $dir plplot51[info sharedlibextension]]
}

package ifneeded Plplotter 5.1 "
    set pllibrary $dir
    load $file Plplotter
"
unset file

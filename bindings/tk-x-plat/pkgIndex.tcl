
if {$tcl_platform(platform) == "unix"} {
    set file_d [file join $dir drivers tkwind_drv[info sharedlibextension]]
    set file_s [file join $dir drivers tkwin_drv[info sharedlibextension]]
# give preference to double precision driver, if it exists.
    if [file exists $file_d] {
	set file $file_d
    } else {
	set file $file_s
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
# put core tcl scripts in path
    lappend auto_path $dir/tcl
}

package ifneeded Plplotter 5.1.0 [list loadPlplot $dir $file]
unset file

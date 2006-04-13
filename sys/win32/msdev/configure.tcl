# configure.tcl --
#     Simple configuration script for the Windows version of
#     PLplot. It recognises the following options:
#     --with-double           Use double precision reals (default)
#     --without-double        Use single precision reals instead
#     --with-single           Use single precision reals instead
#     --prefix=directory      Directory to install PLplot in
#     --installdir=directory  Directory to install PLplot in
#
# TODO:
#     string tolower
#
#     (more to come)
#
#     The script can be run via Tcl/Tk (see the README file
#     for details).
#
#     The textual user-interface allows you to set the
#     various options.
#
#     The chosen options are stored in a file "config.res"
#     for future use (if this file exists, it is read at
#     start-up).
#

# mainMenu --
#    Present the main menu
#
# Arguments:
#    None
# Result:
#    None
# Side effects:
#    New menus may be shown, option values set
#
proc mainMenu {} {
    global stdin
    global options

    while {1} {
        puts "
PLplot - main configuration menu
--------------------------------

Choices:

1. Precision for floating-point:   $options(float)
2. Installation directory:         $options(install)
3. Device drivers:
      $options(drivers)
4. Antialiasing:                   $options(antialiasing)
5. Interpolation libraries:
      $options(interpolation)

H. Explanations

E. Exit and configure
Q. Exit, do not configure

Your choice:"


        set choice [$stdin gets]
        switch -- $choice {
        1 - 2 - 3 - 4 - 5 {
            showMenu$choice
        }
        h - H {
            showHelp main
        }
        e - E {
            runConfiguration
            break
        }
        q - Q {
            puts "Choices not saved; no re-configuration"
            break
        }
        default {
            puts "Unknown choice ..."
        }
        }
    }
    exit
}

# showMenu1 --
#    Present the menu to select the precision
#
# Arguments:
#    None
# Result:
#    None
# Side effects:
#    Option float set
#
proc showMenu1 {} {
    global stdin
    global options

    while {1} {
        puts "
PLplot - floating-point precision
---------------------------------

Choices:

Currently: PLplot will use *** $options(float) *** precision

1. Use single precision

2. Use double precision

   Note:
   This choice is important for interacting with
   programs that do not explicitly use the PLFLT type
   and for programming languages other than C or C++,
   as they can not be told what the PLFLT type
   actually is.

R. Return

Your choice:"

        set choice [$stdin gets]
        switch -- $choice {
        1 {
            set options(float) "single"
        }
        2 {
            set options(float) "double"
        }
        r - R {
            break
        }
        default {
            puts "Unknown choice ..."
        }
        }
    }
}

# showMenu2 --
#    Present the menu to select the installation directory
#
# Arguments:
#    None
# Result:
#    None
# Side effects:
#    Option install set
#
proc showMenu2 {} {
    global stdin
    global options

    while {1} {
        puts "
PLplot - installation directory
-------------------------------

Choices:

Currently: PLplot will use *** $options(install) ***
           to install the libraries

1. Set new directory

   Note:
   This choice is important for creating your own
   software using PLplot and for finding all the
   files that you need to distrbute.

   Please note that the files will be stored in
   subdirectories (include, lib, ...) of the
   directory you give.

R. Return

Your choice:"

        set choice [$stdin gets]
        switch -- $choice {
        1 {
            puts "New directory: "
            set options(install) [$stdin gets]
        }
        r - R {
            break
        }
        default {
            puts "Unknown choice ..."
        }
        }
    }
}

# showMenu3 --
#    Present the menu to select the device drivers
#
# Arguments:
#    None
# Result:
#    None
# Side effects:
#    Option install set
#
proc showMenu3 {} {
    global stdin
    global options

    while {1} {
        puts "
PLplot - device drivers
-----------------------

Choices:

Currently: PLplot will use the following device drivers:
           *** $options(drivers) ***

1. Use a minimum set of drivers (win32 and PostScript only)

2. Use all standard drivers

3. Add the following drivers: JPEG and PNG

   Note:
   The additional drivers require the GD library to be
   present on the system.

R. Return

Your choice:"

        set choice [$stdin gets]
        switch -- $choice {
        1 {
            set options(drivers) [join $options(minimum_drivers) ", "]
        }
        2 {
            set options(drivers) [join $options(standard_drivers) ", "]
        }
        3 {
            puts "Not implemented yet!"
        }
        r - R {
            break
        }
        default {
            puts "Unknown choice ..."
        }
        }
    }
}

# showMenu4 --
#    Present the menu to select the antialiasing options
#
# Arguments:
#    None
# Result:
#    None
# Side effects:
#    Options related to antialiasing set
#
proc showMenu4 {} {
    global stdin
    global options
    puts $options

    while {1} {
        puts "
PLplot - antialiasing capabilities
----------------------------------

Choices:

NOT IMPLEMENTED YET

R. Return

Your choice:"

        set choice [$stdin gets]
        switch -- $choice {
        r - R {
            break
        }
        default {
            puts "Unknown choice ..."
        }
        }
    }
}

# showMenu5 --
#    Present the menu to select the interpolation libraries
#
# Arguments:
#    None
# Result:
#    None
# Side effects:
#    Options related to interpolation set
#
proc showMenu5 {} {
    global stdin
    global options
    puts $options

    while {1} {
        puts "
PLplot - interpolation libraries
--------------------------------

Choices:

Currently: PLplot will use the following libraries:
           *** $options(interpolation) ***

1. Use the CSA library (cubic splines):     $options(usecsa)

2. Use the NN library (natural neighbours): $options(usenn)

   Note:
   The nearest neighbours (NN) library requires that you
   have the Qhull library too.

3. Location of Qhull library: $options(libqhull)

R. Return

Your choice:"

        set choice [$stdin gets]
        switch -- $choice {
        1 {
            yesNo usecsa
            set options(interpolation) [composeChoices csa nn]
        }
        2 {
            yesNo usenn
            set options(interpolation) [composeChoices csa nn]
        }
        3 {
            findLibrary qhull qhull.lib qhull.h
        }
        r - R {
            break
        }
        default {
            puts "Unknown choice ..."
        }
        }
    }
}

# showHelp --
#    Present some information
#
# Arguments:
#    name         Name of the chapter
# Result:
#    None
# Side effects:
#    None
#
proc showHelp {name} {
    global stdin

    while {1} {
        puts "
PLplot - information on the configuration options
-------------------------------------------------

Compile options:
PLplot has a single compile option, the precision
of floating-point numbers. This can be used to
comply better to the default of your programs.

External libraries:
PLplot can be configured to use various external
libraries to enhance its capabilities. As these
are maintained by others than the PLplot team,
you will need to get them yourself.

More information in the README and INSTALL files

Press R to return ..."

        set choice [$stdin gets]
        switch -- $choice {
        r - R {
            break
        }
        default {
            puts "Unknown choice ..."
        }
        }
    }
}

# yesNo --
#    Toggle a choice
#
# Arguments:
#    name       Name of the option to toggle
# Result:
#    None
# Side effects:
#    Option toggled
#
proc yesNo {name} {
    global options

    if { [string equal $options($name) "No"] } {
        set options($name) "Yes"
    } else {
        set options($name) "No"
    }
}

# findLibrary --
#    Find a particular library (and its include files)
#
# Arguments:
#    keyword    Keyword to store the information in
#    libname    Name of the library to find
#    incname    Name of the include file to find
# Result:
#    None
# Side effects:
#    Option indicated by keyword set
#
proc findLibrary {keyword libname incname} {
    global stdin
    global options

    set found 0

    foreach dir [concat [split [env LIB]] $options(lib$keyword)] {
        if { [file exists "$dir/$libname"] } {
            set options(lib$keyword) $dir
            set found 1
        }
    }
    foreach dir [concat [split [env LIB]] $options(inc$keyword)] {
        if { [file exists "$dir/$incname"] } {
            set options(inc$keyword) $dir
            incr found
        }
    }
    if { $found != 3 } {
        puts "File $libname or $incname not found!"
        puts "Please enter the parent directory where they reside: "
        set dir [$stdin gets]
        set options(lib$keyword) "$dir\\lib"
        set options(inc$keyword) "$dir\\include"
    }
}

# composeChoices --
#    Compose a list of choices (based on yes/no values)
#
# Arguments:
#    args       List of names to examine (use$name and $name)
# Result:
#    "List" of chosen options or "None"
# Side effects:
#    None
#
proc composeChoices {args} {
    global options

    set result {}
    foreach name $args {
        if { [info exists options($name)] &&
             [info exists options(use$name)] } {
            #
            # Proceed - otherwise a programming error
            #
            #if { [string tolower $options(use$name)] == "yes" } {}
            if { [string equal $options(use$name) "Yes" } {
                lappend result $options($name)
            }
        } else {
            puts "PROGRAMMING ERROR: options($name) or options(use$name) \
does not exist!"
            exit
        }
    }
    if { [string equal $result ""] } {
        set result "None"
    }
    return [join $result ", "]
}


# saveConfiguration --
#    Save the configuration data
#
# Arguments:
#    None
# Result:
#    None
# Side effects:
#    Options saved to file for future reference
#
proc saveConfiguration {} {
    global savefile
    global options

    set outfile [aio.open $savefile w]

   #puts $outfile "array set options [list [array get options]]"
    foreach {key value} $options {
        $outfile puts [list set options($key) $options($key)]
    }
    $outfile close
}

# runConfiguration --
#    Save the configuration data
#
# Arguments:
#    None
# Result:
#    None
# Side effects:
#    Adjust the various files according to the options
#
proc runConfiguration {} {

    #
    # Save the options
    #
    saveConfiguration

    #
    # Adjust the makefile for installing the files
    #
    adjustFile "makefile.in" "makefile"
    adjustFile "plplib/plplib.mak.in" "plplib/makefile"

    #
    # Adjust the Fortran examples
    #
    puts "Fortran examples not treated yet ..."
#   set orgdir [pwd]
#   cd ../../../examples/f77
#
#    foreach f [glob *.fm4] {
#        adjustFile $f "[file root $f].f"
#    }
#   cd $orgdir

    #
    # Put up a message ...
    #
    puts "
Configuration is complete.
Run the makefiles to create the actual
libraries and examples"

    exit
}

# adjustFile --
#    Adjust the contents of the given file and copy it to a new file
#
# Arguments:
#    srcfile         Name of the original file
#    dstfile         Name of the new file
# Result:
#    None
# Side effects:
#    New file written with the new contents
#
proc adjustFile {srcfile dstfile} {
    global options

    #
    # Construct the set of old/new strings
    #
    set replaces {}

    #
    # Single-precision?
    #
    if { $options(float) eq "single" } {
        lappend replaces "~define_plflt~" "#undef PL_DOUBLE"
        lappend replaces "real*8"         "real*4"
    } else {
        lappend replaces ~define_plflt~ "#define PL_DOUBLE"
        lappend replaces "real*4"         "real*8"
    }

    #
    # Installation directory and other stuff
    #
    lappend replaces "~installdir~" "$options(install)"
    lappend replaces "~version~"    "\"$options(version)\""

    set device_drivers {}
    foreach driver $options(drivers) {
        set driver [string map {, ""} $driver]
        append device_drivers \
            "[string toupper $driver] = PLD_$driver\n"
    }
    lappend replaces "~devices~" $device_drivers

    #
    # Additional source files
    #
    set selected_objs ""
    if { [string tolower $options(usecsa)] eq "yes" } {
        append selected_objs "\t\"\$(INTDIR)\\csa.c\" \\\n"
    }
    if { [string tolower $options(usenn)] eq "yes" } {
        append selected_objs "\t\"\$(INTDIR)\\nn.c\" \\\n"
    }
    lappend replaces "~selected_objs~" $selected_objs

    #
    # Read the file, replace the strings and write the new one
    #
    set infile  [aio.open $srcfile r]
    set outfile [aio.open $dstfile w]

    set contents  [$infile read]
    $outfile puts [string map $replaces $contents]

    $infile  close
    $outfile close
}

# handleCmdOptions --
#    Interpret the command-line options - if any
#
# Arguments:
#    arglist         List of command-line options
# Result:
#    None
# Side effects:
#    Set the values in the options array
#
proc handleCmdOptions {arglist} {
    global options

    foreach a $arglist {
        switch -glob -- $a {
            "--with-double"    { set options(float) "double" }
            "--without-double" { set options(float) "single" }
            "--with-single"    { set options(float) "single" }
            "--prefix=*"       -
            "--installdir=*"   {
               set dirname [lindex [split $a "="] 1]
               set options(install) $dirname
            }

            default {
                # Nothing further at the moment
            }
        }
    }
}

# main --
#     Initialise all options and get the GUI started
#
#
global savefile
global options
set savefile         "config.res"
set options(version) "5.6.0"
set options(float)   "double"
set options(install) "c:\\plplot"
set options(drivers) "?"
set options(antialiasing)  "No"
set options(interpolation) "None"
set options(csa)           "Cubic splines"
set options(usenn)         "No"
set options(usecsa)        "No"
set options(nn)            "Natural neighbours"
set options(usenn)         "No"
set options(libqhull)      "?"
set options(incqhull)      "?"
set options(minimum_drivers)  {win3 ps}
set options(standard_drivers) {win3 lj_hpgl null plmeta ps xfig}
set options(drivers)       [join $options(standard_drivers) ", "]

load jim-aio
global stdin
set stdin [aio.open standard input]

# file --
#    Compatibility command for jim
#
proc file {cmd args} {
    switch -- $cmd {
    exists {
        if { [catch {
                 set infile [aio.open [lindex $args 0]]
              } msg] } {
            return 0
        } else {
            $infile close
            return 1
        }
    }
    }
}


if { [file exists $savefile] } {
    source $savefile
}
handleCmdOptions $argv

mainMenu

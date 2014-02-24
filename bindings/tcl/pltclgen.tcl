#!/usr/local/bin/tclsh
# Arjen Markus
# - translation of the original Perl script
#
# This script is used to automatically generate most of the functions needed
# to implement the PLplot Tcl API.  It reads a file which specifies the
# PLplot API command arguments and return value (basically like a C
# prototype, but easier to parse) and generates a Tcl command procedure to
# call that function.
#
# Currently it can support arguments of type PLINT, PLFLT, char *, PLINT&
# and PLFLT& where the last two are used for the 'g' series functions which
# provide data to the caller.
#
# What is not supported at this time, but needs to be, is support for (those
# few) PLplot API functions with non-void return types, and (the many)
# PLplot API functions which accept arrays (PLFLT *, etc).  The latter can
# in many cases be correctly treated as 1-d Tcl Matricies.  The code for
# using these should be sufficiently perfunctory to be amenable to an
# approach like that used here.  Automatic support for the 2-d API is
# probably unrealistic.
###############################################################################

# Process a function "prototype".  Suck up the args, then perform the
# needed substitutions to the Tcl command procedure template.
# Generate the three outputs needed for use in tclAPI.c:  the C
# function prototype, the CmdInfo structure initializer, and the
# actual function definition.

proc process_pltclcmd {cmd rtype} {
    global verbose
    global cmdfile
    global GENHEAD
    global GENSTRUCT
    global GENFILE
    global SPECFILE

    if { $::verbose } {
       puts "\nautogenerating Tcl command proc for $rtype $cmd ()"
    }

    puts $GENHEAD "static int ${cmd}Cmd( ClientData, Tcl_Interp *, int, const char **);"
    puts $GENSTRUCT "    {\"$cmd\",          ${cmd}Cmd},"

    set args    ""
    set nargs   0
    set ndefs   0
    set refargs 0
    while { [gets $SPECFILE line] >= 0 } {

        if { $line == "" } {
            break
        }

        if { [regexp {^(\w+)\s+(.*)$} $line ==> vname vtype] > 0 } {
            set defval ""
            if { $verbose } {
               puts "vname=$vname vtype=$vtype"
            }
            if { [regexp {(.*)\s+Def:\s+(.*)} $vtype ==> vtype defval] } {
                if { $verbose } {
                    puts "default arg: vtype=$vtype defval=$defval"
                }
            }
            set argname($nargs) $vname
            set argtype($nargs) $vtype
            set argdef($nargs)  $defval
            set argref($nargs)  0 ;# default.

            # Check to see if this arg is for fetching something from PLplot.

           if { [string first & $vtype] >= 0 || $vtype == "char *" } {
               set refargs 1
               set argref($nargs) 1
           }

           if { $nargs == 0 } {
               set args "${args}$vname"
           } else {
               set args "$args $vname"
           }
           if { $defval != "" } {
               incr ndefs
           }
           incr nargs
           continue
       }

       # Unrecognized output.

       puts "bogus: $line"
    }

    if { $verbose } {
        puts "There are $nargs arguments, $ndefs are defaultable.";
        for { set i 0 } { $i < $nargs } { incr i } {
            puts "$argtype($i) $argname($i)";
        }
        if { $refargs > 0 } {
           puts "return string required."
        }
    }

    set TEMPLATE [open "$cmdfile"]

    while { [gets $TEMPLATE tmpline] >= 0 } {

        # Emit the argument declarations.  Reference args require special
        # handling, the others should be perfunctory.

        switch -- $tmpline {
            "<argdecls>" {
                for { set i 0 } { $i < $nargs } { incr i } {
                    switch -- $argtype($i) {
                        "PLINT&" {
                            puts $GENFILE "    PLINT $argname($i);"
                        }
                        "PLUNICODE&" {
                            puts $GENFILE "    PLUNICODE $argname($i);"
                        }
                        "PLFLT&" {
                            puts $GENFILE "    PLFLT $argname($i);"
                        }
                        "char&" {
                            puts $GENFILE "    char $argname($i);"
                        }
                        "PLINT *" {
                            puts $GENFILE "    PLINT *$argname($i);"
                            puts $GENFILE "    tclMatrix *mat$argname($i);"
                        }
                        "PLUNICODE *" {
                            puts $GENFILE "    PLUNICODE *$argname($i);"
                            puts $GENFILE "    tclMatrix *mat$argname($i);"
                        }
                        "PLFLT *" {
                            puts $GENFILE "    PLFLT *$argname($i);"
                            puts $GENFILE "    tclMatrix *mat$argname($i);"
                        }
                        "const char *" {
                            puts $GENFILE "    const char *$argname($i);"
                        }
                        "char *" {
                            puts $GENFILE "    char $argname($i)\[200\];"
                        }
                        default {
                            if { $argdef($i) != "" } {
                                puts $GENFILE "    $argtype($i) $argname($i) = $argdef($i);"
                            } else {
                                puts $GENFILE "    $argtype($i) $argname($i);"
                            }
                        }
                    }
                }
            }
            "<getargs>" {
                # Obtain the arguments which we need to pass to the PLplot API call,
                # from the argc/argv list passed to the Tcl command proc.  Each
                # supported argument type will need special handling here.

                for { set i 0 } { $i < $nargs } { incr i } {
                    if { $ndefs > 0 } {
                        puts $GENFILE "    if (argc > $i+1) \{    "
                    }
                    if { $argref($i) } {
                        puts $GENFILE "/* $argname($i) is for output. */"
                        continue
                    }
                    switch -- $argtype($i) {
                        "PLINT *" {
                            puts $GENFILE "    mat$argname($i) = Tcl_GetMatrixPtr( interp, argv\[1+$i\] );"
                            puts $GENFILE "    if (mat$argname($i) == NULL) return TCL_ERROR;"
                            puts $GENFILE "    $argname($i) = mat$argname($i)-\>idata;"
                        }
                        "PLUNICODE *" {
                            puts $GENFILE "    mat$argname($i) = Tcl_GetMatrixPtr( interp, argv\[1+$i\] );"
                            puts $GENFILE "    if (mat$argname($i) == NULL) return TCL_ERROR;"
                            puts $GENFILE "    $argname($i) = mat$argname($i)-\>idata;"
                        }
                        "PLFLT \*" {
                            puts $GENFILE "    mat$argname($i) = Tcl_GetMatrixPtr( interp, argv\[1+$i\] );"
                            puts $GENFILE "    if (mat$argname($i) == NULL) return TCL_ERROR;"
                            puts $GENFILE "    $argname($i) = mat$argname($i)-\>fdata;"
                        }
                        "PLINT" {
                            puts $GENFILE "    $argname($i) = atoi(argv\[1+$i\]);"
                        }
                        "PLUNICODE" {
                            puts $GENFILE "    $argname($i) = (PLUNICODE) strtoul(argv\[1+$i\],NULL,10);"
                        }
                        "unsigned int" {
                            puts $GENFILE "    $argname($i) = (unsigned int) strtoul(argv\[1+$i\],NULL,10);"
                        }
                        "PLFLT" {
                            puts $GENFILE "    $argname($i) = atof(argv\[1+$i\]);"
                        }
                        "const char *" {
                            puts $GENFILE "    $argname($i) = argv\[1+$i\];"
                        }
                        "char" {
                            puts $GENFILE "    $argname($i) = argv\[1+$i\]\[0\];"
                        }
                        default {
                            puts "Unrecognized argtype : $argtype($i)"
                        }
                    }
                    if { $ndefs > 0 } {
                        puts $GENFILE "    \}"
                    }
                }
            }

            # Call the PLplot API function.

            "<plcmd>" {
                puts -nonewline $GENFILE "    $cmd ( "
                for { set i 0 } { $i < $nargs } { incr i } {
                    if { [string match "*&" $argtype($i)] } {
                           puts -nonewline $GENFILE "&$argname($i)"
                    } else {
                        puts -nonewline $GENFILE "$argname($i)"
                    }

                    if { $i < $nargs-1 } {
                       puts -nonewline $GENFILE ", "
                    }
                 }
                 puts $GENFILE " );"
             }

             # If there were reference arguments, fetch their contents and stuff them
             # into their corresponding Tcl variables.

             # NOTE: This should be improved so take into account the current value
             # of tcl_precision.

             "<fetch_result>" {
                 if { $refargs > 0 } {
                     for { set i 0 } { $i < $nargs } { incr i } {
                         if { ! $argref($i) } {
                             continue
                         }
                         if { $i > 0 } {
                             puts $GENFILE "    if (argc == 1)"
                             puts $GENFILE "        Tcl_AppendResult( interp, \" \", (char *) NULL );"
                         }
                         switch -- $argtype($i) {
                             "PLINT&" {
                                 puts $GENFILE "    sprintf( buf, \"%d\", $argname($i) );"
                                 puts $GENFILE "    if (argc > 1)"
                                 puts $GENFILE "        Tcl_SetVar( interp, argv\[1+$i\], buf, 0 );"
                                 puts $GENFILE "    else"
                                 puts $GENFILE "        Tcl_AppendResult( interp, buf, (char *) NULL );"
                             }

                             "PLUNICODE&" {
                                 puts $GENFILE "    sprintf( buf, \"%u\", $argname($i) );"
                                 puts $GENFILE "    if (argc > 1)"
                                 puts $GENFILE "        Tcl_SetVar( interp, argv\[1+$i\], buf, 0 );"
                                 puts $GENFILE "    else"
                                 puts $GENFILE "        Tcl_AppendResult( interp, buf, (char *) NULL );"
                             }

                             "char *" {
                                 puts $GENFILE "    if (argc > 1)"
                                 puts $GENFILE "       Tcl_SetVar( interp, argv\[1+$i\], $argname($i), 0 );"
                                 puts $GENFILE "    else"
                                 puts $GENFILE "        Tcl_AppendResult( interp, $argname($i), (char *) NULL );"
                             }

                             "char&" {
                                 puts $GENFILE "    sprintf( buf, \"%c\", $argname($i) );"
                                 puts $GENFILE "    if (argc > 1)"
                                 puts $GENFILE "        Tcl_SetVar( interp, argv\[1+$i\], buf, 0 );"
                                 puts $GENFILE "    else"
                                 puts $GENFILE "        Tcl_AppendResult( interp, buf, (char *) NULL );"
                             }

                             # The following needs to be corrected to work with the Tcl
                             # precision standard (global var tcl_precision).

                             "PLFLT&" {
                                 puts $GENFILE "    Tcl_PrintDouble( interp, $argname($i), buf );"
                                 puts $GENFILE "    if (argc > 1)"
                                 puts $GENFILE "        Tcl_SetVar( interp, argv\[1+$i\], buf, 0 );"
                                 puts $GENFILE "    else";
                                 puts $GENFILE "        Tcl_AppendResult( interp, buf, (char *) NULL );"
                             }

                             default {
                                 puts $GENFILE "Unsupported arg type."
                             }
                         }
                     }
                 }
             }
             default {

                 # substitutions here...

                 set line [string map [list %cmd% $cmd %nargs% $nargs] $tmpline]
                 if { $refargs } {
                    set line [string map [list %args% \?$args\?] $line]
                 } else {
                    set line [string map [list %args% $args] $line]
                 }
                 set line [string map [list %ndefs% $ndefs %isref% $refargs] $line]

                 puts $GENFILE $line
             }
        }
    }

    close $TEMPLATE
}

# main code --
#
set verbose [expr {[lsearch $argv "-v"] >= 0}]

# Find the source tree directory that must be specified on the command line.
set sourcedir [lindex $argv 0]                    ;# Get the current source directory - for "out of source tree builds"
set specfile  [file join $sourcedir "plapi.tpl"]  ;# PLplot API template specification file.
set genfile   "tclgen.c"                          ;# Generated functions go here.
set genhead   "tclgen.h"                          ;# Prototypes for generated functions.
set genstruct "tclgen_s.h"                        ;# Initializers for CmdInfo struct.
set cmdfile   [file join $sourcedir "tclcmd.tpl"] ;# Template file for generated functions.

set SPECFILE  [open $specfile]
set GENFILE   [open $genfile "w"]
set GENHEAD   [open $genhead "w"]
set GENSTRUCT [open $genstruct "w"]

# Scan the PLplot API template specification file looking for function
# "prototypes".  These are introduced with the token "pltclcmd".  When
# we find one, go process it.  Anything other than a comment or a
# valid function "prototype" is considered an error, and is printed to
# stdout.

while { [gets $SPECFILE line] >= 0 } {
    regsub {#.*$} $line {} line
    if { $line == "" } continue

    if { [regexp {^pltclcmd (\w+) (.*)} $line ==> cmd rtype] } {
        process_pltclcmd $cmd $rtype
        continue
    }

# Just print the unrecognized output to stdout.

    puts "? $line"
}

# This file is sourced by the Plplot-Tcl Init function
# It should be used for any general purpose initialization
# There is no need to source lots of other files, since
# auto-loading should be used for that purpose

# Turn this on if you can't figure out if this is being sourced.
#puts "In plplot.tcl"

# I see no reason for plframe to depend on Itcl/Itk.
#package require Itk 2.1
package require Tk

# Can't do it this way, b/c no way to build pkgIndex.tcl, so instead
# we have Pltk_Init just call Matrix_Init directly.
#package require Matrix

# Don't see how this will get executed unless the directory containing
# this path is already on the auto_path, in which case these lines are
# superfluous. 
#lappend auto_path $pllibrary
# searched for fonts by plplot
#set env(PL_LIB) $pllibrary

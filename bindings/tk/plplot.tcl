# This file is sourced by the Plplot-Tcl Init function
# It should be used for any general purpose initialization
# There is no need to source lots of other files, since
# auto-loading should be used for that purpose

# initialisation

lappend auto_path $pllibrary
# searched for fonts by plplot
set env(PL_LIB) $pllibrary
# Set up defaults (auto-loaded from the auto_path)
pldefaults

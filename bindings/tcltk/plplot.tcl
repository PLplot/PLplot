# Initialisation.  This file is automatically found and sourced by 
# either Pltcl_Init or Pltk_Init.  If it cannot be found or sourced, those
# procedures will throw an error.

lappend auto_path $pllibrary
# searched for fonts by plplot
set env(PL_LIB) $pllibrary
# Set up defaults (auto-loaded from the auto_path)
if {![catch {package present Tk}]} {
    pldefaults
}


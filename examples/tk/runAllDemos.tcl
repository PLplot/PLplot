#----------------------------------------------------------------------------
# Source this file into a working Tk interpreter to run all the Tcl demos
# in a nice window with buttons for each demo you'd like to run.
# 
# Vince Darley
# vince@santafe.edu
# 
#----------------------------------------------------------------------------

cd [file join [file dirname [info script]] .. tcl]
lappend auto_path [pwd]
catch {package require Plplotter}
plframe .p
grid .p -columnspan 5 -sticky news
grid rowconfigure . 0 -weight 1
for {set i 0} {$i < 5} {incr i} {
    grid columnconfigure . $i -weight 1
}

# turn on pauses
.p cmd plspause 1

button .cexit -text "Quit" -command exit
button .cshell -text "Shell" -command "console show"
button .creload -text "Reload" -command reload
button .bnextpage -text "Page" -command ".p nextpage"

set buttons [list .cexit .cshell .creload .bnextpage]

proc reload {} {
    global demos
    foreach demo $demos {
	catch {rename $demo {}}
    }
    auto_reset
}

proc run {demo} {
    .p configure -eopcmd [list .bnextpage configure -state normal]
    .l configure -text "Starting $demo"
    setButtonState disabled
    update idletasks
    .p cmd plbop
    if {[catch {$demo .p} err]} {
	puts stderr $err
    }
    .p cmd pleop
    focus .p
    .l configure -text "$demo complete"
    setButtonState normal
    .bnextpage configure -state disabled
}

proc setButtonState {state} {
    foreach b [info commands .b*] {
	$b configure -state $state
    }
}

for {set i 1} {$i <= [llength [glob x*.tcl]]} {incr i} {
    set demo x[format "%02d" $i]
    button .b$i -text "Demo $i" -command [list run $demo]
    lappend demos $demo
    lappend buttons .b$i
    if {[llength $buttons] == 5} {
	eval grid $buttons -sticky ew
	set buttons {}
    }
}

if {[llength $buttons]} {
    eval grid $buttons -sticky ew
}

label .l
grid .l -sticky ew -columnspan 5

plxframe .plw
pack append . .plw {left expand fill}

for {set i 1} {$i <= 18} {incr i} {
    set demo x[format "%02d" $i]
    puts $demo.tcl
    source ../tcl/$demo.tcl
    proc $i {} "$demo .plw.plwin"
}

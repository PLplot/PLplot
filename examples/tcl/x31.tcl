#----------------------------------------------------------------------------
# $Id$
#----------------------------------------------------------------------------

proc x31 {{w loopback}} {

    # this constant should be defined centrally.
    set PL_NOTSET -42

    set compression1 95
    $w cmd plscompression $compression1
    # Test setting / getting familying parameters across plinit.
    set fam1 0
    set num1 10
    set bmax1 1000
    $w cmd plsfam $fam1 $num1 $bmax1
    
    # Test setting / getting page parameters across plinit.
    set xp1 200.
    set yp1 200.
    set xleng1 400
    set yleng1 200
    set xoff1 10
    set yoff1 20
    $w cmd plspage $xp1 $yp1 $xleng1 $yleng1 $xoff1 $yoff1

    $w cmd plinit

    set status 0

    # Test if device initialization screwed around with the preset
    # compression parameter.
    $w cmd plgcompression compression2 
    puts [ format "Output various PLplot parameters" ]
    puts [ format "compression parameter = %d" $compression2 ]
    if {$compression2 != $compression1} {
        puts stderr "plgcompression test failed"
        set status 1
    }

    # Test if device initialization screwed around with any of the
    # preset familying values.
    $w cmd plgfam fam2 num2 bmax2
    puts [ format "family parameters: fam, num, bmax = %d %d %d" $fam2 $num2 $bmax2 ]
    if {$fam2 != $fam1 || $num2 != $num1 || $bmax2 != $bmax1} {
        puts stderr "plgfam test failed"
        set status 1
    }

    # Test if device initialization screwed around with any of the
    # preset page values.
    $w cmd plgpage xp2 yp2 xleng2 yleng2 xoff2 yoff2
    puts [ format "page parameters: xp, yp, xleng, yleng, xoff, yoff = %f %f %d %d %d %d" $xp2 $yp2 $xleng2 $yleng2 $xoff2 $yoff2 ]
    if {$xp2 != $xp1 || $yp2 != $yp1 || $xleng2 != $xleng1 || $yleng2 != $yleng1 || $xoff2 != $xoff1 || $yoff2 != $yoff1} {
        puts stderr "plgpage test failed"
        set status 1
    }

    # Exercise plscolor, plscol0, plscmap1, and plscmap1a to make sure
    # they work without any obvious error messages.
    $w cmd plscolor 1
    matrix r1 i 2 = {0, 255}
    matrix g1 i 2 = {255, 0}
    matrix b1 i 2 = {0, 0}
    matrix a1 f 2 = {1.0, 1.0}
    $w cmd plscmap1 r1 g1 b1 2
    $w cmd plscmap1a r1 g1 b1 a1 2

    $w cmd plglevel level2 
    puts [ format "level parameter = %d" $level2 ]
    if {$level2 != 1} {
        puts stderr "plglevel test failed."
        set status 1
    }

    $w cmd pladv 0
    $w cmd plvpor 0.01 0.99 0.02 0.49
    $w cmd plgvpd xmin xmax ymin ymax
    puts [ format "plvpor: xmin, xmax, ymin, ymax = %f %f %f %f" $xmin $xmax $ymin $ymax ]
    if {$xmin != 0.01 || $xmax != 0.99 || $ymin != 0.02 || $ymax != 0.49} {
        puts stderr "plgvpd test failed"
        set status 1
    }
    set xmid [ expr 0.5*($xmin+$xmax) ]
    set ymid [ expr 0.5*($ymin+$ymax) ]

    $w cmd plwind 0.2 0.3 0.4 0.5
    $w cmd plgvpw xmin xmax ymin ymax
    puts [ format "plwind: xmin, xmax, ymin, ymax = %f %f %f %f" $xmin $xmax $ymin $ymax ]
    if {$xmin != 0.2 || $xmax != 0.3 || $ymin != 0.4 || $ymax != 0.5} {
        puts stderr "plgvpw test failed"
        set status 1
    }

    # Get world coordinates for middle of viewport
     $w cmd plcalc_world $xmid $ymid wx wy win
    puts [ format "world parameters: wx, wy, win = %f %f %d" $wx $wy $win ]
    if {abs($wx-0.5*($xmin+$xmax)) > 1.0E-5 || abs($wy-0.5*($ymin+$ymax)) > 1.0E-5} {
        puts stderr "plcalc_world test failed"
        set status 1
    }

    # Retrieve and print the name of the output file (if any)
    $w cmd plgfnam fnam 
    if {$fnam == ""} {
	puts [ format "No output file name is set" ]
    } {else} {
	puts [ format "Output file name read" ]
    }
    puts stderr [ format "Output file name is %s" $fnam ]

    # Set and get the number of digits used to display axis labels
    # Note digits is currently ignored in pls[xyz]ax and 
    # therefore it does not make sense to test the returned value.
    $w cmd plsxax 3 0
    $w cmd plgxax digmax digits
    puts [ format "x axis parameters: digmax, digits = %d %d" $digmax $digits ]
    if {$digmax != 3} {
	puts stderr "plgxax test failed"
	set status 1
    }

    $w cmd plsyax 4 0
    $w cmd plgyax digmax digits
    puts [ format "y axis parameters: digmax, digits = %d %d" $digmax $digits ]
    if {$digmax != 4} {
	puts stderr "plgyax test failed"
	set status 1
    }
    $w cmd plszax 5 0
    $w cmd plgzax digmax digits 
    puts [ format "z axis parameters: digmax, digits = %d %d" $digmax $digits ]
    if {$digmax != 5} {
	puts stderr "plgzax test failed"
	set status 1
    }

    $w cmd plsdidev 0.05 $PL_NOTSET 0.1 0.2
    $w cmd plgdidev mar aspect jx jy
    puts [ format "device-space window parameters: mar, aspect, jx, jy = %f %f %f %f" $mar $aspect $jx $jy ]
    if {$mar != 0.05 || $jx != 0.1 || $jy != 0.2} {
	puts stderr "plgdidev test failed"
	set status 1
    }

    $w cmd plsdiori 1.0
    $w cmd plgdiori ori
    puts [ format "ori parameter = %f" $ori ]
    if {$ori != 1.0 } {
	puts stderr "plgdiori test failed"
	set status 1
    }

    $w cmd plsdiplt 0.1 0.2 0.9 0.8
    $w cmd plgdiplt xmin ymin xmax ymax
    puts [ format "plot-space window parameters: xmin, ymin, xmax, ymax = %f %f %f %f" $xmin $ymin $xmax $ymax ]
    if {$xmin != 0.1 || $xmax != 0.9 || $ymin != 0.2 || $ymax != 0.8} {
	puts stderr "plgdiplt test failed"
	set status 1
    }

    $w cmd plsdiplz 0.1 0.1 0.9 0.9
    $w cmd plgdiplt zxmin zymin zxmax zymax
    puts [ format "zoomed plot-space window parameters: xmin, ymin, xmax, ymax = %f %f %f %f" $zxmin $zymin $zxmax $zymax ]
    if {abs($zxmin -($xmin + ($xmax-$xmin)*0.1)) > 1.0E-5 || abs($zxmax -($xmin+($xmax-$xmin)*0.9)) > 1.0E-5 || abs($zymin -($ymin+($ymax-$ymin)*0.1)) > 1.0E-5 || abs($zymax -($ymin+($ymax-$ymin)*0.9)) > 1.0E-5 } {
	puts stderr "plsdiplz test failed"
	set status 1
    }

    $w cmd plscolbg 10 20 30
    $w cmd plgcolbg r g b
    puts [ format "background colour parameters: r, g, b = %d %d %d" $r $g $b ]
    if {$r != 10 || $g != 20 || $b != 30} {
	puts stderr "plgcolbg test failed"
	set status 1
    }

    $w cmd plscolbga 20 30 40 0.5
    $w cmd plgcolbga r g b a
    puts [ format "background/transparency colour parameters: r, g, b, a = %d %d %d %f" $r $g $b $a ]
    if {$r != 20 || $g != 30 || $b != 40 || $a != 0.5} {
	puts stderr "plgcolbga test failed"
	set status 1
    }


    # Restore defaults
    # $w cmd plcol0 1
    $w cmd plend
}

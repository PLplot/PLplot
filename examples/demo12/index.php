<?php include("../example_head.html") ?>

<div ALIGN="CENTER"><b>Select to see the example 12 source code</b></div>

<div ALIGN="CENTER">
<form ACTION="index.php" METHOD="GET">
<input TYPE="submit" NAME="lbind" VALUE="C">
<input TYPE="submit" NAME="lbind" VALUE="F77">
<input TYPE="submit" NAME="lbind" VALUE="Tcl">
<input TYPE="submit" NAME="lbind" VALUE="Java">
<input TYPE="submit" NAME="lbind" VALUE="Python">
<input TYPE="submit" NAME="lbind" VALUE="Octave">

</form>
</div>
<br>

<?php

if (!strcmp($lbind, "C"))
        $fname="x12c.c-";
elseif (!strcmp($lbind, "F77"))
        $fname="x12f.f-";
elseif (!strcmp($lbind, "Tcl"))
        $fname="x12.tcl-";
elseif (!strcmp($lbind, "Java"))
        $fname="x12.java-";
elseif (!strcmp($lbind, "Python"))
        $fname="xw12.py-";
elseif (!strcmp($lbind, "Octave"))
        $fname="x12c.m-";
else
        $fname="x12c.c-";

echo "<OBJECT DATA=\"" . $fname . "\" WIDTH=600 HEIGHT=200 TYPE=\"text/plain\"></object>";

?>

<hr>

<br>
<div ALIGN="CENTER">
<b>Select to view full-size image</b><br>
<A HREF="x12.01.png"><IMG SRC="prev-x12.01.png"></a>
</div>

<?php include("../example_tail.html") ?>

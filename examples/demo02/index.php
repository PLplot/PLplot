<?php include("../example_head.html") ?>

<div ALIGN="CENTER"><b>Select to see the example 2 source code</b></div>

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
        $fname="x02c.c-";
elseif (!strcmp($lbind, "F77"))
        $fname="x02f.f-";
elseif (!strcmp($lbind, "Tcl"))
        $fname="x02.tcl-";
elseif (!strcmp($lbind, "Java"))
        $fname="x02.java-";
elseif (!strcmp($lbind, "Python"))
        $fname="xw02.py-";
elseif (!strcmp($lbind, "Octave"))
        $fname="x02c.m-";
else
        $fname="x02c.c-";

echo "<OBJECT DATA=\"" . $fname . "\" WIDTH=600 height=200 TYPE=\"text/plain\"></object>";

?>

<hr>

<br>


<div ALIGN="CENTER">
<b>Select to view full-size image</b><br>
<A HREF="x02.01.png"><IMG SRC="prev-x02.01.png"></a>
</div>

<?php include("../example_tail.html") ?>

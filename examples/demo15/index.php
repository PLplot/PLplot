<?php include("../example_head.html") ?>

<div ALIGN="CENTER"><b>Select to see the example 15 source code</b></div>

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
        $fname="x15c.c-";
elseif (!strcmp($lbind, "F77"))
        $fname="x15f.f-";
elseif (!strcmp($lbind, "Tcl"))
        $fname="x15.tcl-";
elseif (!strcmp($lbind, "Java"))
        $fname="x15.java-";
elseif (!strcmp($lbind, "Python"))
        $fname="xw15.py-";
elseif (!strcmp($lbind, "Octave"))
        $fname="x15c.m-";
else
        $fname="x15c.c-";

echo "<OBJECT DATA=\"" . $fname . "\" WIDTH=600 HEIGHT=200 TYPE=\"text/plain\"></object>";

?>

<hr>
<br>
<div ALIGN="CENTER"><b>Select to view full-size image</b>
<table>
  <tbody>

    <tr>
      <td> <A HREF="x15.01.png"><IMG SRC="prev-x15.01.png"></a> </td>
      <td> <A HREF="x15.02.png"><IMG SRC="prev-x15.02.png"></a> </td>
    </tr>

  </tbody>
</table>
</div>

<?php include("../example_tail.html") ?>

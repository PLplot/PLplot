<?php include("../example_head.html") ?>

<div ALIGN="CENTER"><b>Select to see the example 20 source code</b></div>

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
        $fname="x20c.c-";
elseif (!strcmp($lbind, "F77"))
        $fname="x20f.f-";
elseif (!strcmp($lbind, "Tcl"))
        $fname="x20.tcl-";
elseif (!strcmp($lbind, "Java"))
        $fname="x20.java-";
elseif (!strcmp($lbind, "Python"))
        $fname="xw20.py-";
elseif (!strcmp($lbind, "Octave"))
        $fname="x20c.m-";
else
        $fname="x20c.c-";

echo "<OBJECT DATA=\"" . $fname . "\" WIDTH=600 HEIGHT=200 TYPE=\"text/plain\"></OBJECT>";

?>

<hr>
<br>
<div ALIGN="CENTER"><b>Select to view full-size image</b>

<table>
  <tbody>

    <tr>
      <td> <A HREF="x20.01.png"><IMG SRC="prev-x20.01.png"></a> </td>
      <td> <A HREF="x20.03.png"><IMG SRC="prev-x20.03.png"></a> </td>
    </tr>

    <tr>
      <td> <A HREF="x20.04.png"><IMG SRC="prev-x20.04.png"></a> </td>
      <td> <A HREF="x20.06.png"><IMG SRC="prev-x20.06.png"></a> </td>
    </tr>

  </tbody>
</table>
</div>

<?php include("../example_tail.html") ?>

<?php include("../example_head.html") ?>

<div ALIGN="CENTER"><b>Select to see the example 9 source code</b></div>

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
        $fname="x09c.c-";
elseif (!strcmp($lbind, "F77"))
        $fname="x09f.f-";
elseif (!strcmp($lbind, "Tcl"))
        $fname="x09.tcl-";
elseif (!strcmp($lbind, "Java"))
        $fname="x09.java-";
elseif (!strcmp($lbind, "Python"))
        $fname="xw09.py-";
elseif (!strcmp($lbind, "Octave"))
        $fname="x09c.m-";
else
        $fname="x09c.c-";

echo "<OBJECT DATA=\"" . $fname . "\" WIDTH=600 HEIGHT=200 TYPE=\"text/plain\"></object>";

?>

<hr>
<br>
<div ALIGN="CENTER"><b>Select to view full-size image</b>

<table>
  <tbody>

    <tr>
      <td> <A HREF="x09.01.png"><IMG SRC="prev-x09.01.png"></a> </td>
      <td> <A HREF="x09.02.png"><IMG SRC="prev-x09.02.png"></a> </td>
    </tr>

    <tr>
      <td> <A HREF="x09.03.png"><IMG SRC="prev-x09.03.png"></a> </td>
      <td> <A HREF="x09.04.png"><IMG SRC="prev-x09.04.png"></a> </td>
    </tr>

    <tr>
      <td> <A HREF="x09.05.png"><IMG SRC="prev-x09.05.png"></a> </td>
    </tr>

  </tbody>
</table>
</div>

<?php include("../example_tail.html") ?>

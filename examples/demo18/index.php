<?php include("../example_head.html") ?>

<div ALIGN="CENTER"><b>Select to see the example 18 source code</b></div>

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
        $fname="x18c.c-";
elseif (!strcmp($lbind, "F77"))
        $fname="x18f.f-";
elseif (!strcmp($lbind, "Tcl"))
        $fname="x18.tcl-";
elseif (!strcmp($lbind, "Java"))
        $fname="x18.java-";
elseif (!strcmp($lbind, "Python"))
        $fname="xw18.py-";
elseif (!strcmp($lbind, "Octave"))
        $fname="x18c.m-";
else
        $fname="x18c.c-";

echo "<OBJECT DATA=\"" . $fname . "\" WIDTH=600 HEIGHT=200 TYPE=\"text/plain\"></object>";

?>

<hr>
<br>
<div ALIGN="CENTER"><b>Select to view full-size image</b>
<table>
  <tbody>

    <tr>
      <td> <A HREF="x18.01.png"><IMG SRC="prev-x18.01.png"></a> </td>
      <td> <A HREF="x18.02.png"><IMG SRC="prev-x18.02.png"></a> </td>
      <td> <A HREF="x18.03.png"><IMG SRC="prev-x18.03.png"></a> </td>
      <td> <A HREF="x18.04.png"><IMG SRC="prev-x18.04.png"></a> </td>
    </tr>

    <tr>
      <td> <A HREF="x18.05.png"><IMG SRC="prev-x18.05.png"></a> </td>
      <td> <A HREF="x18.06.png"><IMG SRC="prev-x18.06.png"></a> </td>
      <td> <A HREF="x18.07.png"><IMG SRC="prev-x18.07.png"></a> </td>
      <td> <A HREF="x18.08.png"><IMG SRC="prev-x18.08.png"></a> </td>
    </tr>

  </tbody>
</table>
</div>

<?php include("../example_tail.html") ?>

<?php include("../example_head.html") ?>


<div ALIGN="CENTER"><b>Select to see the example 11 source code</b></div>

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
        $fname="x11c.c-";
elseif (!strcmp($lbind, "F77"))
        $fname="x11f.f-";
elseif (!strcmp($lbind, "Tcl"))
        $fname="x11.tcl-";
elseif (!strcmp($lbind, "Java"))
        $fname="x11.java-";
elseif (!strcmp($lbind, "Python"))
        $fname="xw11.py-";
elseif (!strcmp($lbind, "Octave"))
        $fname="x11c.m-";
else
        $fname="x11c.c-";

echo "<OBJECT DATA=\"" . $fname . "\" WIDTH=600 HEIGHT=200 TYPE=\"text/plain\"></object>";

?>

<hr>
<br>
<div ALIGN="CENTER"><b>Select to view full-size image</b>
<table>
  <tbody>

    <tr>
      <td> <A HREF="x11.01.png"><IMG SRC="prev-x11.01.png"></a> </td>
      <td> <A HREF="x11.05.png"><IMG SRC="prev-x11.05.png"></a> </td>
    </tr>

    <tr>
      <td> <A HREF="x11.02.png"><IMG SRC="prev-x11.02.png"></a> </td>
      <td> <A HREF="x11.06.png"><IMG SRC="prev-x11.06.png"></a> </td>
    </tr>

    <tr>
      <td> <A HREF="x11.03.png"><IMG SRC="prev-x11.03.png"></a> </td>
      <td> <A HREF="x11.07.png"><IMG SRC="prev-x11.07.png"></a> </td>
      </tr>

    <tr>
      <td> <A HREF="x11.04.png"><IMG SRC="prev-x11.04.png"></a> </td>
      <td> <A HREF="x11.08.png"><IMG SRC="prev-x11.08.png"></a> </td>
    </tr>

  </tbody>
</table>
</div>

<?php include("../example_tail.html") ?>

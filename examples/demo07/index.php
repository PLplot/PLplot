<?php include("../example_head.html") ?>

<div ALIGN="CENTER"><b>Select to see the example 7 source code</b></div>

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
        $fname="x07c.c-";
elseif (!strcmp($lbind, "F77"))
        $fname="x07f.f-";
elseif (!strcmp($lbind, "Tcl"))
        $fname="x07.tcl-";
elseif (!strcmp($lbind, "Java"))
        $fname="x07.java-";
elseif (!strcmp($lbind, "Python"))
        $fname="xw07.py-";
elseif (!strcmp($lbind, "Octave"))
        $fname="x07c.m-";
else
        $fname="x07c.c-";

echo "<OBJECT DATA=\"" . $fname . "\" WIDTH=600 HEIGHT=200 TYPE=\"text/plain\"></object>";

?>

<hr>
<br>
<div ALIGN="CENTER"><b>Select to view full-size image</b>
<table>
  <tbody>

    <tr>
      <td> <A HREF="x07.01.png"><IMG SRC="prev-x07.01.png"></a> </td>
      <td> <A HREF="x07.02.png"><IMG SRC="prev-x07.02.png"></a> </td>
      <td> <A HREF="x07.03.png"><IMG SRC="prev-x07.03.png"></a> </td>
      </tr>

    <tr>
      <td> <A HREF="x07.04.png"><IMG SRC="prev-x07.04.png"></a> </td>
      <td> <A HREF="x07.05.png"><IMG SRC="prev-x07.05.png"></a> </td>
      <td> <A HREF="x07.06.png"><IMG SRC="prev-x07.06.png"></a> </td>
      </tr>

    <tr>

      <td> <A HREF="x07.07.png"><IMG SRC="prev-x07.07.png"></a> </td>
      <td> <A HREF="x07.08.png"><IMG SRC="prev-x07.08.png"></a> </td>
      <td> <A HREF="x07.09.png"><IMG SRC="prev-x07.09.png"></a> </td>
      </tr>

    <tr>
      <td> <A HREF="x07.10.png"><IMG SRC="prev-x07.10.png"></a> </td>
      <td> <A HREF="x07.11.png"><IMG SRC="prev-x07.11.png"></a> </td>
      <td> <A HREF="x07.12.png"><IMG SRC="prev-x07.12.png"></a> </td>
    </tr>

    <tr>
      <td> <A HREF="x07.13.png"><IMG SRC="prev-x07.13.png"></a> </td>
      <td> <A HREF="x07.14.png"><IMG SRC="prev-x07.14.png"></a> </td>
      <td> <A HREF="x07.15.png"><IMG SRC="prev-x07.15.png"></a> </td>
      </tr>

    <tr>
      <td> <A HREF="x07.16.png"><IMG SRC="prev-x07.16.png"></a> </td>
      <td> <A HREF="x07.17.png"><IMG SRC="prev-x07.17.png"></a> </td>
    </tr>

  </tbody>
</table>
</div>

<?php include("../example_tail.html") ?>

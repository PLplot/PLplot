<?php include("../example_head.html") ?>

<div ALIGN="CENTER"><b>Select to see the example 4 source code</b></div>

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
        $fname="x04c.c-";
elseif (!strcmp($lbind, "F77"))
        $fname="x04f.f-";
elseif (!strcmp($lbind, "Tcl"))
        $fname="x04.tcl-";
elseif (!strcmp($lbind, "Java"))
        $fname="x04.java-";
elseif (!strcmp($lbind, "Python"))
        $fname="xw04.py-";
elseif (!strcmp($lbind, "Octave"))
        $fname="x04c.m-";
else
        $fname="x04c.c-";

echo "<OBJECT DATA=\"" . $fname . "\" WIDTH=600 height=200 TYPE=\"text/plain\"></object>";

?>
<
<hr>
<br>
<div ALIGN="CENTER"><b>Select to view full-size image</b>

<table>
  <tbody>

    <tr>
      <td> <A HREF="x04.01.png"><IMG SRC="prev-x04.01.png"></a> </td>
      <td> <A HREF="x04.02.png"><IMG SRC="prev-x04.02.png"></a> </td>
    </tr>

  </tbody>
</table>
</div>
  
<?php include("../example_tail.html") ?>

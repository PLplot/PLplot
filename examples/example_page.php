<?php

function demo_page ($demo, $nfigs, $language) {

  include ("example_head.html");

  $demo_str = sprintf ("%02d", $demo);
  $demo_dir = "../examples-data/demo$demo_str";

  print <<<END
<div ALIGN="CENTER"><b>Select to see the example $demo source code</b></div>

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
END;

  switch ($language) {
  case ("C"):
    $fname = "x" . $demo_str . "c.c-";
    break;
  case ("F77"):
    $fname = "x" . $demo_str . "f.f-";
    break;
  case ("Tcl"):
    $fname = "x" . $demo_str . ".tcl-";
    break;
  case ("Java"):
    $fname = "x" . $demo_str . ".java-";
    break;
  case ("Python"):
    $fname = "xw" . $demo_str . ".py-";
    break;
  case ("Octave"):
    $fname = "x" . $demo_str . "c.m-";
    break;
  default:
    $fname = "x" . $demo_str . "c.c-";
  }

  echo '<OBJECT TYPE="text/plain" DATA="' . "$demo_dir/$fname"
    . '" WIDTH=600 height=200></object>';

  print <<<END
<hr>
<br>

<div ALIGN="CENTER">
<b>Select to view full-size image</b><br>
<table>
  <tbody>
    <tr>

END;

  for ($ndx = 1; $ndx <= $nfigs; ++$ndx) {
    if (($ndx > 1) and ($ndx % 3 == 1)) {
      print <<<END
    </tr>
    <tr>
END;
    }
    $ndx_str = sprintf ("%02d", $ndx);
    echo "      <td> <A HREF=\"$demo_dir/x$demo_str.$ndx_str.png\">"
      . "<IMG SRC=\"$demo_dir/prev-x$demo_str.$ndx_str.png\">"
      . "</a> </td>\n";
  }

  print <<<END
    </tr>
  </tbody>
</table>
</div>
END;

  include ("example_tail.html");

}

?>

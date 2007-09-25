<?php

function demo_page ($demo, $nfigs, $language, $credits) {

  include ("example_head.html");

  $demo_str = sprintf ("%02d", $demo);
  $demo_dir = "../examples-data/demo$demo_str";
  $demos = array_merge (range (1, 13), array (15, 16), range (18, 28));

  print <<<END
<div ALIGN="CENTER"><b>Select to see the example $demo source code</b></div>

<div ALIGN="CENTER">
<form ACTION="demo$demo_str.php" METHOD="GET">

<input TYPE="submit" NAME="lbind" VALUE="C">
<input TYPE="submit" NAME="lbind" VALUE="C++">
<input TYPE="submit" NAME="lbind" VALUE="F77">
<input TYPE="submit" NAME="lbind" VALUE="F95">
<input TYPE="submit" NAME="lbind" VALUE="Ada">
<input TYPE="submit" NAME="lbind" VALUE="Tcl">
<input TYPE="submit" NAME="lbind" VALUE="Java">
<input TYPE="submit" NAME="lbind" VALUE="Python">
<input TYPE="submit" NAME="lbind" VALUE="Octave">
<input TYPE="submit" NAME="lbind" VALUE="PerlDL">

</form>
</div>
<br>
END;

  switch ($language) {
  case ("C"):
    $fname = "x" . $demo_str . "c.c-";
    break;
  case ("C++"):
    $fname = "x" . $demo_str . ".cc-";
    break;
  case ("F77"):
    $fname = "x" . $demo_str . "f.f-";
    break;
  case ("F95"):
    $fname = "x" . $demo_str . "f.f90-";
    break;
  case ("Ada"):
    $fname = "x" . $demo_str . "a.adb-";
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
  case ("PerlDL"):
    $fname = "x" . $demo_str . ".pl-";
    break;
  default:
    $fname = "x" . $demo_str . "c.c-";
  }

  echo '<OBJECT CLASS="code" TYPE="text/plain" DATA="' . "$demo_dir/$fname"
    . '" WIDTH=600 height=200></object>';

  print <<<END
<br>
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
      . "<IMG BORDER=\"0\" SRC=\"$demo_dir/prev-x$demo_str.$ndx_str.png\">"
      . "</a> </td>\n";
  }

  print <<<END
    </tr>
  </tbody>
</table>
</div>
$credits      
<hr>
<div ALIGN="CENTER">
END;

  foreach ($demos as $i) {
    $demo_str = sprintf ("%02d", $i);
    if ($i != $demo) {
      echo "<A HREF=\"demo$demo_str.php?lbind=$language\">x$demo_str</A>\n";
    } else {
      echo "<b>x$demo_str\n</b>";
    }
  }

  print <<<END
</div>
END;

  include ("example_tail.html");

}

?>

<?php
  include "corefunctions.php";

	# Each element of the structure below contains:
	# [ demo#, number of pages, particular thumbnail for top-level page, credits]
  # if number of pages==0 the entry is ignored (e.g. example 14 and 17)
	$demoArray = array(
	  array( 1,  1, 1, ""),
	  array( 2,  2, 1, ""),
	  array( 3,  1, 1, ""),
	  array( 4,  2, 1, ""),
	  array( 5,  1, 1, ""),
	  array( 6,  1, 1, ""),
	  array( 7, 17, 1, ""),
	  array( 8,  8, 1, ""),
	  array( 9,  5, 1, ""),
	  array(10,  1, 1, ""),
	  array(11,  8, 1, ""),
	  array(12,  1, 1, ""),
	  array(13,  1, 1, ""),
	  array(14,  0, 0, ""),
	  array(15,  2, 1, ""),
	  array(16,  5, 1, ""),
	  array(17,  0, 0, ""),
	  array(18,  8, 1, ""),
	  array(19,  3, 1, ""),
	  array(20,  6, 3, ""),
	  array(21,  3, 3, ""),
	  array(22,  4, 3, ""),
	  array(23, 11, 1, ""),
	  array(24,  1, 1, "
	Credits:
	<ul class=\"arrowlist\">
	<li><a href=\"http://www.columbia.edu/~fdc/pace/\">Say Peace in all languages!</a></li>
	<li><a href=\"http://www.columbia.edu/~fdc/pace/flag.html\">Pace flag</a></li>
	</ul>
	"),
	  array(25,  4, 1, ""),
	  array(26,  2, 1, ""),
	  array(27, 10, 1, ""),
	  array(28,  5, 1, ""),
	  array(29,  3, 1, ""),
	  array(30,  2, 1, ""));
?>

<!-- include the html header -->
<?php pageHeader("Examples"); ?>

<body>

<div id="pagewrapper">

  <!-- create the menu, examples is selected -->
	<?php pageMenu("examples"); ?>

  <!-- the actual content of the page -->
	<div id="contentwrapper">
		<div id="fullside">
<?php
  // if the variable demo was set the source code of this example
  // is displayed
	if(isset($_GET["demo"])) {
		$demoID=$_GET["demo"];  // demo number
		$lbind=$_GET["lbind"];  // language
		$demo_str = sprintf("%02d", $demoArray[$demoID-1][0]);
		$nplots = $demoArray[$demoID-1][1];
		$credits = $demoArray[$demoID-1][3];
	  $demo_dir = "examples-data/demo$demo_str";

    // add buttons for language selection
		echo <<<END
		<h3>Example $demoID</h3>
		<p>Select language to see the source code</p>
		<form class="codeform" action="examples.php" method="get">
		<input type="hidden" name="demo" value="$demo_str" />
		<input type="submit" name="lbind" value="C" />
		<input type="submit" name="lbind" value="C++" />
		<input type="submit" name="lbind" value="F77" />
		<input type="submit" name="lbind" value="F95" />
		<input type="submit" name="lbind" value="Ada" />
		<input type="submit" name="lbind" value="Tcl" />
		<input type="submit" name="lbind" value="Java" />
		<input type="submit" name="lbind" value="Python" />
		<input type="submit" name="lbind" value="Octave" />
		<input type="submit" name="lbind" value="PerlDL" />
		<input type="submit" name="lbind" value="Ocaml" />
		</form>
END;

	  switch($lbind) {
	  case ("C"):      $fname = "x" . $demo_str . "c.c-"; break;
	  case ("C++"):$fname = "x" . $demo_str . ".cc-"; break;
	  case ("F77"):    $fname = "x" . $demo_str . "f.f-"; break;
	  case ("F95"):    $fname = "x" . $demo_str . "f.f90-"; break;
	  case ("Ada"):    $fname = "x" . $demo_str . "a.adb-"; break;
	  case ("Tcl"):    $fname = "x" . $demo_str . ".tcl-";  break;
	  case ("Java"):   $fname = "x" . $demo_str . ".java-"; break;
	  case ("Python"): $fname = "xw" . $demo_str . ".py-"; break;
	  case ("Octave"): $fname = "x" . $demo_str . "c.m-"; break;
	  case ("PerlDL"): $fname = "x" . $demo_str . ".pl-"; break;
	  case ("Ocaml"):  $fname = "x" . $demo_str . ".ml-"; break;
	  default:         $fname = "x" . $demo_str . "c.c-";
	  }

    // view code
	  echo "<object id=\"codebox\" type=\"text/plain\" data=\"$demo_dir/$fname\">\n";
		echo "Your browser is not able to display text!\n</object>\n";
			
    // show plots for chosen example
	  echo "<p>Select to view full-size image</p>\n";
		echo "<p>\n";
	  for($ndx = 1; $ndx <= $nplots; ++$ndx) {
	    $ndx_str = sprintf ("%02d", $ndx);
			echo "<a href=\"$demo_dir/x$demo_str.$ndx_str.png\" rel=\"lightbox[plplot]\" title=\"x$demo_str.$ndx_str\">
					<img src=\"$demo_dir/prev-x$demo_str.$ndx_str.png\" border=\"1\" alt=\"x$demo_str.$ndx_str\"/></a>\n";
	    if(($ndx%3 == 0))
	      echo "<br/>\n";
	  }
		echo "</p>\n";

    // show credits if any
		if($credits!="")
			echo "<p>" . $credits . "</p>\n";

		echo "<h4>Choose other examples</h4><p>\n";
		$counter=1;
	  foreach($demoArray as $drec) {
		  $dstr = sprintf ("%02d", $drec[0]);
			if($drec[1]!=0) {
		    if($dstr != $demo_str) {
		      echo "<a href=\"examples.php?demo=$dstr&amp;lbind=$lbind\">x$dstr</a>\n";
		    } else {
		      echo "x$dstr\n";
		    }
				if($counter%20==0)
					echo "<br />\n";
				$counter++;
			}
	  }
		echo "</p>\n";
	} else {
    // display plots if all examples
		echo <<<END
			<h3>Examples</h3>
			<p>These examples were generated with the pngcairo device and
				 reflect the status of PLplot-5.9.0.</p>
			<p>Select a thumbnail to view full-size image and source code.</p>
			<p>
END;
		$count = 0;
		foreach($demoArray as $drec) {
		  $demo_str = sprintf ("%02d", $drec[0]);
		  $dir = "demo$demo_str";
		  $thumbnail = sprintf ("%02d", $drec[2]);
		  $npages = $drec[1];

			if($npages!=0) {
				echo "<a href=\"examples.php?demo=$demo_str\"><img border=\"0\" src=\"examples-data/$dir/prev-x$demo_str.$thumbnail.png\" /></a>\n";
			  $count++;
			  if(($count%3) == 0) echo "<br />\n";  
			}
		}
	}
?>
		</div>
	</div>

  <!-- include the page footer -->
	<?php pageFooter(); ?>
</div>

</body>
</html>

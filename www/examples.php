<?php
  include "corefunctions.php";

	# Each element of the structure below contains:
	# [ demo#, number of pages, particular thumbnail for top-level page, credits]
	# if number of pages==0 the entry is ignored (currently none are ignored).
	$demoArray = array(
	  array( 1,  1, 1, "
	<p>Simple line plot and multiple windows demo.</p>
	\n"),
	  array( 2,  2, 1, "
	<p>Multiple window and color map 0 demo.</p>
	\n"),
	  array( 3,  1, 1, "
	<p>Polar plot demo.</p>
	\n"),
	  array( 4,  2, 1, "
	<p>Log plot demo.</p>
	\n"),
	  array( 5,  1, 1, "
	<p>Histogram demo.</p>
	\n"),
	  array( 6,  1, 1, "
	<p>Symbol demo.</p>
	\n"),
	  array( 7, 17, 1, "
	<p>Font demo for Hershey encodings.</p>
	\n"),
	  array( 8,  8, 1, "
	<p>3-d plot demo.</p>
	\n"),
	  array( 9,  5, 1, "
	<p>Contour plot demo.</p>
	\n"),
	  array(10,  1, 1, "
	<p>Window positioning demo.</p>
	\n"),
	  array(11,  8, 1, "
	<p>Mesh plot demo.</p>
	\n"),
	  array(12,  1, 1, "
	<p>Bar chart demo.</p>
	\n"),
	  array(13,  1, 1, "
	<p>Pie chart demo.</p>
	\n"),
	  array(14,  4, 1, "
	<p>Demo of multiple stream/window capability.  The two pages of the master stream appear as the first two pages here.  The two pages of the slave stream appear as pages 3 and 4 here.</p>
	\n"),
	  array(15,  3, 1, "
	<p>Shade plot demo using color map 0 (discrete colors) (plshade).</p>
	\n"),
	  array(16,  5, 1, "
	<p>Shade plot demo using color map 1 (continuous colours) (plshades).</p>
	\n"),
	  array(17,  1, 1, "
	<p>4-pen strip chart demo.</p>
	\n"),
	  array(18,  8, 1, "
	<p>3-d line and point plot demo.</p>
	\n"),
	  array(19,  3, 1, "
	<p>Demo of backdrop plotting of world, US maps.</p>
	\n"),
	  array(20,  6, 3, "
	<p>Demo of gray-scale image manipulation (plimage).</p>
	\n"),
	  array(21,  3, 3, "
	<p>Grid data demo.</p>
	\n"),
	  array(22,  4, 3, "
	<p>Demo of simple vector plotting.</p>
	\n"),
	  array(23, 16, 1, "
	<p>Font demo for unicode encodings which shows Greek letters and math symbols.</p>
	\n"),
	  array(24,  1, 1, "
	<p>Demo of unicode Pace Flag.</p>
	<p>Credits:</p>
	<ul class=\"arrowlist\">
	<li><a href=\"http://www.columbia.edu/~fdc/pace/\">Say Peace in all languages!</a></li>
	<li><a href=\"http://www.columbia.edu/~fdc/pace/flag.html\">Pace flag</a></li>
	</ul>
	\n"),
	  array(25,  4, 1, "
	<p>Diagnostic demo of filling and clipping polygons.</p>
	\n"),
	  array(26,  2, 1, "
	<p>Demo of multi-lingual unicode versions of the first page of example 4.</p>
	\n"),
	  array(27, 10, 1, "
	<p>Demo of drawing 'spirograph' curves - epitrochoids, cycolids, roulettes.</p>
	\n"),
	  array(28,  5, 1, "
	<p>Demo of 3-d writing of text (plmtex3 and plptex3).</p>
	\n"),
	  array(29,  3, 1, "
	<p>Demo of date / time formatting for axes.</p>
	\n"),
	  array(30,  2, 1, "
	<p>Demo of opacity/ transparency (or alpha channel).</p>
	\n"),
	  array(31,  1, 1, "
	<p>Diagnostic demo to exercise all our set/get functions.  The result should be a properly-formatted empty plot with black background.</p>
	\n"),
	  array(32,  1, 1, "
	<p>Demo of how to draw box-whisker / candlestick plots using PLplot. These are commonly used for financial data.</p>
	\n"));
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
		<input type="submit" name="lbind" value="Ada(thick)" />
		<input type="submit" name="lbind" value="Ada(thin)" />
		<input type="submit" name="lbind" value="C" />
		<input type="submit" name="lbind" value="C++" />
		<input type="submit" name="lbind" value="F77" />
		<input type="submit" name="lbind" value="F95" />
		<input type="submit" name="lbind" value="Java" />
		<input type="submit" name="lbind" value="Ocaml" />
		<input type="submit" name="lbind" value="Octave" />
		<input type="submit" name="lbind" value="PerlDL" />
		<input type="submit" name="lbind" value="Python" />
		<input type="submit" name="lbind" value="Tcl" />
		</form>
END;

	  switch($lbind) {
	  case ("Ada(thick)"):    $fname = "xthick" . $demo_str . "a.adb-"; break;
	  case ("Ada(thin)"):    $fname = "x" . $demo_str . "a.adb-"; break;
	  case ("C"):      $fname = "x" . $demo_str . "c.c-"; break;
	  case ("C++"):$fname = "x" . $demo_str . ".cc-"; break;
	  case ("F77"):    $fname = "x" . $demo_str . "f.f-"; break;
	  case ("F95"):    $fname = "x" . $demo_str . "f.f90-"; break;
	  case ("Java"):   $fname = "x" . $demo_str . ".java-"; break;
	  case ("Ocaml"):  $fname = "x" . $demo_str . ".ml-"; break;
	  case ("Octave"): $fname = "x" . $demo_str . "c.m-"; break;
	  case ("PerlDL"): $fname = "x" . $demo_str . ".pl-"; break;
	  case ("Python"): $fname = "xw" . $demo_str . ".py-"; break;
	  case ("Tcl"):    $fname = "x" . $demo_str . ".tcl-";  break;
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
	  }
		echo "</p>\n";

    // show credits if any
		if($credits!="")
			echo "$credits";

		echo "<h4>Choose other examples</h4><p>\n";
	  foreach($demoArray as $drec) {
		  $dstr = sprintf ("%02d", $drec[0]);
			if($drec[1]!=0) {
		    if($dstr != $demo_str) {
		      echo "<a href=\"examples.php?demo=$dstr&amp;lbind=$lbind\">x$dstr</a>\n";
		    } else {
		      echo "x$dstr\n";
		    }
			}
	  }
		echo "</p>\n";
	} else {
    // display plots if all examples
		echo <<<END
			<h3>Examples</h3>
			<p>These examples were generated with the pngcairo device and
				 reflect the status of PLplot-5.9.3.</p>
			<p>Select a thumbnail to view full-size image and source code.</p>
			<p>
END;
		foreach($demoArray as $drec) {
		  $demo_str = sprintf ("%02d", $drec[0]);
		  $dir = "demo$demo_str";
		  $thumbnail = sprintf ("%02d", $drec[2]);
		  $npages = $drec[1];

			if($npages!=0) {
				echo "<a href=\"examples.php?demo=$demo_str\"><img border=\"0\" src=\"examples-data/$dir/prev-x$demo_str.$thumbnail.png\" alt=\"x$demo_str.thumbnail\"/></a>\n";
			}
		}
		echo "</p>";
	}
?>
		</div>
	</div>

  <!-- include the page footer -->
	<?php pageFooter(); ?>
</div>

</body>
</html>

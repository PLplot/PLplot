<?php
  include "corefunctions.php";
?>

<?php pageHeader("Downloads"); ?>

<body>

<div id="pagewrapper">

	<?php pageMenu("download"); ?>

	<div id="contentwrapper">
		<div id="leftside">
			<h3>Obtain the source code of the PLplot library</h3>
		  <p>You can obtain the latest stable version of PLplot from our
		     <a href="http://sourceforge.net/projects/plplot/files/plplot/">file
		     release site</a>. Alternatively, you can obtain the cutting-edge version of
		     PLplot (but with no promises about stability) using</p>
		  <p><tt>git clone git://git.code.sf.net/p/plplot/plplot plplot.git</tt></p>
		     <p>Finally, you can browse our git repository
		     <a href="http://sourceforge.net/p/plplot/plplot/ci/master/tree">
		     here</a>.</p>
			<h3>PLplot Packages for Linux Distributions</h3>
		  <ul class="arrowlist">
<li><a href="http://packages.ubuntu.com/search?keywords=plplot&amp;searchon=names&amp;suite=all&amp;section=all">Ubuntu</a></li>
<li><a href="http://packages.debian.org/search?searchon=sourcenames&amp;keywords=plplot">Debian</a></li>
<li><a href="http://rpmfind.net/linux/rpm2html/search.php?query=plplot&amp;submit=Search+...">Rpmfind Results</a></li>
<li><a href="http://rpm.pbone.net/">http://rpm.pbone.net/</a> (enter "plplot" search term)</li>
		  </ul>
		</div>

		<?php pageSidebar(1); ?>

		<div id="spacer"></div>
	</div>

	<?php pageFooter(); ?>
</div>

</body>
</html>

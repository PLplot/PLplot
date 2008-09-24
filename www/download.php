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
		     <a href="http://sourceforge.net/project/showfiles.php?group_id=2915">file
		     release site</a>. Alternatively, you can obtain the cutting-edge version of
		     PLplot (but with no promises about stability) from SVN via anonymous
		     access to our <a href="http://sourceforge.net/svn/?group_id=2915">
		     SVN repository</a>.  Finally, you can browse our SVN repository
		     <a href="http://plplot.svn.sourceforge.net/viewvc/plplot/">
		     here</a>.</p>

			<h3>Subversion Access</h3>
		  <p>The PLplot Subversion repository can be checked out on the command line through svn
		     with the following instruction set:</p>
		  <p><tt>svn co https://plplot.svn.sourceforge.net/svnroot/plplot/trunk plplot</tt></p>

		  <h4>Subversion Clients</h4>
		  <ul class="arrowlist">
		  <li>SvnX (Mac OS X): <a href="http://www.lachoseinteractive.net/en/community/subversion/svnx/features/">
		      http://www.lachoseinteract......nx/features/</a></li>
		  <li>TortoiseSVN (Windows): <a href="http://tortoisesvn.tigris.org/">http://tortoisesvn.tigris.org/</a></li>
		  <li>KDESvn (Linux): <a href="http://kdesvn.alwins-world.de/wiki">http://kdesvn.alwins-world.de/wiki</a></li>
		  </ul>
		</div>

		<?php pageSidebar(1); ?>

		<div id="spacer"></div>
	</div>

	<?php pageFooter(); ?>
</div>

</body>
</html>

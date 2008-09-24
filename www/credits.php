<?php
  include "corefunctions.php";
?>

<?php pageHeader("Credits"); ?>

<body>

<div id="pagewrapper">

	<?php pageMenu("credits"); ?>

	<div id="contentwrapper">
		<div id="leftside">
			<h3>The Authors</h3>
		  <p>The current version of PLplot is primarily a product of Maurice J. LeBrun
				 and Geoffrey Furnish.  The conversion of the documentation to DocBook form
				 and subsequent upgrading of that documentation is primarily the product of
				 Rafael Laboissiere and Alan W. Irwin.  These four people and Joao Cardoso,
				 Vince Darley, Arjen Markus, Andrew Roach, Andrew Ross, and David Schleef
				 (the other members of the
				 <a href="http://sourceforge.net/project/memberlist.php?group_id=2915">development
				 team</a>) continue to commit documentation and code to the SVN repository.</p>
		</div>

		<?php pageSidebar(1); ?>

		<div id="spacer"></div>
	</div>

	<?php pageFooter(); ?>
</div>

</body>
</html>

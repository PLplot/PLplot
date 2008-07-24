<?php
  include "corefunctions.php";
?>

<?php pageHeader("Template"); ?>

<body>

<div id="pagewrapper">

	<?php pageMenu("template"); ?>

	<div id="contentwrapper">
		<div id="leftside">
			<h3>Title</h3>
		  <p>text</p>
		</div>

		<?php pageSidebar(); ?>

		<div id="spacer"></div>
	</div>

	<?php pageFooter(); ?>
</div>

</body>
</html>

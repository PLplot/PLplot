<?php
	//
	// Print standard page header
	//
	function pageHeader($title="Main")
	{
		echo <<<END
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--
Copyright 2008-2010 Werner Smekal
Copyright 2008-2011 Hazen Babcock
Copyright 2008-2026 Alan W. Irwin
-->

<head>
  <title>PLplot Home Page - $title</title>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <meta name="description" content="Homepage of the open source PLplot project" />
  <meta name="keywords" content="plot,library,c++,python" />
  <link rel="stylesheet" title="liquid" href="css/style_liquid.css" type="text/css" media="screen,projection" />
  <link rel="alternate stylesheet" title="static" href="css/style_static.css" type="text/css" media="screen,projection" />
  <script type="text/javascript" src="js/slimbox.js"></script>
  <link rel="stylesheet" href="css/slimbox.css" type="text/css" media="screen" />
</head>

END;
	}

	//
	// Print menu code with selected element
	// $item might be: index, downloads, examples, development, credits
	//
	function pageMenu($item)
	{
		echo "	<div id=\"pageheader\">\n";
		echo "	</div>\n";

		echo '	<div id="menubar">';
		echo '		<ul>';
		echo '		  <li><a href="index.php" ' . (($item=='index') ? ('id="selected"') : ('')) .'>Home</a></li>';
		echo '		  <li><a href="http://sourceforge.net/p/plplot/news" ' . (($item=='news') ? ('id="selected"') : ('')) .'>News</a></li>';
		echo '		  <li><a href="downloads.php" ' . (($item=='downloads') ? ('id="selected"') : ('')) .'>Downloads</a></li>';
		echo '		  <li><a href="examples.php" ' . (($item=='examples') ? ('id="selected"') : ('')) .'>Examples</a></li>';
		echo '		  <li><a href="documentation.php" ' . (($item=='documentation') ? ('id="selected"') : ('')) .'>Documentation</a></li>';
//		echo '		  <li><a href="development.php" ' . (($item=='development') ? ('id="selected"') : ('')) .'>Development</a></li>';
		echo '		  <li><a href="credits.php" ' . (($item=='credits') ? ('id="selected"') : ('')) .'>Credits</a></li>';
		echo '		</ul>';
		echo '	</div>';
	}

	//
	// Print a standard page footer
	//
	function pageFooter()
	{
		echo '		<div id="pagefooter">';
		echo '			<p>Original design by <a href="http://DesignsByDarren.com">DesignsByDarren.com</a>, Some Rights Reserved.<br/>Design modifications &copy; 2008-2015 Plplot developer community.<br /><br/>';
		echo '			<a href="http://validator.w3.org/check?uri=referer"><img style="border:0;width:88px;height:31px" src="http://www.w3.org/Icons/valid-xhtml10" alt="Valid XHTML 1.0 Transitional" /></a>';
		echo '      <a href="http://jigsaw.w3.org/css-validator/check?uri=referer"><img style="border:0;width:88px;height:31px" src="http://jigsaw.w3.org/css-validator/images/vcss" alt="Valid CSS!" /></a>';
		echo '      </p>';
		echo '		</div>';
	}

	// Print a standard sidebar.

	// N.B. A previously available (dynamic) display of the latest
	// three news items using a modern simplepie/php approach
	// "just worked" on a copy of our website hosted by a localhost
	// apache2 webserver that allows dynamic content.  However, that
	// copy of our website failed to work at plplot.sf.net, and I
	// have attributed that issue to SourceForge no longer allowing
	// dynamic content for project websites.  As far as I know, the
	// rest of our web content is static.  So to adjust to the
	// plplot.sf.net conditions, I have stripped out all dynamic
	// display of news from our website after committing that previous
	// version of our website that worked perfectly for localhost.

	function pageSidebar()
	{
		echo <<<END
		<!-- Sidebar -->
		<div id="rightside">
		     <h3>Resources</h3>
		     <ul class="arrowlist">
		     	 <li><a href="http://sourceforge.net/p/plplot/news">News</a></li>
		     	 <li><a href="documentation.php">Documentation</a></li>
			 <li><a href="http://sourceforge.net/p/plplot/wiki">Wiki</a></li>
			 <li><a href="http://sourceforge.net/projects/plplot">SourceForge Project Page</a></li>
			 <li><a href="http://sourceforge.net/projects/plplot/support">Project Support Page</a></li>
			 <li><a href="http://sourceforge.net/p/plplot/mailman/">Mailing Lists</a></li>
			 <li><a href="http://sourceforge.net/p/plplot/bugs/">Bug Tracker</a></li>
		     </ul>
		     <h3>Source Code</h3>
		     <ul class="arrowlist">
		     	 <li><a href="http://sourceforge.net/projects/plplot/files/plplot/">Releases</a></li>
			 <li><a href="http://sourceforge.net/p/plplot/plplot/ci/master/tree/">Browse git repository</a></li>
		     </ul>
		</div>
END;
	}

?>

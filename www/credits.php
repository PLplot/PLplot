<?php
  include "corefunctions.php";
?>

<?php pageHeader("Credits"); ?>

<body>

<div id="pagewrapper">

	<?php pageMenu("credits"); ?>

	<div id="contentwrapper">
		<div id="leftside">
			<h3>Developers</h3>
		  <p>Several developers contributed to the PLplot library, some of them are not
				 part of the active development team any more. You can view a list of all
				 active developers at the <a href=http://sourceforge.net/project/memberlist.php?group_id=2915">
				 Sourceforge memberpage</a> of the PLplot project.</p>
				 
			<p>A more complete list, including inactive developers but very likely
			   not all of them, can be found at the <a href="http://www.ohloh.net/projects/10094/contributors">
			   PLplot project page</a> at <a href="http://www.ohloh.net/">Ohloh</a>. In
			   addition many others contributed by providing patches, finding bugs
         or requesting features.</p>
         
      <h3>Additional libraries</h3>
      <p>The Plplot library depends on many other open source libraries provided by 
      the community. These libraries enhance the functionality of the PLplot library
      considerably.</p>
      <ul class="arrowlist">
      <li><a href="http://www.qhull.org/">Qhull</a>: computes the convex hull, Delaunay
          triangulation, Voronoi diagram, halfspace intersection about a point,
          furthest-site Delaunay triangulation, and furthest-site Voronoi diagram.</li>
      <li><a href="http://www.freetype.org/">FreeType</a>: a free, high-quality, and 
      	  portable font engine.</li>
      <li><a href="http://www.libharu.org/">libHaru</a>: free, cross platform, open
      	  source library for generating PDF files.</li>
      </ul>
      
      <h3>Tools</h3>
      <p>The development of the Plplot project is supported by the use of several
         open source tools.</p>
      <ul class="arrowlist">
      <li><a href="http://www.cmake.org/">CMake</a>: cross-platform, open-source build system.</li>
      </ul>
		</div>

		<?php pageSidebar(1); ?>

		<div id="spacer"></div>
	</div>

	<?php pageFooter(); ?>
</div>

</body>
</html>

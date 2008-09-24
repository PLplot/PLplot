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
      <p>The Plplot library depends on some other open source libraries provided by 
      the community.</p>
      <h4>PLplot core dependencies</h4>
      <ul class="arrowlist">
      <li><a href="http://www.qhull.org/">Qhull</a>: computes the convex hull, Delaunay
          triangulation, Voronoi diagram, halfspace intersection about a point,
          furthest-site Delaunay triangulation, and furthest-site Voronoi diagram.</li>
			</ul>

      <h4>PLplot driver dependencies</h4>
      <ul class="arrowlist">
      <li><a href="http://www.freetype.org/">FreeType</a>: a free, high-quality, and 
      	  portable font engine.</li>
      <li><a href="http://www.libharu.org/">libHaru</a>: free, cross platform, open
      	  source library for generating PDF files.</li>
      <li><a href="http://www.cairographics.org/">cairo</a>: a 2D graphics library
          with support for multiple output devices. Currently supported output
          targets include the X Window System, Quartz, Win32, image buffers,
          PostScript, PDF, and SVG file output.</li>
      <li><a href="http://www.pango.org/">Pango</a>: a library for laying out
          and rendering of text, with an emphasis on internationalization.</li>
      <li><a href="http://www.antigrain.com/">Anti-Grain Geometry</a>: an open source, 
          free of charge high quality rendering engine, written in industrially standard C++.</li>
      <li><a href="http://www.libgd.org/">gd library</a>: an open source code library
                for the dynamic creation of images by programmers.</li>
      </ul>
      
      <h3>Tools</h3>
      <p>The development of the Plplot project is supported by the use of several
         open source tools.</p>
      <ul class="arrowlist">
      <li><a href="http://www.cmake.org/">CMake</a>: cross-platform, open-source build system.</li>
      <li><a href="http://www.swig.org/">SWIG</a>: a software development tool that
          connects programs written in C and C++ with a variety of high-level
          programming languages.</li>
      <li><a href="http://pkg-config.freedesktop.org/wiki/">pkg-config</a>: a helper
          tool used when compiling applications and libraries.</li>
      <li><a href="http://www.docbook.org/">docbook</a>: a schema (available in several
          languages including RELAX NG, SGML and XML DTDs, and W3C XML Schema) which is
          particularly well suited to books and papers about computer hardware and software.</li>
      </ul>
		</div>

		<?php pageSidebar(1); ?>

		<div id="spacer"></div>
	</div>

	<?php pageFooter(); ?>
</div>

</body>
</html>

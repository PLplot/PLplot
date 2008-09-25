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
		  <p>Many developers have contributed to PLplot over its long history.  The list of the current core developers is <a href="http://sourceforge.net/project/memberlist.php?group_id=2915">here.</a> A more complete list of developers is at the <a href="http://www.ohloh.net/projects/10094/contributors"> PLplot project page</a> at <a href="http://www.ohloh.net/">Ohloh</a>. In addition many other individuals have contributed to PLplot by providing patches, finding bugs, or requesting features.</p>
         
      <h3>Additional libraries</h3>
      <p>The Plplot library depends on some other open source libraries provided by 
      the community.</p>
      <h4>PLplot core dependencies</h4>
      <ul class="arrowlist">
      <li><a href="http://www.qhull.org/">Qhull</a>: computes the convex hull, Delaunay
          triangulation, Voronoi diagram, halfspace intersection about a point,
          furthest-site Delaunay triangulation, and furthest-site Voronoi diagram.</li>
			</ul>

      <h4>PLplot device-driver dependencies</h4>
      <ul class="arrowlist">
<p> N.B. each device driver is an optional component of PLplot.</p>
      <li>The aquaterm device driver depends on <a href="http://aquaterm.sourceforge.net/">AquaTerm.app</a>; a Mac OS X plotting front end (grahics terminal) written using Objective-C and the Cocoa API.</li>
      <li>The cairo device driver depends on <a href="http://www.pango.org/">Pango</a>; a library for laying out and rendering of text, with an emphasis on internationalization.</li>
      <li>The cairo device driver depends on <a href="http://www.cairographics.org/">cairo</a>; a 2D graphics library with support for multiple output devices. Currently supported output targets include the X Window System, Quartz, Win32, image buffers, PostScript, PDF, and SVG file output.</li>
      <li>The gcw device driver depends on <a href="http://www.gtk.org/">GTK+</a>; a multi-platform toolkit library for creating graphical user interfaces.</li>
      <li>The gcw, gd, wingcc, and wxwidgets device drivers depend on <a href="http://www.freetype.org/">FreeType</a>; a free, high-quality, and portable font engine.</li>
      <li>The gd device driver depends on <a href="http://www.libgd.org/">libgd</a>; an open-source code library for the dynamic creation of images by programmers.</li>
      <li>The pdf device driver depends on <a href="http://www.libharu.org/">libHaru</a>; a free, cross-platform, open-source library for generating PDF files.</li>
      <li>The psttf device driver depends on <a href="http://www.unifont.org/lasi/">LASi</a>; a library that provides a C++ stream output interface (with operator <<) for creating Postscript documents that can contain characters from any of the scripts and symbol blocks supported in Unicode and by Owen Taylor's Pango layout engine.</li>
      <li>The wxwidgets device driver depends on <a href="http://www.wxwidgets.org/">wxWidgets</a>; a GUI and tools library that lets developers create GUI applications for Win32, Mac OS X, GTK+, X11, Motif, WinCE, and more using one codebase.</li>
      <li>The wxwidgets device driver optionally depends on <a href="http://www.antigrain.com/">Anti-Grain Geometry</a>; an open-source, free-of-charge, high-quality rendering engine written in C++.</li>
      </ul>
      
      <h3>Tools</h3>
      <p>The development of the Plplot project is supported by the use of several open source tools.</p>
      <ul class="arrowlist">
      <li><a href="http://www.cmake.org/">CMake</a>, a cross-platform, open-source build system.</li>
      <li><a href="http://www.swig.org/">SWIG</a>, a software development tool that connects programs written in C and C++ with a variety of high-level programming languages.</li>
      <li><a href="http://pkg-config.freedesktop.org/wiki/">pkg-config</a>, a helper tool used when compiling applications and libraries.</li>
      <li><a href="http://www.docbook.org/">DocBook</a>, an XML schema (available in several languages including RELAX NG, SGML and XML DTDs, and W3C XML Schema) which is particularly well suited for writing books and papers about computer hardware and software.</li>
      </ul>
		</div>

		<?php pageSidebar(1); ?>

		<div id="spacer"></div>
	</div>

	<?php pageFooter(); ?>
</div>

</body>
</html>

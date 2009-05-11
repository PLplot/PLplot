<?php
  include "corefunctions.php";
?>

<!-- include the html header -->
<?php pageHeader("Main"); ?>

<body>

<div id="pagewrapper">

  <!-- create the menu, index is selected -->
	<?php pageMenu("index"); ?>

  <!-- the actual content of the page -->
  	<div id="contentwrapper">
	<div id="leftside">
	<h3>Introduction</h3>
        <p>PLplot is a cross-platform software package for creating scientific plots.  To help accomplish that task it is organized as a core C library, language bindings for that library, and device drivers which control how the plots are presented in non-interactive and interactive plotting contexts.</p>
	<p>The PLplot core library can be used to create standard x-y plots, semi-log plots, log-log plots, contour plots, 3D surface plots, mesh plots, bar charts and pie charts. Multiple graphs (of the same or different sizes) may be placed on a single page, and multiple pages are allowed for those device formats that support them.</p>
	<p>PLplot has core support for Unicode. This means plots can be labelled using the enormous selection of Unicode mathematical symbols for simple text layout (left to right) language scripts using our libfreetype-based devices. For our SVG and libpango/libcairo-based devices we additionally support complex text layout (CTL) languages such as Arabic, Hebrew, and Indic and Indic-derived CTL scripts such as Devanagari, Thai, Lao, and Tibetan.</p>
        <p>A number of compiled and interpreted languages have access to PLplot, <a href="#languages">see our list of language bindings</a>.</p>
        <p>PLplot device drivers support <a href="#files">a number of different file formats</a> for non-interactive plotting and <a href="#interactive">a number of different platforms</a> that are suitable for interactive plotting.  It is easy to add new device drivers to PLplot by writing a small number of device dependent routines.</p>
<p>PLplot is free software primarily licensed under the
				 <a href="http://www.gnu.org/licenses/lgpl.html">LGPL</a>.</p>

			<h3>Feature Summary</h3>
			<div class="splitcontentleft">
				<h4>Cross Platform</h4>
				 <p>PLplot is currently known to work on the following platforms:</p>
				 <ul class="arrowlist">
				   <li>Linux, Mac OS X, and other Unices</li>
				   <li>Windows (2000, XP and Vista)</li>
				   <li>MS-DOS (DJGPP)</li>
				 </ul>

				 <p>With the exception of DJGPP, where we have implemented a custom build system, PLplot is built on the above platforms using our CMake-based build system.</p>
				<h4><a name="languages">Language Bindings</a></h4>
				<p>There are many bindings to the PLplot library</p>
				<ul class="arrowlist">
				  <li>Ada</li>
				  <li>C/C++/D</li>
				  <li>Fortran 77/90</li>
				  <li>Java</li>
				  <li><a href="http://common-lisp.net/project/cl-plplot/">Lisp</a></li>
				  <li>Lua</li>
				  <li>OCaml</li>
				  <li>Octave</li>
				  <li><a href="http://search.cpan.org/~dhunt/PDL-Graphics-PLplot-0.47/plplot.pd">Perl</a></li>
				  <li>Python</li>
				  <li>Tcl/Tk</li>
				</ul>
			</div>
			<div class="splitcontentright">
				<h4><a name="files">Output file formats</a></h4>
				<p>PLplot device drivers support a number of plotting file formats.</p>
				<ul class="arrowlist">
				  <li>CGM</li>
				  <li>GIF</li>
				  <li>JPEG</li>
				  <li>LaTeX</li>
				  <li>PBM</li>
				  <li>PDF</li>
				  <li>PNG</li>
				  <li>PostScript</li>
				  <li>SVG</li>
				  <li>Xfig</li>
				</ul>
				<h4><a name="interactive">Interactive Platforms</a></h4>
				<p>PLplot device drivers support a number of platforms that are suitable for interactive plotting.</p>
				<ul class="arrowlist">
				  <li>GNOME</li>
				  <li>Gtk+</li>
				  <li>PyQt</li>
				  <li>Qt</li>
				  <li>Tcl/Tk</li>
				  <li>wxWidgets</li>
				  <li>X</li>
				</ul>
			</div>
			<div class="clear"></div>

		
      <!-- show some plots using the lightbox js script -->
			<h3>Screenshots</h3>
			<p>
			<a href="examples-data/demo03/x03.01.png" rel="lightbox[plplot]" title="Polar plot">
				<img src="examples-data/demo03/x03.01.png" width="172" height="129" border="1" alt="Polar plot"/></a>
			<a href="examples-data/demo08/x08.03.png" rel="lightbox[plplot]" title="Shade 3D plots">
				<img src="examples-data/demo08/x08.03.png" width="172" height="129" border="1" alt="Shade 3D plots"/></a>
			<a href="examples-data/demo09/x09.01.png" rel="lightbox[plplot]" title="Contouring plot">
				<img src="examples-data/demo09/x09.01.png" width="172" height="129" border="1" alt="Contouring plot"/></a>
			<a href="examples-data/demo16/x16.01.png" rel="lightbox[plplot]" title="False colour plot">
				<img src="examples-data/demo16/x16.01.png" width="172" height="129" border="1" alt="False colour plot"/></a>
			<a href="examples-data/demo24/x24.01.png" rel="lightbox[plplot]" title="Unicode example">
				<img src="examples-data/demo24/x24.01.png" width="172" height="129" border="1" alt="Unicode example"/></a>
			<a href="examples-data/demo30/x30.01.png" rel="lightbox[plplot]" title="Alpha color support">
				<img src="examples-data/demo30/x30.01.png" width="172" height="129" border="1" alt="Alpha color support"/></a>
			</p>
			<p>You can find the code for these and other examples at the <a href="examples.php">examples page</a></p>
		</div>

    <!-- add the sidebar, news displayed -->
    <?php pageSidebar(1); ?>

		<div id="spacer"></div>
	</div>

  <!-- include the page footer -->
	<?php pageFooter(); ?>
</div>

</body>
</html>

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
			<p>PLplot is a library of functions that are useful for making scientific
				 plots.</p>
			<p>PLplot can be used from within compiled languages such as C, C++, D,
				 FORTRAN and Java, and interactively from interpreted languages
				 such as Octave, Python, Perl and Tcl.</p>
      <p>The PLplot library can be used to create standard x-y plots, semi-log plots,
				 log-log plots, contour plots, 3D surface plots, mesh plots, bar charts and
				 pie charts. Multiple graphs (of the same or different sizes) may be
				 placed on a single page with multiple lines in each graph.</p>
      <p>A variety of output file devices such as Postscript, png, jpeg
				 and others, as well as interactive devices such as xwin, tk,
				 xterm and Tektronics devices are supported. There are also interfaces
         available to include the PLplot library in your application, e.g.
         for <a href="http://www.wxwidgets.org">wxWidgets</a>. New devices can be easily
				 added by writing a small number of device dependent routines.</p>
      <p>PLplot has core support for Unicode. This means plots can be labelled using
         the enormous selection of Unicode mathematical symbols and simple text
         layout (left to right) language scripts for many of our devices. For our
         cairo-related devices we also support complex text layout (CTL) languages
         such as Arabic, Hebrew, and Indic and Indic-derived CTL scripts such as
         Devanagari, Thai, Lao, and Tibetan.</p>
      <p>Some devices support their own way of dealing with text,
				 such as the Postscript driver (using postscript fonts), or the png and jpeg drivers
         (which use the Freetype library).</p>
			<p>PLplot is free software primarily licensed under the
				 <a href="http://www.gnu.org/licenses/lgpl.html">LGPL</a>.</p>

			<h3>Feature Summary</h3>
			<div class="splitcontentleft">
				<h4>Cross Platform</h4>
				 <p>PLplot is currently known to work on following platforms:</p>
				 <ul class="arrowlist">
				   <li>Linux and other Unixes</li>
				   <li>Windows (2000, XP and Vista)</li>
				   <li>Mac OS X</li>
				   <li>MS-DOS (DJGPP)</li>
				 </ul>
				<h4>Output file formats</h4>
          <p>pdf, svg, postscript, cgm, png, jpeg, gif pbm, xfig and latex</p>
        <h4>Interactive Platforms</h4>
          <p>X, Tk/Tcl, wxWidgets, Gnome</p>
			</div>
			<div class="splitcontentright">
				<h4>Language Bindings</h4>
				<p>There are many bindings to the PLplot library</p>
				<ul class="arrowlist">
				  <li>C/C++</li>
				  <li>FORTAN 77/90</li>
				  <li>D</li>
				  <li>Java</li>
				  <li>Python</li>
				  <li>Perl</li>
				  <li>Ada</li>
				  <li>OCaml</li>
				  <li>Octave</li>
				  <li>TCL/Tk</li>
				  <li>wxWidgets</li>
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

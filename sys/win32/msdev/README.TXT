README file for the win32/msdev port of plplot
==============================================

I will here give some background information, information 
on current status and future developments of the win32/msdev 
port of plplot. For installation instructions please see the
plplot\sys\win32\msdev\INSTALL.TXT.

The win32\msdev port of plplot were originally written by
Paul Casteels (casteels@uia.ua.ac.be) and Carla Carmelo Rosa  
(l36658@alfa.ist.utl.pt). However, Paul and Carla have not
done any new developments since 1996. I managed to pick up
their code and make it work under 98/NT/2000. My colleague
Alexandre Gobbo (gobbo@esrf.fr) has provided very valuable
help in this work. Even though we had to make substantial 
changes of their code it is essentially the same so the credit 
of the win32/msdev port of plplot in general and the win3 
driver in particular should go as much to Paul and Carla as 
to Alexandre and me.

Please note that the work on the win32/msdev port is under
progress. The plplot distribution contains makefiles for
making the plplot.lib library and the c examples, however
the work on the plplot.dll is not yet finished. All C 
examples compile and run except x14c (which needs TCL) and 
x17c (problem with refreshments of the plot). x20c, which 
demonstrates the new function plimage, works but is rather 
slow (see below). 

Here's a list of (known!) missing features from the win3
device:

* Only cursor movements are reported back via the plGetCursor
  function, not any key strokes.
* It is not a fast plimage device, however the work on
  making it one is well advanced.

If you notice more missing features or bugs in the win32/msdev
port of plplot please let me know. 

I hope to be able to finish the work on the plplot.dll and
the fast plimage soon.

If you have comments or suggestions on how to improve the
win32/msdev port of plplot please dont't hesitate to contact 
me via email.


January 2002

Olof Svensson
European Synchrotron Radiation Facility
email: svensson@esrf.fr



January 2004

I have volunteered to take over the maintenance of this
port. So, please contact me about problems and suggestions:

Arjen Markus
WL | Delft Hydraulics
email: arjen.markus@wldelft.nl


Important note regarding the DLL version:
You must compile with the flag __PLDLL_H__ defined.
This is necessary to select the correct prototypes.
It is unfortunate, but there is - currently - no transparant
way to achieve this.

Some more remarks (running the examples):
- If an example program can not find the PLplot DLL at run-time,
  it may display a message box saying so, or it may simply stop without
  any output. The latter happens if you start it in a DOS-box (at least
  that is the case on my computer, running Windows XP).
- If the example program can not find the font files, it will stop
  after you have selected a device. It will probably display a message
  to that effect, but unless you run it in a DOS-box, the message will
  disappear very quickly.

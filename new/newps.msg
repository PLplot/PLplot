Date: Sun, 12 Apr 92 22:23:05 GMT
From: <amr@egr.duke.edu>
Message-Id: <9204122223.AA10249@ferry.egr.duke.edu>
To: mjl@fusion.ph.utexas.edu
Subject: another bug in ps.c

Dear Maurice,
   The device xmin, xmax, ymin, and ymax need to be initialized
differently depending on the orientation.  The fix is simple and
I've included my fixed version of ps.c below.  A quick glance
in the drivers directory indicates that all of the devices that
support landscape and portrait orientations need to be fixed.

   I'll try to send you a truly conformant EPS driver in a few days.
(The current PostScript driver generates code that is almost EPS
conformant, i.e. can be imported into most desktop publishing
programs,  but a few programs have problems with it.)  It's a
driver I've been using for awhile and it is similar to the next
driver.

Tony



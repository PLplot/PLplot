Date: Wed, 8 Apr 92 17:04:16 GMT
From: <amr@egr.duke.edu>
Message-Id: <9204081704.AA15655@ferry.egr.duke.edu>
To: mjl@fusion.ph.utexas.edu
Subject: Bug in ps.c

This may have already been reported and/or fixed.  Before you call 
plRotPhy() in psline() in ps.c the pls->orient is negated but bit
negation is used instead of logical negation.  To fix change the
line that reads:

  ori = pls->orient; pls->orient = ~pls->orient;

to

  ori = pls->orient; pls->orient = !pls->orient;


Things work fine in landscape mode, but portrait mode is comes out
as landscape without the fix.

Tony

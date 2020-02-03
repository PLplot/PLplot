%% /u/sy/beebe/tex/bibsort/README.AWK, Sat Nov  9 15:35:00 1996
%% Edit by Nelson H. F. Beebe <beebe@plot79.math.utah.edu>

These notes provide some information about awk, in case you are
unfamiliar with it, or want to learn more about it.

I use the awk programming language for implementing many of my
software tools (I have written more than 114,000 lines of awk code as
of [09-Nov-1996]), and I use it in teaching as an example of a little
language that every computer user who does text processing can benefit
from learning.

While awk is an interpreted language which suffers a runtime
performance penalty compared to natively compiled languages such as
Ada, C, C++, Fortran, and Pascal, for many text processing problems it
is almost perfect.  A C implementation of my bibcheck utility ran 3.5
times faster than the awk version, but took 22.4 times as many lines
of code!  And, of course, the awk version was much easier to write,
and required very little debugging.

awk is a POSIX standard, though I don't yet have on hand the POSIX awk
language description.  This means that you can expect your computer
vendor to provide it, and that it should be widely available for a
long time.

awk is a clean simple language, with few blemishes.  This is in stark
contrast to perl, which I find so ugly that I refuse to learn it, even
though I deeply appreciate what it is trying to do.

The official description of awk is found in the book

@String{pub-AW                  = "Ad{\-d}i{\-s}on-Wes{\-l}ey"}
@String{pub-AW:adr              = "Reading, MA, USA"}

@Book{Aho:1987:APL,
  author =       "Alfred V. Aho and Brian W. Kernighan and Peter J.
                 Weinberger",
  title =        "The {AWK} Programming Language",
  publisher =    pub-AW,
  address =      pub-AW:adr,
  pages =        "x + 210",
  year =         "1988",
  ISBN =         "0-201-07981-X",
  LCCN =         "QA76.73.A95 A35 1988",
  bibdate =      "Tue Dec 14 22:33:46 1993",
}

Another book which you may find useful (though I much prefer the above
one) is

@String{pub-ORA                 = "O'Reilly \& {Associates, Inc.}"}
@String{pub-ORA:adr             = "981 Chestnut Street, Newton, MA 02164, USA"}

@Book{Dougherty:SA91,
  author =       "Dale Dougherty",
  title =        "sed {\&} awk",
  publisher =    pub-ORA,
  address =      pub-ORA:adr,
  pages =        "xxii + 394",
  year =         "1991",
  ISBN =         "0-937175-59-5",
  LCCN =         "QA76.76.U84 D69 1991",
}

There is also a recent one (based on the GNU awk implementation) that
I have not yet seen:

@String{pub-SSC                 = "Specialized Systems Consultants"}
@String{pub-SSC:adr             = "P.O. Box 55549, Seattle, WA 98155"}

@Book{Robbins:1996:EAP,
  author =       "Arnold Robbins",
  title =        "Effective {AWK} Programming",
  publisher =    pub-SSC,
  address =      pub-SSC:adr,
  year =         "1996",
  URL =          "http://www.ssc.com/ssc/eap/",
  ISBN =         "0-916151-88-3",
  LCCN =         "",
  acknowledgement = ack-nhfb,
  pages =        "321",
  price =        "US\$27.00",
  bibdate =      "Fri Jun 14 17:24:04 1996",
  libnote =      "Not yet in my library.",
}

Some other publications on, and suppliers of, awk are:

@String{j-SUNEXPERT             = "SunExpert"}
@Article{Collinson:awk,
  author =       "Peter Collinson",
  title =        "Awk",
  journal =      j-SUNEXPERT,
  volume =       "2",
  number =       "1",
  pages =        "33--36",
  month =        jan,
  year =         "1991",
}

@String{pub-FSF                 = "{Free Software Foundation}"}
@String{pub-FSF:adr             = "675 Mass Ave, Cambridge, MA 02139,
                                  USA, Tel: (617) 876-3296"}

@Misc{FSF:gawk,
  key =          "GAWK",
  title =        "The {GAWK} Manual",
  howpublished = pub-FSF # " " # pub-FSF:adr,
  year =         "1987",
  note =         "Also available via ANONYMOUS FTP to
                 \path|prep.ai.mit.edu|. See also \cite{Aho:APL87}.",
}

@Misc{MKS:awk,
  author =       "{Mortice Kern Systems, Inc.}",
  title =        "{MKSAWK}",
  year =         "1987",
  note =         "35 King Street North, Waterloo, Ontario, Canada, Tel:
                 (519) 884-2251. See also \cite{Aho:APL87}.",
}

@Misc{ONW:awk,
  author =       "{OpenNetwork}",
  title =        "{The Berkeley Utilities}",
  year =         "1991",
  note =         "215 Berkeley Place, Brooklyn, NY 11217, USA, Tel:
                 (718) 398-3838.",
  altnote =      "See ad on p. 108 of April 1991 UNIX Review.",
}

@Misc{Polytron:polyawk,
  author =       "Polytron Corporation",
  title =        "{Poly{\-}AWK}",
  year =         "1987",
  note =         "170 NW 167th Place, Beaverton, OR 97006. See also
                 \cite{Aho:APL87}.",
}

@String{j-SPE                   = "Soft{\-}ware\emdash Prac{\-}tice
                                  and Experience"}

@Article{VanWyk:awk,
  author =       "Christopher J. Van Wyk",
  title =        "{AWK} as Glue for Programs",
  journal =      j-SPE,
  volume =       "16",
  number =       "4",
  pages =        "369--388",
  month =        apr,
  year =         "1986",
}

These entries are all taken from

	ftp://ftp.math.utah.edu/pub/tex/bib/index.html#master

which records books in my library, and other selected references; by
the time you read this, there may be more awk-related entries in that
bibliography.

At the time that the Aho, Kernighan, and Weinberger book appeared, awk
was only available in UNIX systems, and it took a few years for UNIX
vendors to incorporate the new, and much enhanced, version of the
language described in the book.  Most UNIX vendors retain the name
`awk' for the old original function-less language from 1978, and call
the 1987 one `nawk' (for `new awk').  An important exception is IBM,
which supplies the new implementation on RS/6000 AIX systems, but
calls it just awk.

Unfortunately, several vendors have not kept up with Brian Kernighan's
further development of awk, with the result that some nawk
implementations lack features that were added after the 1987 book was
published, notably the ENVIRON[] array for access to environment
variables.  Also, some of the vendor implementations have not
incorporated bug fixes which Kernighan introduced.

Fortunately, this situation has improved through three important
developments:

	(1) Arnold Robbin's gawk, the GNU Project implementation of
	awk, available at
		ftp://prep.ai.mit.edu/pub/gnu/gawk-x.yy.tar.gz
	The gawk distribution includes ports for the Amiga, the IBM
	PC, the Atari, and for DEC OpenVMS.

	(2) Brian Kernighan's awk has been released by AT&T Bell Labs,
	and is available at
		http://cm.bell-labs.com/who/bwk/awk.sh

	(3) Mike Brennan's mawk, available at
		ftp://ftp.whidbey.net/pub/brennan/mawkx.y.z.tar.gz

Besides these freely-distributable (for non-commercial purposes, as
detailed in licenses included with their distributions), there are
commercially-supported versions of awk for the IBM PC world and other
machines, recorded in the BibTeX entries above.

Robbins, Kernighan, and Brennan are in contact with one another, so
their implementations support the same features, although gawk has
added a number of (well-documented) extensions that the others have
not yet incorporated.  With a few exceptions, I've tried hard in my
awk programs to stick to the standard language as documented in the
1987 book.

	(1) gawk and recent AT&T awk have the IGNORECASE extension,
	which I only rarely use.  That feature is difficult to
	simulate in a portable awk program.

	(2) gawk and mawk have toupper() and tolower() for efficient
	lettercase conversion; it is possible to implement these in
	awk itself, but only very inefficiently

	(3) gawk, mawk, and recent AT&T awk support the ENVIRON[]
	array for efficient access to environment variables.

	(4) gawk, mawk, and recent AT&T awk support the names
	/dev/stderr and /dev/stdout for the standard UNIX devices
	(which sadly, UNIX never got around to giving names), and gawk
	and AT&T awk support /dev/stdin.  The alternative to
	/dev/stderr is /dev/tty (except it fails if the process is
	running without a controlling terminal, which happens for
	batch jobs, and for background processes), or a horrid
	contortion to invoke the shell and cat.  Since any realistic
	program will require the ability to write error messages, use
	of /dev/stderr is the one feature that is likely to cause
	portability problems.
	
	(5) only gawk is 8-bit clean, and capable of processing all
	256 8-bit byte values, including NUL, and accepting 8-bit
	characters in regexp patterns.  Recent AT&T awk loses NUL
	during processing (because it uses C-style strings internally,
	which reserve NUL for a string terminator), and it rejects
	characters 128..255 in regexp patterns.  mawk gets thoroughly
	confused by NUL in its input stream, and terminates; it
	handles the other 255 byte values (1..255) correctly, at least
	for I/O.

For further discussion of awk implementation differences and language
evolution, see the 
	(gawk.info)Language History 
node in the GNU Emacs info system.

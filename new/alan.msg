Date: Wed, 1 Apr 92 19:09:29 +0900
From: Alan McAllister <alan@flare6.solar.isas.ac.jp>
Message-Id: <9204011009.AA05423@flare6.solar.isas.ac.jp>
To: mjl@solar.ph.utexas.edu

Dear Maurice,

Got plplot home tonight and thought I'd have a go at it. I have made notes
of various warnings etc., though things seemed to go generally O.K. till I
got to 'make'ing the fonts. 

Hi some notes on setting up plplot.

When I tried to run m4 on the makefile with the -DMATA=FFP option I got a
message:

           m4: null deffinition

Next I had to set the s bit with chmod on the sksh scripts before they would 
exicute. Even then when I ran makelink I got a lot of messages:

           makelink: unable to exicute program(24)

though the links seem to have been made?

When running make, compiling the thing, I got a warning:

            warning 84: redeffinition of pre processor [variable?] SIGN

Then when compiling the examples I got various warnings:


     x03: warning 88: incorrect argument, in call to c_plutex

     x06: the same
  
     x07: the same

     x11: warning 93 no reference to identifier "r"
          warning 88 in call to plbox
        
     x14: warning 93
          warning 88 in call to plwind

          there were a bunch of these, and I didn't catch all the
          identifiers(93), and perhaps all the routines(88).

     tutor: warning 85 ? 
            perhaps the plwind call was here.

Trying to make the fonts it compiles all the fon0X.c files but bombs out
trying to link them with:
                          Error 426: Cannot find object -o 
                          w:bin/make: Error code 140

Trying this by hand I get the same result. Typing blink by itself I get the
syntax message 


BLINK [FROM|ROOT] files   [TO file]   [WITH files] [VER file]   [OVLYMGR
file]       
      [LIBRARY|LIB files] [XREF file] [BUFSIZE n]  [NODEBUG|ND] [NOICONS]            
      [SMALLCODE|SC]  [SMALLDATA|SD]  [ONEDATA|OD] [FAST]       [CHIP]               
      [LIBFD name.fd] [LIBPREFIX prefix]           [LIBID "ID string"]               
      [QUIET]         [PRELINK]       [ADDSYM]     [NOALVS]     [MAXHUNK n]          
      [DEFINE symbol=symbol|value]    [VERBOSE]    [FASTER]     [BATCH]              
      [MAP file H,X,S,F,L,O] [PLAIN]  [HWIDTH n]   [HEIGHT n]   [WIDTH n]            
                             [FANCY]  [FWIDTH n]   [PWIDTH n]   [SWIDTH n]           
:                                      

which seems inconsistant with your calling syntax. This is the SAS/C 5.10
blink??

Any idea what gives here? My guess is there is either something wierd or old
about my Lattice (have you gotten another update you forgot to mention?) or 
else there is something wrong with the makefile??


Also I'm puzzeled why x08c is looking for plstnd4.fnt ? This dosn't seem to
be called for in the makefile at all?

Do I have the wrong plplot package? or the wrong makefile? 

Lastly you left a directory /new in here with a grumpy note from Geoff in
it. Not the best thing for final release versions.

I haven't tried in on a Sun yet, though I may pretty soon.

Any help you can give me is much appreciated. 

BTW my e-mail at Todai seems to be back up, it is even working here at the
space institute..connected to the world again..

        Alan

      implicit none
      character*100 command
      integer narg, iargc
c      get number of command-line arguments
      narg = iargc()
c      get command name.  Apparently this does not always give back the
c      command name on all systems, but all that is required for the
c      getarg(0 call by the interface logic is that a valid string is
c      returned.  (All we are really interested in is the subsequent
c      arguments.)
      call getarg(0, command)
      if(narg.gt.0) call getarg(1, command)
c     ran into one case (Cygwin) where under certain badly linked
c     circumstances, iargc() could return -1
      if(narg.lt.0) then
        call exit(1)
      else
        call exit(0)
      endif
      stop
      end

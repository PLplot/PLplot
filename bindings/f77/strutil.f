! $Id$

      subroutine plstrf2c(string1, string2, maxlen)

      integer*4 maxlen
      character*(*) string1, string2

      integer*4 limit, islen
      external islen

      limit = min0(islen(string1),maxlen-1)
      do 100 i = 1,limit
         string2(i:i) = string1(i:i) 
 100  continue
      string2(limit+1:limit+1) = char(0)

      return
      end


      subroutine plstrc2f(string1, string2)

      character*(*) string1, string2

      integer*4 limit
      character*300 stringbuf

      limit = 1
 10   if (ichar(string1(limit:limit)) .eq. 0) goto 20
      stringbuf(limit:limit) = string1(limit:limit)
      limit = limit + 1
      goto 10
	
 20   if(limit.gt.1) then
        string2 = stringbuf(1:limit-1)
      else
        string2 = ''
      endif

      return
      end


      integer function islen(string)
      character*(*) string
      integer i

      do 100 i = len(string),1,-1
         if (string(i:i) .ne. ' ') then
            islen = i
            return
         endif
 100  continue
	   
C      If string is blank or length 0, return length 0
      islen = 0
      return
      end

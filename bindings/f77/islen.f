	integer function islen(string)
	character*(*) string
        integer i

	do i = len(string),1,-1
	   if (string(i:i) .ne. ' ') then
	      islen = i
	      return
	   endif
	enddo
	   
	islen = 1
	return
	end

	subroutine str2int(string, istring, limit)

	character string*(*)
	integer*4 istring(1), limit

	integer i,j,k,ii,nfac,ilim

	do 200 i = 1, limit, 4
	    k = 1 + (i-1)/4
	    istring(k) = 0

	    ii = 1
	    nfac = 1
	    j = i + ii-1
	    istring(k) = istring(k) + ichar(string(j:j))*nfac

	    ilim = min0(limit-i+1, 4)
	    do 100 ii = 2, ilim
		nfac = nfac*256
		j = i + ii-1
		istring(k) = istring(k) + ichar(string(j:j))*nfac
100	    continue
200	continue
	istring(k+1) = 0

	return
	end


      subroutine int2str(istring, string, maxslen)

      character string*(*)
      integer*4 istring(1), maxslen

      integer ii, jj, lstring, mstring, nstring

      i = 0
  100 continue
         i = i+1
         lstring = istring(i)

         do 102 ii = 1, 4
            j = (i-1)*4 + ii

            mstring = lstring / 256
            nstring = lstring - (mstring * 256)

            if(j .eq. maxslen .or. nstring .eq. 0) then
               do 104 jj = j, maxslen
                  string(jj:jj) = ' '
  104          continue
               return 
            endif

            string(j:j) = char(nstring)
            lstring = mstring

  102    continue
      go to 100
      end

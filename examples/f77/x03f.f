*
      program example03
*     =================
*
* Illustration of 1-1 scaling for polar plot
*
      character*3 text
      real x0(0:360), y0(0:360)
      real x(0:360), y(0:360)
*
      dtr = 3.141592654/180.0
      do 1 i=0,360
        x0(i) = cos(dtr * real(i))
        y0(i) = sin(dtr * real(i))
    1 continue
*
* Ask user to specify the output device.
* Note that for this demo, neither the global aspect ratio flag nore
* the global orientation flag give the desired results, since plenv is told
* to do scaling based on physical dimensions.
* Thus we MUST print to a physical device and not globally mess with
* orientation or aspect ratio (this may be improved in the future). 
*
      call plinit()
*
* Set up viewport and window, but do not draw box
*
      call plenv(-1.3,1.3,-1.3,1.3,1,-2)
      do 2 i = 1,10
        do 3 j = 0,360
          x(j) = 0.1*i*x0(j)
          y(j) = 0.1*i*y0(j)
    3   continue
*
* Draw circles for polar grid
*
        call plline(361,x,y)
    2 continue
*
      call plcol(2)
      do 4 i = 0,11
        theta = 30.0*i
        dx = cos(dtr*theta)
        dy = sin(dtr*theta)
*
* Draw radial spokes for polar grid
*
        call pljoin(0.0,0.0,dx,dy)
        write (text,'(i3)') nint(theta)
*
* Write labels for angle
*
        text = text(nsp(text):)
        if (dx.ge.0) then
          call plptex(dx,dy,dx,dy,-0.15,text)
        else
          call plptex(dx,dy,-dx,-dy,1.15,text)
        end if
    4 continue
*
* Draw the graph
*
      do 5 i=0,360
        r = sin(dtr*real(5*i))
        x(i) = x0(i) * r
        y(i) = y0(i) * r
    5 continue
      call plcol(3)
      call plline(361,x,y)
*
      call plcol(4)
      call plmtex
     *   ('t',2.0,0.5,0.5,'#frPLPLOT Example 3 - r(#gh)=sin 5#gh')
*
* Close the plot at end
*
      call plend
      end
*
      function nsp(text)
*     ==================
*
* Find first non-space character
*
      character*(*) text            
*
      l = len(text)
      nsp = 1
    1 if (text(nsp:nsp).eq.' ' .and. nsp.lt.l) then
        nsp = nsp+1
        goto 1
      end if
      end

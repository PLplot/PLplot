*
      program example02
*     =================
*
* Demonstration of colours
*
      character*2 text
*
* Divide screen into 16 regions, one for each colour
*
      call plssub(4,4)
      call plinit()
      call plschr(0.0,3.5)
      call plfont(4)
*
      do 1 i=0,15
        write (text,'(i2)') i
        if (i .lt. 10) text=text(2:2)
        call pladv(0)
        call plcol(i)
        vmin = 0.1
        vmax = 0.9
        do 2 j=1,3
          call plvpor(vmin,vmax,vmin,vmax)
          call plwind(0.0,1.0,0.0,1.0)
          call plbox('bc',0.0,0,'bc',0.0,0)
          vmin = vmin + 0.1
          vmax = vmax - 0.1
    2   continue
        call plptex(0.5,0.5,1.0,0.0,0.5,text)
    1 continue
*
      call plend
      end

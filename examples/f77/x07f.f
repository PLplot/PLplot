*
      program example07
*     =================
*
* Displays the plotter symbols for PLSYM
*
      character*4 text
*
      integer base(17)
      data base /   0, 200, 500, 600, 700, 800, 900,
     *           2000,2100,2200,2300,2400,2500,2600,
     *           2700,2800,2900 /
*
* Full sized page for display
*
      call plinit()
      call plfontld(1)
      do 100 l=1,17
        call pladv(0)
*
        call plcol(2)
*
* Set up viewport and window
*
        call plvpor(0.1,1.0,0.1,0.9)
        call plwind(0.0,1.0,0.0,1.0)
*
* Draw the grid using plbox
*
        call plbox('bcgt',0.1,0,'bcgt',0.1,0)
        call plcol(15)
*
* Write the digits below the frame
*
        do 5 i=0,9
          write (text,'(i1)') i
          call plmtex('b',1.5,0.1*i+0.05,0.5,text)
    5   continue
*    
        k=0
        do 1 i=0,9
*        
* Write the digits to the left of the frame
*
          write (text,'(i4)') base(l)+10*i
          call plmtex('lv',1.0,0.95-0.1*i,1.0,text)
          do 2 j=0,9
            x=0.1*j+0.05
            y=0.95-0.1*i
*
* Display the symbols
*
            call plsym(1,x,y,base(l)+k)
            k=k+1
    2     continue
    1   continue
*
        call plmtex('t',1.5,0.5,0.5,'PLPLOT Example 7 - PLSYM symbols')
  100 continue
      call plend
      end

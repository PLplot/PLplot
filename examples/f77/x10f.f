*
      program example10
*     =================
*
* Demonstration program for PLPLOT illustrating absolute positioning
*  of graphs on a page
*
      call plinit()
*
      call pladv(0)
      call plvpor(0.0,1.0,0.0,1.0)
      call plwind(0.0,1.0,0.0,1.0)
      call plbox('bc',0.0,0,'bc',0.0,0)
*
      call plsvpa(50.0,150.0,100.0,150.0)
      call plwind(0.0,1.0,0.0,1.0)
      call plbox('bc',0.0,0,'bc',0.0,0)
      call plptex(0.5,0.5,1.0,0.0,0.5,'BOX at (50,150,100,150)')
*
      call plend
*
      end

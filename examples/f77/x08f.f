*
      program example08
*     =================
*
* Demonstration of 3-d plotting      
*      
      integer i,j,opt(4),npts
      parameter (npts=41)
*
      real x(npts), y(npts), z(npts,npts)
*
      character*80 title(4)
      real alt(4),az(4)
      data opt /1,   2,   3,   3/
      data alt /60.0,30.0,60.0,60.0/
      data az  /30.0,60.0,120.0,160.0/
      data title /'#frPLPLOT Example 8 - Alt=60, Az=30, Opt=1',
     *            '#frPLPLOT Example 8 - Alt=30, Az=60, Opt=2',
     *            '#frPLPLOT Example 8 - Alt=60, Az=120, Opt=3',
     *            '#frPLPLOT Example 8 - Alt=60, Az=160, Opt=3'/
*
      do 1 i=1,npts
        x(i) = (i-1-(npts/2))/real(npts/2)
        y(i) = (i-1-(npts/2))/real(npts/2)
    1 continue
*
      do 2 i=1,npts
        xx = x(i)
        do 3 j=1,npts
          yy = y(j)
          r = sqrt(xx*xx + yy*yy)
          z(i,j) = exp(-r*r) * cos(2.0*3.141592654*r)
    3   continue
    2 continue
*
      call plinit()
      do 5 k=1,4
         call pladv(0)
         call plcol(1)
         call plvpor(0.0,1.0,0.0,0.9)
         call plwind(-1.0,1.0,-0.9,1.1)
*
         call plw3d(1.0,1.0,1.0,-1.0,1.0,-1.0,1.0,-1.0,1.0,alt(k),az(k))
         call plbox3('bnstu','x axis',0.0,0,'bnstu','y axis',0.0,0,
     *               'bcdmnstuv','z axis',0.0,0)
         call plcol(2)
         call plot3d(x,y,z,npts,npts,opt(k),1,npts)
         call plcol(3)
         call plmtex('t',1.0,0.5,0.5,title(k))
*     
    5 continue
      call plend
*
      end

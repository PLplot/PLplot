*     Demonstration of mesh plotting 

      parameter (NPTS=41)
      integer opt(4)
      real alt(4), az(4)
      real x(NPTS), y(NPTS), z(NPTS,NPTS)
      character*80 title(4)

      data opt /1,   2,   3,   3/
      data alt /60.0,20.0,60.0,60.0/
      data az /30.0,60.0,120.0,160.0/

      data title /"#frPLPLOT Example 11 - Alt=60, Az=30, Opt=1",
     *            "#frPLPLOT Example 11 - Alt=20, Az=60, Opt=2",
     *            "#frPLPLOT Example 11 - Alt=60, Az=120, Opt=3",
     *            "#frPLPLOT Example 11 - Alt=60, Az=160, Opt=3"/

      do 1 i=1,NPTS
         x(i) = (i-1-(NPTS/2))/float(NPTS/2)
         y(i) = (i-1-(NPTS/2))/float(NPTS/2)
1     continue

      do 2 i=1,NPTS
         xx = x(i)
         do 3 j=1,NPTS
            yy = y(j)
            r = sqrt(xx*xx + yy*yy)
            z(i,j) = exp(-r*r) * cos(2.0*3.141592654*r)
3        continue
2     continue

      call plinit()

      do 4 k=1,4
         call pladv(0)
         call plcol(1)
         call plvpor(0.0,1.0,0.0,0.9)
         call plwind(-1.0,1.0,-0.9,1.1)

         call plw3d(1.0,1.0,1.0,-1.0,1.0,-1.0,1.0,-1.0,1.0,alt(k),az(k))
         call plbox3("bnstu","x axis",0.0,0,"bnstu","y axis",0.0,0,
     *               "bcdmnstuv","z axis",0.0,0)
         call plcol(2)
         call plmesh(x,y,z,NPTS,NPTS,opt(k),NPTS)
         call plcol(3)
         call plmtex("t",1.0,0.5,0.5,title(k))
4     continue

      call plend

      end

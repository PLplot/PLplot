*     Demonstration program for PLPLOT: */
      parameter (PI=3.14156)
      real just, x(500), y(500), per(5)
      character*20 text(5)
      data text / "Maurice", "Randy", "Mark", "Steve", "Warner"/

      per(1) = 10.
      per(2) = 32.
      per(3) = 12.
      per(4) = 30.
      per(5) = 16.
*
* Ask user to specify the output device.
* Note that for this demo, neither the global aspect ratio flag nore
* the global orientation flag give the desired results, since plenv is told
* to do scaling based on physical dimensions.
* Thus we MUST print to a physical device and not globally mess with
* orientation or aspect ratio (this may be improved in the future). 
*
      call plinit()

      call plenv(0.,10.,0.,10.,1,-2)
      call plcol(2)

      theta0 = 0.
      dthet = 2*PI/500

      do 1 i=1,5
         x(1) = 5.
         y(1) = 5.
         j = 1
         theta1 = theta0 + 2*PI*per(i)/100.
         if(i.eq.5) theta1 = 2*PI
         do 2 theta=theta0,theta1,dthet
            j = j + 1
            x(j) = 5 + 3*cos(theta)
            y(j) = 5 + 3*sin(theta)
2        continue
         call plcol(i)
         call plpsty(mod(i+3,8)+1)
         call plfill(j,x,y)
         call plcol(1)
         call plline(j,x,y)
         just = (theta0+theta1)/2.
         dx = .25*cos(just)
         dy = .25*sin(just)
         if(just .lt. PI/2 .or. just .gt. 3*PI/2) then
            just = 0.
         else 
            just = 1.
         endif
         call plptex(x(j/2)+dx,y(j/2)+dy,1.,0.,just,text(i))
         theta0 = theta-dthet
1     continue
      call plfont(2)
      call plschr(0.,1.3)
      call plptex(5.,9.,1.,0.,.5,"Percentage of Sales")

      call plend

      end

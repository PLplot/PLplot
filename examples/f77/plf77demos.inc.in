      function myisnan(x)

      implicit none
      logical myisnan
      real*8 x

@HAVE_F77_ISNAN_FALSE@      myisnan = (x.ne.x)
@HAVE_F77_ISNAN_TRUE@      myisnan = isnan(x)

      return

      end function

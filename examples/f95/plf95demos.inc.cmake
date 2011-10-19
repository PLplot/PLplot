      function myisnan(x)

      use plplot_flt
      implicit none
      logical :: myisnan
      real (kind=plflt) :: x

@HAVE_F77_ISNAN_FALSE@      myisnan = (x.ne.x)
@HAVE_F77_ISNAN_TRUE@      myisnan = isnan(x)

      return

      end function

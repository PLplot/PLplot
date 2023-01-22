      program testisnan

      real dummy

c      external isnan
      dummy = 0.0

      if (isnan(0.0/dummy)) then
        print *,"NaN"
      endif

      end program testisnan

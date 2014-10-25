! test_plf95demolib.f90 --
!     Test program for the auxiliary functions
!
program test_plf95demolib
    use plf95demolib

    real, dimension(10)   :: value
    integer               :: i
    integer, dimension(4) :: start = (/  0,  0,  1,  2 /), &
                             stop  = (/ 10, 10, 11, 22 /), &
                             step  = (/  1,  2,  1,  2 /), &
                             expected_size = &
                                     (/ 10,  5, 10, 10 /)
    integer               :: sz

    ! Function arange:
    ! - Check the length of the returned array
    ! - Check the values
    !
    do i = 1,size(start)
        sz = size( arange( start(i), stop(i), step(i) ) )
        if ( sz /= expected_size(i) ) then
            write(*,*) 'Expected:', expected_size(i), ' - got: ', sz
        else
            value(1:sz) = arange( start(i), stop(i), step(i) )
            write(*,*) value(1:sz)
            if ( any( value(1:sz) < start(i) ) ) then
                write(*,*) 'Minimum value too low: ', minval(value(1:sz)), ' - expected: ', start(i)
            endif
            if ( any( value(1:sz) >= stop(i) ) ) then
                write(*,*) 'Maximum value too high: ', maxval(value(1:sz)), ' - should be below: ', stop(i)
            endif
            if ( any( value(2:sz) - value(1:sz-1) /= step(i) ) ) then
                write(*,*) 'Difference in subsequent values incorrect: ', value(1:sz), ' - expected difference:', step(i)
            endif
        endif
    enddo
end program

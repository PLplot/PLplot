! test_plf95demolib.f90 --
!     Test program for the auxiliary functions
!
program test_plf95demolib
    use plf95demolib

    real, dimension(10)   :: value
    integer(kind=plint)               :: i
    integer(kind=plint), dimension(4) :: start = (/  0_plint,  0_plint,  1_plint,  2_plint /), &
                             stop  = (/ 10_plint, 10_plint, 11_plint, 22_plint /), &
                             step  = (/  1_plint,  2_plint,  1_plint,  2_plint /), &
                             expected_size = &
                                     (/ 10_plint,  5_plint, 10_plint, 10_plint /)
    integer(kind=plint)               :: sz

    ! Function arange:
    ! - Check the length of the returned array
    ! - Check the values
    !
    do i = 1_plint,size(start,kind=plint)
        sz = size( arange( start(i), stop(i), step(i) ),kind=plint )
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

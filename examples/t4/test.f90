program hello
    implicit none

    interface
        subroutine say_hello_from_c() bind(C)
        end subroutine say_hello_from_c
    end interface

    print *, "Hello from Fortran!"
    call say_hello_from_c()
end program hello
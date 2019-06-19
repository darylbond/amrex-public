module amrex_multifab_utility
  use amrex_fort_module, only : rt=>amrex_real
  implicit none
  private

  public :: save_FAB

  contains

  subroutine save_FAB(u, ulo, uhi, ncomp, comp, name, nchar)  bind(c,name='save_FAB')
    use amrex_fort_module, only : amrex_real
    USE iso_c_binding, only: C_CHAR, C_INT
    integer, intent(in) :: ulo(3), uhi(3), ncomp
    real(amrex_real), intent(in   ) :: u ( ulo(1): uhi(1), ulo(2): uhi(2), ulo(3): uhi(3), 0:ncomp-1)
    integer(c_int), intent(in) :: comp, nchar
    character(kind = c_char) :: name(nchar)

    character(len=nchar) :: fname

    integer i,j,k

    do i = 1,nchar
      fname(i:i) = name(i)
    enddo

    open(1, file=fname)
    write(1,*) ulo(1),",",ulo(2),",",ulo(3)
    write(1,*) uhi(1),",",uhi(2),",",uhi(3)
    do       k = ulo(3),uhi(3)
       do    j = ulo(2),uhi(2)
          do i = ulo(1),uhi(1)
            write(1,*) i,",",j,",",k,",",u(i,j,k,comp)
          enddo
      enddo
    enddo

  end subroutine save_FAB

end module amrex_multifab_utility

c
c     sample driver for qr
c
      program qrtest
      real*8 a(5,5),work(5),z(5,5),wr(5),wi(5)
c
      integer iwork(5)
      nm = 5
c
c     Case 1 : real eigenvalues.
c     ------
      n  = 3
c
      a(1,1) = 2.0d0
      a(1,2) = 2.0d0
      a(1,3) = 2.0d0
c
      a(2,1) = 0.0d0
      a(2,2) = 3.0d0
      a(2,3) = 1.0d0
c
      a(3,1) = 1.0d0
      a(3,2) = -2.0d0
      a(3,3) = 1.0d0
c
      write (*,*)
      write (*,*) 'test for qr : case 1 : real eigenvectors'
      call qr (nm, n, a, work, wr, wi, z, iwork, ierr)
      if (ierr .ne. 0) stop
c
      write (*,*) 'eigenvalues'
      write (*,70) (wr(i), wi(i), i = 1, n)
   70 format (1x,3('(',f6.2,',',f6.2,')',2x))
c
      write(*,*) 'eigenvectors'
      j = 1
   90 continue
      if (wi(j) .eq. 0.0) then
        write (*,70) (z(i,j), wi(j), i = 1, n)
      else
        write (*,70) (z(i,j), z(i,j+1), i = 1, n)
      end if
      if (wi(j) .ne. 0.0) then
        write (*,70) (z(i,j), -z(i,j+1), i = 1, n)
        j = j + 2
      else
        j = j + 1
      end if
      if (j .le. n) go to 90
      write(*,*) 'correct eigenvalues 1.0, 3.0 & 2.0'
c
c     Case 2 : complex eigenvalues.
c     ------
      n  = 4
c
      a(1,1) = 4.0d0
      a(1,2) = -5.0d0
      a(1,3) = 0.0d0
      a(1,4) = 3.0d0
c
      a(2,1) = 0.0d0
      a(2,2) = 4.0d0
      a(2,3) = -3.0d0
      a(2,4) = -5.0d0
c
      a(3,1) = 5.0d0
      a(3,2) = -3.0d0
      a(3,3) = 4.0d0
      a(3,4) = 0.0d0
c
      a(4,1) = 3.0d0
      a(4,2) = 0.0d0
      a(4,3) = 5.0d0
      a(4,4) = 4.0d0
c
      write (*,*)
      write (*,*) 'test for qr : case 2 : complex eigenvectors'
      call qr (nm, n, a, work, wr, wi, z, iwork, ierr)
      if (ierr .ne. 0) stop
c
      write (*,*) 'eigenvalues'
      write (*,170) (wr(i), wi(i), i = 1, n)
  170 format (1x,4('(',f6.2,',',f6.2,')',2x))
c
      write(*,*) 'eigenvectors'
      j = 1
  190 continue
      if (wi(j) .eq. 0.0) then
        write (*,170) (z(i,j), wi(j), i = 1, n)
      else
        write (*,170) (z(i,j), z(i,j+1), i = 1, n)
      end if
      if (wi(j) .ne. 0.0) then
        write (*,170) (z(i,j), -z(i,j+1), i = 1, n)
        j = j + 2
      else
        j = j + 1
      end if
      if (j .le. n) go to 190
      write (*,*) 'correct eigenvalues 12.0, 1.0+-5i & 2.0'
c
c     Case 3 : coincident eigenvalues.
c     ------
      n  = 4
c
      a(1,1) = 6.0d0
      a(1,2) = 4.0d0
      a(1,3) = 4.0d0
      a(1,4) = 1.0d0
c
      a(2,1) = 4.0d0
      a(2,2) = 6.0d0
      a(2,3) = 1.0d0
      a(2,4) = 4.0d0
c
      a(3,1) = 4.0d0
      a(3,2) = 1.0d0
      a(3,3) = 6.0d0
      a(3,4) = 4.0d0
c
      a(4,1) = 1.0d0
      a(4,2) = 4.0d0
      a(4,3) = 4.0d0
      a(4,4) = 6.0d0
c
      write (*,*)
      write (*,*) 'test for qr : case 3 : coincident eigenvectors'
      call qr (nm, n, a, work, wr, wi, z, iwork, ierr)
      if (ierr .ne. 0) stop
c
      write (*,*) 'eigenvalues'
      write (*,270) (wr(i), wi(i), i = 1, n)
  270 format (1x,4('(',f6.2,',',f6.2,')',2x))
c
      write(*,*) 'eigenvectors'
      j = 1
  290 continue
      if (wi(j) .eq. 0.0) then
        write (*,270) (z(i,j), wi(j), i = 1, n)
      else
        write (*,270) (z(i,j), z(i,j+1), i = 1, n)
      end if
      if (wi(j) .ne. 0.0) then
        write (*,270) (z(i,j), -z(i,j+1), i = 1, n)
        j = j + 2
      else
        j = j + 1
      end if
      if (j .le. n) go to 290
      write (*,*) 'correct eigenvalues 15.0, 5.0, 5.0 & -1.0'
c
      end

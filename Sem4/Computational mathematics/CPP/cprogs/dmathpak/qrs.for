c     QRS.FOR
c     QR eigenvalue solver in FORTRAN 77
c
      subroutine qr (nm, n, a, work, wr, wi, z, iwork, ierr)
c
c   Purpose ...
c   -------
c   Computes eigenvalues and eigenvectors of real
c   general matrix by the qr method. The program is
c   taken from :
c   B.T.Smith, J.M.Boyle, B.S.Garbow, Y.Ikebe, V.C.Klema,
c   C.B.Moler "Matrix eigensystem routines - eispack
c   guide"  Lecture notes in computer science,vol 6,
c   Springer-Verlag,Berlin (1974).
c
c   Converted to FORTRAN 77 by
c   P. Jacobs   Department of Mechanical Engineering
c               University of Queensland
c   It now needs complex division routine cdivsn().
c
c   Input...
c   -----
c   nm   : declared dimension of arrays.
c   n    : order of system.
c   a    : matrix to be analysed.
c   work,: work space.  Arrays must be dimensioned nm in calling
c   iwork  program.  No other input required.
c
c   Output...
c   ------
c   wr   : vector containing real parts of eigenvalues of a.
c   wi   : vector containing imaginary parts of eigenvalues.
c   z    : if wi(j) is 0.0 (real eigenvalue),then z(i,j)
c          contains corresponding eigenvector.
c          If wi(j) is not 0.0 (complex eigenvalue) then z(i,j)
c          and z(i,j+1) contain real and imaginary parts of
c          eigenvector corresponding to eigenvalue with positive
c          imaginary part.
c          The conjugate of this vector corresponds to the conjugate
c          of this eigenvalue,but is not listed.
c   ierr : error flag.
c          = 0 ,normal return.
c          > 0 ,more than 30 iterations required to determine
c               an eigenvalue.  The eigenvalues in wr ,wi are
c               correct for ierr+1,ierr+2,..,n, but no
c               vectors are computed.
c
c   Version ... 1.0, July 1989
c   -------
c
      integer nm, n, iwork(n), ierr, low, igh
      real*8 a(nm,n), work(n), wr(n), wi(n), z(nm,n)
c
      call balanc (nm, n, a, low, igh, work)
      call elmhes (nm, n, low, igh, a, iwork)
      call eltran (nm, n, low, igh, a, iwork, z)
      call hqr2 (nm, n, low, igh, a, wr, wi, z, ierr)
c
      if (ierr .eq. 0) then
         call balbak (nm, n, low, igh, work, n, z)
      endif
c
      return
      end
c
c-----------------------------------------------------------------------
c
      subroutine balanc (nm, n, a, low, igh, scale)
c
      integer i, j, k, L, m, n, jj, nm, igh, low
      real*8 a(nm,n), scale(n)
      real*8 c, f, g, r, s, b2, radix
      logical noconv
c
c     radix is a machine dependent parameter specifying
c     the base of the machine floating point representation.
c
      radix = 2.0
      b2 = radix * radix
c
      k = 1
      L = n
      go to 100
c
c     Search for rows isolating an eigenvalue and push them down
c
   80 continue
      if (L .eq. 1) go to 280
      L = L - 1
c
  100 continue
c
c     for j = L step -1 until 1 do
      do 120 jj = 1, L
         j = L + 1 - jj
c
         do 110 i = 1, L
            if (i .ne. j) then
               if (a(j,i) .ne. 0.0) go to 120
            endif
  110       continue
c
         m = L
c
c        In-line procedure for row and column exchange
         scale(m) = dble(j)
         if (j .ne. m) then
            do 30 i = 1, L
               f = a(i,j)
               a(i,j) = a(i,m)
               a(i,m) = f
   30          continue
            do 40 i = k, n
               f = a(j,i)
               a(j,i) = a(m,i)
               a(m,i) = f
   40          continue
         endif
c
         go to 80
  120    continue
c
      go to 140
c
  130 continue
c
c     Search for columns isolating an eigenvalue and push them left
      k = k + 1
c
  140 continue
      do 170 j = k, L
         do 150 i = k, L
            if (i .ne. j) then
               if (a(i,j) .ne. 0.0) go to 170
            endif
  150       continue
         m = k
c
c        In-line procedure for row and column exchange
         scale(m) = dble(j)
         if (j .ne. m) then
            do 31 i = 1, L
               f = a(i,j)
               a(i,j) = a(i,m)
               a(i,m) = f
   31          continue
            do 41 i = k, n
               f = a(j,i)
               a(j,i) = a(m,i)
               a(m,i) = f
   41          continue
         endif
c
         go to 130
  170    continue
c
c     Now balance the submatrix in rows k to L
      do 180 i = k, L
         scale(i) = 1.0d0
  180    continue
c
c     Iterative loop for norm reduction.
  190 continue
c
      noconv = .false.
c
      do 270 i = k, L
         c = 0.0
         r = 0.0
         do 200 j = k, L
            if (j .ne. i) then
               c = c + abs(a(j,i))
               r = r + abs(a(i,j))
            endif
  200       continue
c
         g = r / radix
         f = 1.0
         s = c + r
  210    if (c .lt. g) then
            f = f * radix
            c = c * b2
            go to 210
         endif
         g = r * radix
  230    if (c .ge. g) then
            f = f / radix
            c = c / b2
            go to 230
         endif
c
c        Now balance
         if ((c + r) / f .lt. 0.95d0 * s) then
            g = 1.0d0 / f
            scale(i) = scale(i) * f
            noconv = .true.
            do 250 j = k, n
               a(i,j) = a(i,j) * g
  250          continue
            do 260 j = 1, L
               a(j,i) = a(j,i) * f
  260          continue
         endif
c
  270    continue
c
      if (noconv) go to 190
c
  280 continue
      low = k
      igh = L
      return
      end
c
c---------------------------------------------------------------------
c
      subroutine elmhes (nm, n, low, igh, a, int)
c
      parameter (zero = 0.0d0)
c
      integer i, j, m, n, la, nm, igh, kp1, low, mm1, mp1
      real*8 a(nm,n), x, y
      integer int(igh)
c
      la = igh - 1
      kp1 = low + 1
      if (la .lt. kp1) go to 200
c
      do 180 m = kp1, la
         mm1 = m - 1
         x = zero
         i = m
c
         do 100 j = m, igh
            if (abs(a(j,mm1)) .gt. abs(x)) then
               x = a(j,mm1)
               i = j
            endif
  100       continue
c
         int(m) = i
         if (i .ne. m) then
c           Interchange for rows and columns of a
            do 110 j = mm1, n
               y = a(i,j)
               a(i,j) = a(m,j)
               a(m,j) = y
  110          continue
c
            do 120 j = 1, igh
               y = a(j,i)
               a(j,i) = a(j,m)
               a(j,m) = y
  120          continue
         endif
c
  130    continue
         if (x .ne. zero) then
            mp1 = m + 1
            do 160 i = mp1, igh
               y = a(i,mm1)
               if (y .ne. zero) then
                  y = y / x
                  a(i,mm1) = y
                  do 140 j = m, n
                     a(i,j) = a(i,j) - y * a(m,j)
  140                continue
                  do 150 j = 1, igh
                     a(j,m) = a(j,m) + y * a(j,i)
  150                continue
                endif
  160           continue
         endif
c
  180    continue
c
  200 return
      end
c
c---------------------------------------------------------------------
c
      subroutine eltran (nm, n, low, igh, a, int, z)
c
      parameter (zero = 0.0d0, one = 1.0d0)

      integer i, j, n, kl, mm, mp, nm, igh, low, mp1
      real*8 a(nm,igh), z(nm,n)
      integer int(igh)
c
c     Initialize z to identity matrix
      do 80 i = 1, n
         do 60 j = 1, n
            z(i,j) = zero
   60       continue
         z(i,i) = one
   80    continue
c
      kl = igh - low - 1
      if (kl .lt. 1) go to 200
c
c     for mp=igh-1 step -1 until low+1 do ...
      do 140 mm = 1, kl
         mp = igh - mm
         mp1 = mp + 1
c
         do 100 i = mp1, igh
            z(i,mp) = a(i,mp-1)
  100       continue
c
         i = int(mp)
         if (i .ne. mp) then
            do 130 j = mp, igh
               z(mp,j) = z(i,j)
               z(i,j) = zero
  130          continue
            z(i,mp) = one
         endif
c
  140    continue
c
  200 return
      end
c
c--------------------------------------------------------------------
c
      subroutine hqr2 (nm, n, Low, igh, h, wr, wi, z, ierr)
c
      parameter (zero = 0.0d0, one = 1.0d0, two = 2.0d0)
c
      integer i, j, k, L, m, n, en, ii, jj, LL, mm, na, nm, nn,
     &        igh, its, Low, mp2, enm2, ierr
      real*8 h(nm,n), wr(n), wi(n), z(nm,n)
      real*8 p, q, r, s, t, w, x, y, ra, sa, vi, vr, zz, norm, machep
      logical notlas
*     complex*16 z3
*     real*8 t3(2)
*     equivalence (z3,t3(1))
      real*8 t3r, t3i
c
c     machep is a machine dependent parameter specifying
c     the relative precision of floating point arithmetic.
c
      machep = 1.0d-16
c
      ierr = 0
c
c     store roots isolated by balanc
      do 50 i = 1, n
         if (i .lt. Low .or. i .gt. igh) then
            wr(i) = h(i,i)
            wi(i) = zero
         endif
   50    continue
c
      en = igh
      t = zero
c
c     Search for next eigenvalue
   60 continue
      if (en .lt. Low) go to 340
      its = 0
      na = en - 1
      enm2 = na - 1
c
c     Look for single small sub-diagonal element
c
c     for L=en step -1 until Low do ...
   70 continue
      do 80 LL = Low, en
         L = en + Low - LL
         if (L .eq. Low) go to 100
         if (abs(h(L,L-1)) .le. machep * (abs(h(L-1,L-1))
     &      + abs(h(L,L)))) go to 100
   80    continue
c
c     Form shift
c
  100 continue
      x = h(en,en)
      if (L .eq. en) go to 270
      y = h(na,na)
      w = h(en,na) * h(na,en)
      if (L .eq. na) go to 280
      if (its .eq. 30) then
c        Set error -- no convergence to an
c        eigenvaLue after 30 iterations
         ierr = en
         goto 1001
      endif
c
      if (its .eq. 10 .or. its .eq. 20) then
c        Form exceptional shift
         t = t + x
         do 120 i = Low, en
            h(i,i) = h(i,i) - x
  120       continue
         s = abs(h(en,na)) + abs(h(na,enm2))
         x = 0.75d0 * s
         y = x
         w = -0.4375d0 * s * s
      endif
c
      its = its + 1
c
c     Look for two consecutive small sub-diagonal elements.
c
c     for m=en-2 step -1 untiL L do ...
      do 140 mm = L, enm2
         m = enm2 + L - mm
         zz = h(m,m)
         r = x - zz
         s = y - zz
         p = (r * s - w) / h(m+1,m) + h(m,m+1)
         q = h(m+1,m+1) - zz - r - s
         r = h(m+2,m+1)
         s = abs(p) + abs(q) + abs(r)
         p = p / s
         q = q / s
         r = r / s
         if (m .eq. L) go to 150
         if (abs(h(m,m-1)) * (abs(q) + abs(r)) .le. machep * abs(p)
     &      * (abs(h(m-1,m-1)) + abs(zz) + abs(h(m+1,m+1)))) go to 150
  140    continue
c
  150 continue
      mp2 = m + 2
c
      do 160 i = mp2, en
         h(i,i-2) = zero
         if (i .ne. mp2) h(i,i-3) = zero
  160    continue
c
c     double qr step involving rows L to en and
c     coLumns m to en
      do 260 k = m, na
         notlas = k .ne. na
         if (k .ne. m) then
            p = h(k,k-1)
            q = h(k+1,k-1)
            r = zero
            if (notlas) r = h(k+2,k-1)
            x = abs(p) + abs(q) + abs(r)
            if (x .eq. zero) go to 260
            p = p / x
            q = q / x
            r = r / x
         endif
         s = sign(sqrt(p*p+q*q+r*r),p)
         if (k .eq. m) then
            if (L .ne. m) h(k,k-1) = -h(k,k-1)
         else
            h(k,k-1) = -s * x
         endif
         p = p + s
         x = p / s
         y = q / s
         zz = r / s
         q = q / p
         r = r / p
c        Row modification
         do 210 j = k, n
            p = h(k,j) + q * h(k+1,j)
            if (notlas) then
               p = p + r * h(k+2,j)
               h(k+2,j) = h(k+2,j) - p * zz
            endif
            h(k+1,j) = h(k+1,j) - p * y
            h(k,j) = h(k,j) - p * x
  210       continue
c
         j = min(en,k+3)
c
c        Column modification
         do 230 i = 1, j
            p = x * h(i,k) + y * h(i,k+1)
            if (notlas) then
               p = p + zz * h(i,k+2)
               h(i,k+2) = h(i,k+2) - p * r
            endif
            h(i,k+1) = h(i,k+1) - p * q
            h(i,k) = h(i,k) - p
  230       continue
c
c        Accumulate transformations
         do 250 i = Low, igh
            p = x * z(i,k) + y * z(i,k+1)
            if (notlas) then
               p = p + zz * z(i,k+2)
               z(i,k+2) = z(i,k+2) - p * r
            endif
            z(i,k+1) = z(i,k+1) - p * q
            z(i,k) = z(i,k) - p
  250       continue
c
  260    continue
c
      go to 70
c
c     One root found.
  270 continue
      h(en,en) = x + t
      wr(en) = h(en,en)
      wi(en) = zero
      en = na
      go to 60
c
c     Two roots found.
  280 continue
      p = (y - x) / two
      q = p * p + w
      zz = sqrt(abs(q))
      h(en,en) = x + t
      x = h(en,en)
      h(na,na) = y + t
c
      if (q .ge. zero) then
c
c        Real pair.
         zz = p + sign(zz,p)
         wr(na) = x + zz
         wr(en) = wr(na)
         if (zz .ne. zero) wr(en) = x - w / zz
         wi(na) = zero
         wi(en) = zero
         x = h(en,na)
         r = sqrt(x * x + zz * zz)
         p = x / r
         q = zz / r
c
c        Row modification.
         do 290 j = na, n
            zz = h(na,j)
            h(na,j) = q * zz + p * h(en,j)
            h(en,j) = q * h(en,j) - p * zz
  290       continue
c
c        Column modification.
         do 300 i = 1, en
            zz = h(i,na)
            h(i,na) = q * zz + p * h(i,en)
            h(i,en) = q * h(i,en) - p * zz
  300       continue
c
c        Accumulate transformations.
         do 310 i = Low, igh
            zz = z(i,na)
            z(i,na) = q * zz + p * z(i,en)
            z(i,en) = q * z(i,en) - p * zz
  310       continue
c
      else
c
c        Complex pair
         wr(na) = x + p
         wr(en) = x + p
         wi(na) = zz
         wi(en) = -zz
      endif
c
      en = enm2
      go to 60
c
c     All roots found.  backsubstitute to find
c     vectors of upper triangular form
  340 continue
      norm = zero
      k = 1
c
      do 360 i = 1, n
         do 350 j = k, n
            norm = norm + abs(h(i,j))
  350       continue
         k = i
  360    continue
c
      if (norm .eq. zero) go to 1001
c
c     for en=n step -1 untiL 1 do ...
      do 800 nn = 1, n
         en = n + 1 - nn
         p = wr(en)
         q = wi(en)
         na = en - 1
c
         if (q .eq. zero) then
c
c           Real vector.
            m = en
            h(en,en) = 1.0
            if (na .eq. 0) go to 800
c           for i=en-1 step -1 untiL 1 do ...
            do 700 ii = 1, na
               i = en - ii
               w = h(i,i) - p
               r = h(i,en)
               if (m .le. na) then
                  do 610 j = m, na
                     r = r + h(i,j) * h(j,en)
  610                continue
               endif
c
               if (wi(i) .lt. zero) then
                  zz = w
                  s = r
                  go to 700
               endif
c
               m = i
               if (wi(i) .eq. zero) then
                  t = w
                  if (w .eq. zero) t = machep * norm
                  h(i,en) = -r / t
                  go to 700
               endif
c
c              Solve real equations
               x = h(i,i+1)
               y = h(i+1,i)
               q = (wr(i) - p) * (wr(i) - p) + wi(i) * wi(i)
               t = (x * s - zz * r) / q
               h(i,en) = t
               if (abs(x) .gt. abs(zz)) then
                  h(i+1,en) = (-r - w * t) / x
                  go to 700
               endif
               h(i+1,en) = (-s - y * t) / zz
c
  700          continue
c
c           End real vector.
c
         else if (q .lt. zero) then
c
c           Complex vector.
            m = na
c           Last vector component chosen imaginary so that
c           eigenvector matrix is triangular
            if (abs(h(en,na)) .le. abs(h(na,en))) then
*              z3 = dcmpLx(zero,-h(na,en)) / dcmpLx(h(na,na)-p,q)
               call cdivsn (zero, -h(na,en), h(na,na)-p, q, t3r, t3i)
               h(na,na) = t3r
               h(na,en) = t3i
            else
               h(na,na) = q / h(en,na)
               h(na,en) = -(h(en,en) - p) / h(en,na)
            endif
c
            h(en,na) = zero
            h(en,en) = one
            enm2 = na - 1
            if (enm2 .eq. 0) go to 800
c
            do 790 ii = 1, enm2
               i = na - ii
               w = h(i,i) - p
               ra = 0.0
               sa = h(i,en)
c
               do 760 j = m,na
                  ra = ra + h(i,j) * h(j,na)
                  sa = sa + h(i,j) * h(j,en)
  760             continue
c
               if (wi(i) .lt. zero) then
                  zz = w
                  r = ra
                  s = sa
                  go to 790
               endif
c
               m = i
               if (wi(i) .eq. zero) then
*                 z3 = dcmplx(-ra,-sa) / dcmplx(w,q)
                  call cdivsn (-ra, -sa, w, q, t3r, t3i)
                  h(i,na) = t3r
                  h(i,en) = t3i
                  go to 790
               endif
c
c              Solve complex equations
               x = h(i,i+1)
               y = h(i+1,i)
               vr = (wr(i) - p) * (wr(i) - p) + wi(i) * wi(i) - q * q
               vi = (wr(i) - p) * 2.0 * q
               if (vr .eq. zero .and. vi .eq. zero) vr = machep * norm
     &          * (abs(w) + abs(q) + abs(x) + abs(y) + abs(zz))
*              z3 = dcmpLx(x*r-zz*ra+q*sa,x*s-zz*sa-q*ra)/dcmpLx(vr,vi)
               call cdivsn (x*r-zz*ra+q*sa, x*s-zz*sa-q*ra, vr, vi,
     &                      t3r, t3i)
               h(i,na) = t3r
               h(i,en) = t3i
               if (abs(x) .gt. abs(zz) + abs(q)) then
                  h(i+1,na) = (-ra - w * h(i,na) + q * h(i,en)) / x
                  h(i+1,en) = (-sa - w * h(i,en) - q * h(i,na)) / x
                  go to 790
               endif
c
*              z3 = dcmpLx(-r-y*h(i,na),-s-y*h(i,en)) / dcmpLx(zz,q)
               call cdivsn(-r-y*h(i,na), -s-y*h(i,en), zz, q, t3r, t3i)
               h(i+1,na) = t3r
               h(i+1,en) = t3i
c
  790          continue
c
c           End complex vector.
c
         endif
c
  800    continue
c
c        End back substitution.
c
c        vectors of isolated roots.
      do 840 i = 1, n
         if (i .lt. Low .or. i .gt. igh) then
            do 820 j = i, n
               z(i,j) = h(i,j)
  820          continue
         endif
  840    continue
c
c     Multiply by transformation matrix to give
c     vectors of original full matrix.
c
c     for j=n step -1 until Low do ...
      do 881 jj = Low, n
         j = n + Low - jj
         m = min(j,igh)
c
         do 880 i = Low, igh
            zz = zero
            do 860 k = Low, m
               zz = zz + z(i,k) * h(k,j)
  860          continue
            z(i,j) = zz
  880       continue
  881    continue
c
 1001 return
      end
c
c--------------------------------------------------------------------
c
      subroutine balbak (nm, n, low, igh, scale, m, z)
c
      integer i, j, k, m, n, ii, nm, igh, low
      real*8 scale(n), z(nm,m)
      real*8 s
c
      if (igh .ne. low) then
         do 110 i = low, igh
            s = scale(i)
c           left hand eigenvectors are back transformed
c           if the foregoing statement is replaced by
c           s = 1.0 / scale(i)
            do 100 j = 1, m
               z(i,j) = z(i,j) * s
  100          continue
  110       continue
      endif
c
c     for i=low-1 step -1 until 1,
c        for igh+1 step 1 until n do ...
      do 140 ii = 1, n
         i = ii
         if (i .lt. low .or. i .gt. igh) then
            if (i .lt. low) i = low - ii
            k = scale(i)
            if (k .ne. i) then
               do 130 j = 1, m
                  s = z(i,j)
                  z(i,j) = z(k,j)
                  z(k,j) = s
  130             continue
            endif
         endif
  140    continue
c
      return
      end

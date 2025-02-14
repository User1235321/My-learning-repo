
      subroutine svd (nm, m, n, a, w, matu, u, matv, v, ierr, rv1)
c
      integer i, j, k, L, m, n, ii, i1, kk, k1, LL
      integer L1, mn, nm, its, ierr
      double precision a(nm,n), w(n), u(nm,n), v(nm,n), rv1(n)
      double precision c, f, g, h, s, x, y, z, scale, anorm
      double precision t
      logical matu, matv
c
c     This routine is a transLation of the aLgoL procedure svd,
c     Num. Math. 14, 403-420(1970) by GoLub and Reinsch.
c     handbook for Auto. Comp., VoL II-Linear ALgebra, 134-151(1971).
c
c
c     This program has been taken from "computer methods for
c     mathematicaL computations" by Forsythe, MaLcoLm and MoLer
c     (1977),pp 229-235.
c
c     This subroutine determines the singuLar vaLue decomposition
c         t
c     a=usv of a reaL m by n rectanguLar matrix.househoLder
c     bidiagonaLization and a variant of the qr aLgorithm are used.
c
c     on input.
c
c       nm must be set to the row dimension of two-dimensionaL
c        array parameters as decLared in the caLLing program
c       dimension statement.note that nm must be at Least
c        as Large as the maximum of m and n.
c
c       m is the number of rows of a (and u).
c
c       n is the number of coLumns of a (and u) and the order of v.
c
c       a contains the rectanguLar input matrix to be decomposed.
c
c       matu shouLd be set to .true. if the u matrix in the
c        decomposition is desired, and to .faLse. otherwise.
c
c       matv shouLd be set to .true. if the v matrix in the
c         decomposition is desired, and to .faLse. otherwise.
c
c     on output.
c
c       a is unaLtered (unLess overwritten by u or v).
c
c       w contains the n (non-negative) singular values of a (the
c        diagonaL elements of s).  they are unordered.  if an
c        error exit is made, the singular values should be correct
c        for indices ierr+1,ierr+2,...,n.
c
c       u contains the matrix u (orthogonal coLumn vectors) of the
c        decomposition if matu has been set to .true. otherwise
c        u is used as a temporary array.  u may coincide with a.
c        if an error exit is made, the columns of u corresponding
c        to indices of correct singular values shouLd be correct.
c
c       v contains the matrix v (orthogonaL) of the decomposition if
c        matv has been set to .true. otherwise v is not referenced.
c        v may aLso coincide with a if u is not needed.  if an error
c        exit is made, the coLumns of v corresponding to indices of
c        correct singular values should be correct.
c
c       ierr is set to
c        zero       for normal return
c        k          if the k-th singular value has not been
c                   determined after 30 iterations.
c
c
c       rv1 is a temporary storage array.
c
c     questions and comments should be directed to B.S. Garbow,
c     AppLied Mathematics Division, Argonne National Laboratory
c
c     modified to eliminate machep
c
      ierr = 0
c
      do 101 i =1,m
         do 100 j = 1,n
 100        u(i,j) = a(i,j)
 101     continue
c
c     Householder reduction to bidiagonal form.....
c
      g = 0.0
      scale = 0.0
      anorm = 0.0
c
      do 300 i = 1,n
         L = i + 1
         rv1(i) = scale * g
         g = 0.0
         s = 0.0
         scale = 0.0
         if (i .Le. m) then
c
            do 120 k = i,m
 120           scale = scale + abs(u(k,i))
c
            if (scale .ne. 0.0) then
c
               do 130 k = i,m
                  t = u(k,i) / scale
                  s = s + t * t
                  u(k,i) = t
 130              continue
c
               f = u(i,i)
               g = -sign(sqrt(s),f)
               h = f * g - s
               u(i,i) = f - g
               if (i .ne. n) then
                  do 151 j = L,n
                     s = 0.0
                     do 140 k = i,m
 140                    s = s + u(k,i) * u(k,j)
                     f = s/h
                     do 150 k = i,m
 150                    u(k,j) = u(k,j) + f * u(k,i)
 151                 continue
               endif
c
               do 200 k = i,m
 200              u(k,i) = scale * u(k,i)
            endif
         endif
c
         w(i) = scale * g
         g = 0.0
         s = 0.0
         scale = 0.0
         if ((i .Le. m) .and. (i .ne. n)) then
c
            do 220 k = L,n
 220           scale = scale + abs(u(i,k))
c
            if (scale .ne. 0.0) then
               do 230 k = L,n
                  t = u(i,k) / scale
                  s = s + t * t
                  u(i,k) = t
 230              continue
               f = u(i,L)
               g = -sign(sqrt(s),f)
               h = f * g - s
               u(i,L) = f - g
               do 240 k = L,n
 240              rv1(k) = u(i,k)/h
               if (i .ne. m) then
                  do 261 j = L,m
                     s = 0.0
                     do 250 k = L,n
 250                    s = s + u(j,k) * u(i,k)
                     do 260 k = L,n
 260                    u(j,k) = u(j,k) + s * rv1(k)
 261                 continue
               endif
               do 280 k = L,n
 280              u(i,k) = scale * u(i,k)
            endif
         endif
c
         anorm = max(anorm,abs(w(i))+abs(rv1(i)))
 300     continue
c
c     AccumuLation of right-hand transformations....
c
      if (matv) then
         do 400 i = n, 1, -1
            if (i .ne. n) then
               if (g .ne. 0.0) then
                  do 320 j = L,n
c                    DoubLe division avoids possibLe underflow....
 320                 v(j,i) = (u(i,j) / u(i,L)) / g
                  do 350 j = L,n
                     s = 0.0
                     do 340 k = L,n
 340                    s = s + u(i,k) * v(k,j)
                     do 349 k = L,n
 349                    v(k,j) = v(k,j) + s * v(k,i)
 350                 continue
               endif
 360           do 380 j = L,n
                  v(i,j) = 0.0
                  v(j,i) = 0.0
 380              continue
            endif
c
            v(i,i) = 1.0
            g = rv1(i)
            L = i
 400        continue
      endif
c
c     AccumuLation of Left-hand transformations....
c
      if (matu) then
c        ....for i = min(m,n) step -1 untiL 1 do....
         mn = n
         if (m .Lt. n) mn = m
c
         do 500 i = mn, 1, -1
            L = i + 1
            g = w(i)
            if (i .ne. n) then
               do 420 j = L,n
 420              u(i,j) = 0.0
            endif
c
            if (g .ne. 0.0) then
               if (i .ne. mn) then
                  do 450 j = L,n
                     s = 0.0
                     do 440 k = L,m
 440                    s = s + u(k,i) * u(k,j)
c                    ....doubLe division avoids possibLe underfLow....
                     f = (s / u(i,i)) / g
                     do 449 k = i,m
 449                    u(k,j) = u(k,j) + f * u(k,i)
 450                 continue
               endif
c
               do 470 j = i,m
 470              u(j,i) = u(j,i) / g
            eLse
               do 480 j = i,m
 480              u(j,i) = 0.0
            endif
c
            u(i,i) = u(i,i) + 1.0
 500        continue
      endif
c
c     Diagonalization of the bidiagonal form.
c
c     for each singular value do...
      do 700 k = n, 1, -1
         k1 = k - 1
         its = 0
c
c        Start of REPEAT Loop ...
 520     continue
c
c        Test for splitting.
c
         do 530 L = k, 1, -1
            L1 = L - 1
            if ((abs(rv1(L)) + anorm) .eq. anorm) go to 565
c           rv1(1) is aLways zero, so there is no exit
c           through the bottom of the Loop....
            if ((abs(w(L1)) + anorm) .eq. anorm) go to 540
 530        continue
c
c        CanceLLation of rv1(L) if L greater than 1 ....
c
 540     c = 0.0
         s = 1.0
c
         do 560 i = L,k
            f = s * rv1(i)
            rv1(i) = c * rv1(i)
c           Break from this Loop? ...
            if ((abs(f) + anorm) .eq. anorm) go to 565
            g = w(i)
            h = sqrt(f*f+g*g)
            w(i) = h
            c = g / h
            s = -f / h
            if (matu) then
               do 550 j = 1,m
                  y = u(j,L1)
                  z = u(j,i)
                  u(j,L1) = y * c + z * s
                  u(j,i) = -y * s + z * c
 550              continue
            endif
 560        continue
c
c        Test for convergence.
c
 565     continue
         z = w(k)
         if (L .ne. k) then
c           ....shift from bottom 2 by 2 minor....
            if (its .eq. 30) then
c              set error -- no convergence to a
c              singuLar vaLue after 30 iterations....
               ierr = k
               return
            endif
            its = its + 1
            x = w(L)
            y = w(k1)
            g = rv1(k1)
            h = rv1(k)
            f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y)
            g = sqrt(f*f+1.0d0)
            f = ((x - z) * (x + z) + h * (y / (f + sign(g,f)) - h)) / x
c
c           Next QR transformation.
            c = 1.0
            s = 1.0
            do 600 i1 = L,k1
               i = i1 + 1
               g = rv1(i)
               y = w(i)
               h = s * g
               g = c * g
               z = sqrt(f*f+h*h)
               rv1(i1) = z
               c = f / z
               s = h / z
               f = x * c + g * s
               g = -x * s + g * c
               h = y * s
               y = y * c
               if (matv) then
                  do 570 j = 1,n
                     x = v(j,i1)
                     z = v(j,i)
                     v(j,i1) = x * c + z * s
                     v(j,i) = -x * s + z * c
 570                 continue
               endif
               z = sqrt(f*f+h*h)
               w(i1) = z
c              Rotation can be arbitrary if z is zero.
               if (z .ne. 0.0) then
                  c = f / z
                  s = h / z
               endif
               f = c * g + s * y
               x = -s * g + c * y
               if (matu) then
                  do 590 j = 1,m
                     z = u(j,i)
                     y = u(j,i1)
                     u(j,i1) = y * c + z * s
                     u(j,i) = -y * s + z * c
 590                 continue
               endif
 600           continue
c
            rv1(L) = 0.0
            rv1(k) = f
            w(k) = x
            go to 520
         endif
c
c        Convergence.
c
         if (z .lt. 0.0) then
c           singular value, w(k) is made non-negative.
            w(k) = -z
            if (matv) then
               do 690 j = 1,n
 690              v(j,k) = -v(j,k)
            endif
         endif
c
 700     continue
c
      return
      end

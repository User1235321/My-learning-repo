        subroutine lsp(n,m,x,y,c,work,shift,relerr,resid,ierr)
c
c       purpose         :given a set of m data points x and y,such that
c                        y=y(x),the n coefficients c of a least
c                        squares polynomial are computed.the polynomial is
c                        y=c(1)+c(2)*z+c(3)*z**2+..+c(n)*z**(n-1)
c                        where z = x - shift.
c
c       date            :august 1983.
c
c       written by      :john d. day
c
c       version         :1
c
c       parameters      :n      :integer
c                        m      :integer
c                        x      :array(1..m) of double precision
c                        y      :array(1..m) of double precision
c                        c      :array(1..n) of double precision
c                        work   :array(1..3*m*n+2*n) of double precision
c                        shift  :double precision
c                        relerr :double precision
c                        ierr   :integer
c
c       input           :n      :number of coefficients in least squares
c                                polynomial.the order of the polynomial
c                                is (n-1).
c                        m      :number of data points.
c                        x      :data points ,x coordinate.
c                        y      :data points ,y coordinate.
c                        work   :work space.no input required.
c                        shift  :origin shift for x.a suitable centering
c                                value can improve the accuracy of the
c                                result.
c                        relerr :relative error of the data.(e.g) if the
c                                data is correct to 3 significant figures
c                                then set relerr=0.001. if data is exact,
c                                set relerr=0.0.
c
c       output          :c      :coefficients of shifted least squares
c                                polynomial.
c                        resid  :square root of the sum of squares of
c                                the residuals.
c                        ierr   :error  =0,normal return
c                                       =k,if k th singular value
c                                          has not been computed
c                                          in 30 iterations.
c
        integer n,m,ierr,n1,n2,n3,n4
        double precision x(m),y(m),c(n),work(1),shift,relerr,resid,t
c
c       check relerr for minimum allowable value .
c
        t = n*1.0e-16
        relerr = max(relerr,t)
c
c       interface routine for splitting up work array.
c
        n1 = m*n + 1
        n2 = m*n + n1
        n3 = m*n + n2
        n4 = n3 + n
        call lsp1(n,m,x,y,c,work(1),work(n1),work(n2),work(n3),work(n4),
     #            shift,relerr,resid,ierr)
        return
        end
        subroutine lsp1(n,m,x,y,c,a,u,v,sigma,vwork,shift,relerr,
     #                  resid,ierr)
c
c       interfacing routine with lsp.
c
        integer n,m,ierr,i,j
        double precision x(m),y(m),c(n),a(m,n),u(m,n),v(m,n),sigma(n),
     #                   vwork(n),
     #          shift,relerr,resid,sigmax,tau,t,tt
c
c       set up design matrix.
c
        do 20 i = 1,m
          a(i,1) = 1.0
          do 10 j = 2,n
            a(i,j) = (x(i) - shift)*a(i,j-1)
   10     continue
   20   continue
c
c       decomposition.
c
        call svd(m,m,n,a,sigma,.true.,u,.true.,v,ierr,vwork)
        if (ierr .ne. 0) return
c
c       find largest singular value,reset coefficients.
c
        sigmax = 0.0
        do 30 j = 1,n
          c(j) = 0.0
          sigmax = max(sigmax,sigma(j))
   30   continue
c
c       absolute error tolerance.
c
        tau = relerr*sigmax
c
c       find coefficients.
c
        do 60 j = 1,n
          if (sigma(j) .gt. tau) then
            t = 0.0
            do 40 i = 1,m
              t = t + u(i,j)*y(i)
   40       continue
            t = t/sigma(j)
            do 50 i = 1,n
              c(i) = c(i) + t*v(i,j)
   50       continue
          end if
   60   continue
c
c       find square root of sum of squares of residuals.
c
        t = 0.0
        do 80 i = 1,m
          tt = 0.0
          do 70 j = 1,n
            tt = tt + c(j)*a(i,j)
   70     continue
          t = t + (tt - y(i))**2
   80   continue
        resid = sqrt(t)
        return
        end



        double precision function lspval(xx,shift,c,n)
c
c       purpose         :evaluates the n-1 th order polynomial
c                        yy=c(1)+c(2)*x+c(3)*x**2+..+c(n)*x**(n-1).
c                               where x = xx-shift.
c
c       date            :august 1983
c
c       written         :john d. day
c
c       version         :1
c
c       parameters      :xx     :double precision.
c                        shift  :double precision
c                        c      :array(1..n) of double precision.
c                        n      :integer.
c
c       input           :xx,    :value of x at which polynomial y(x) is
c                        shift      to be evaluated.x=xx-shift.
c                        c      :coefficients of polynomial.
c                        n      :number of coefficients.
c
        double precision c(n),xx,shift,t
        integer i,j
        t = 0.0
        do 10 i = 1,n
          j = n + 1 - i
          t = (xx - shift)*t + c(j)
   10   continue
        lspval = t
        return
        end



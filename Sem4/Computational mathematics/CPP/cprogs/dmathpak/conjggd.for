      program conjgd
c
c     try out the function minimizer conjgg()
c
      real*8 ax, bx, cx, fa, fb, fc, object, linem, fmin, xmin
      external object, deriv
      real*8 work(3), p(3), dirctn(3), xmax, work2(3), work3(3)
      real*8 ftol
      integer flag, nfe, iter, nje
c
      ax = 1.0d0
      bx = 2.0d0
      xmax = 100.0d0
      dirctn(1) = 0.7071d0
      dirctn(2) = -0.7071d0
      p(1) = 0.0d0
      p(2) = 0.0d0
      nfe = 0
c
      write (*,*) 'Bracketing example'
      call braket (object, p, dirctn, 2, ax, bx, cx, xmax, fa, fb, fc,
     &             work, nfe, flag)
c
      write (*,*) 'flag = ', flag, ' .. nfe = ', nfe
      write (*,*) 'ax = ', ax, ' .. fa = ', fa
      write (*,*) 'bx = ', bx, ' .. fb = ', fb
      write (*,*) 'cx = ', cx, ' .. fc = ', fc
c
      write (*,*)
      write (*,*) 'line minimization example'
      nfe = 0
      fmin = linem (object, p, dirctn, 2, 1.0d-6, 100,
     &              xmin, xmax, work, nfe, flag)
c
      write (*,*) 'flag = ', flag, ' .. nfe = ', nfe
      write (*,*) 'x1 = ', p(1), ' .. x2 = ',p(2)
      write (*,*) 'xmin = ', xmin, ' .. fmin = ', fmin
c
      write (*,*)
      write (*,*) 'multi-dimensional minimizer'
      p(1) = -1.0d0
      p(2) = 1.0d0
      ftol = 1.0D-6
      call conjgg (object, deriv, p, 2, ftol, fmin, flag, 100, xmax,
     &             iter, nfe, nje, work, work2, work3, dirctn)
c
      write (*,*) 'flag = ', flag, ' .. nfe = ', nfe, ' .. nje = ', nje
      write (*,*) 'iter = ', iter
      write (*,*) 'x1 = ', p(1), ' .. x2 = ', p(2)
      write (*,*) 'fmin = ', fmin
c
      end



      real*8 function object (x)
c
c     test function to minimize
c     This is the function used as the example in the NAG manual.
c
      real*8 x(*), ex
      ex = exp(x(1))
      object = ex * (4.0d0*x(1)*(x(1)+x(2)) + 2.0d0*x(2)*(x(2)+1.0d0) +
     &               1.0d0)
      return
      end



      subroutine deriv (x, dx)
c
c     derivatives of test function
c
      real*8 x(*), dx(*), ex
      ex = exp(x(1))
      fx = ex * (4.0d0*x(1)*(x(1)+x(2)) + 2.0d0*x(2)*(x(2)+1.0d0) +
     &           1.0d0)
      dx(1) = fx + 4.0d0 * ex * (2.0d0 * x(1) + x(2))
      dx(2) = 4.0d0 * ex * (x(1) + x(2) + 0.5d0)
      return
      end

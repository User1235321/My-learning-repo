      program brakd
c
c     try out the numerical recipes function minimization
c     in multi dimensions
c
      real*8 ax, bx, cx, fa, fb, fc, object
      external object
      real*8 w1(3), w2(3), w3(3), w4(3), p(3), dirctn(3)
      integer flag, nfe
c
      ax = 1.0d0
      bx = 2.0d0
      dirctn(1) = 0.5774d0
      dirctn(2) = 0.5774d0
      dirctn(3) = 0.5774d0
      p(1) = 0.0d0
      p(2) = 0.0d0
      p(3) = 0.0d0
      nfe = 0
c
      call braket (object, p, dirctn, 3, ax, bx, cx, fa, fb, fc,
     &             w1, w2, w3, w4, nfe, flag)
c
      write (*,*) flag, nfe
      write (*,*) ax, fa
      write (*,*) bx, fb
      write (*,*) cx, fc
c
      end



      real*8 function object (x)
c
c     test function to minimize
c
      real*8 x(3), r
      r = sqrt(x(1)**2 + x(2)**2 + x(3)**2)
      object = sin(r)
      return
      end
